/*
 * Copyright (c) 2018-2020 Olivier Ayache <olivier.ayache@gmail.com>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include "avcodec.h"
#include "internal.h"
#include <android/native_window_jni.h>
#include <media/NdkMediaCodec.h>
#include <media/NdkImageReader.h>
#include <media/NdkImage.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>

typedef struct MediaCodecEncContext {
    AMediaCodec* codec;
    AVCodecContext *avctx;
    const AVClass *class; // needed by private avoptions
    enum AVPixelFormat preferred_fmt;
    
} MediaCodecEncContext;

static const AVOption options[]={
    { "preferred_fmt", "Sets the preferred pixel format ", offsetof(MediaCodecEncContext, preferred_fmt), AV_OPT_TYPE_PIXEL_FMT, AV_OPT_FLAG_VIDEO_PARAM | AV_OPT_FLAG_DECODING_PARAM },
    { NULL }
};

static const AVClass mediacodec_class = {
    .class_name = "MediaCodec encoder",
    .item_name  = av_default_item_name,
    .option     = options,
    .version    = LIBAVUTIL_VERSION_INT,
};

static int getColorFormatFromPixelFormat(enum AVPixelFormat fmt) {
    switch (fmt) {
        case AV_PIX_FMT_YUV420P:
            return 19;
        case AV_PIX_FMT_NV12:
            return 21;
    }
    return -1;
}

static enum AVPixelFormat getPixelFormatFromColorFormat(int fmt) {
    switch (fmt) {
        case 19:
            return AV_PIX_FMT_YUV420P;
        case 21:
            return AV_PIX_FMT_NV12;
    }
    return AV_PIX_FMT_NONE;
}

static int mediacodec_encode_init(AVCodecContext *avccontext) {
    MediaCodecEncContext *context = avccontext->priv_data;
    int fmt;
    AMediaFormat* format;
    enum AVPixelFormat pix_fmt;
    media_status_t configure;
    const char* mime_type;

    context->avctx = avccontext;
 
    if (avccontext->codec_id == AV_CODEC_ID_HEVC) {
        mime_type = "video/hevc";
    } else if (avccontext->codec_id == AV_CODEC_ID_H264) {
        mime_type = "video/avc";
    } else if (avccontext->codec_id == AV_CODEC_ID_MPEG2VIDEO) {
        mime_type = "video/mpeg2";
    } else if (avccontext->codec_id == AV_CODEC_ID_MPEG4) {
        mime_type = "video/mp4v-es";
    } else if (avccontext->codec_id == AV_CODEC_ID_VP8) {
        mime_type = "video/x-vnd.on2.vp8";
    } else if (avccontext->codec_id == AV_CODEC_ID_VP9) {
        mime_type = "video/x-vnd.on2.vp9";
    }
    
    context->codec = AMediaCodec_createEncoderByType(mime_type);
    
    format = AMediaFormat_new();
    AMediaFormat_setString(format, AMEDIAFORMAT_KEY_MIME, mime_type);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_HEIGHT, avccontext->height);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_WIDTH, avccontext->width);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_BIT_RATE, avccontext->bit_rate);
    AMediaFormat_setFloat(format, AMEDIAFORMAT_KEY_FRAME_RATE, av_q2d(avccontext->framerate));
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_COLOR_FORMAT, getColorFormatFromPixelFormat(avccontext->pix_fmt != AV_PIX_FMT_NONE ? avccontext->pix_fmt : avccontext->codec->pix_fmts[0]));
    
    av_log(avccontext, AV_LOG_ERROR, AMediaFormat_toString(format));
    
//    ff_get_format(avccontext, avccontext->codec->pix_fmts);
//    
//    pix_fmt = AV_PIX_FMT_NONE;
//    
//    if (avccontext->hwaccel_context && ((MediaCodecSurfaceContext*)avccontext->hwaccel_context)->surface_object){
//        MediaCodecSurfaceContext* hwaccel_context = ((MediaCodecSurfaceContext*)avccontext->hwaccel_context);
//        hwaccel_context->surface = ANativeWindow_fromSurface(hwaccel_context->env, hwaccel_context->surface_object);
//        configure = AMediaCodec_configure(context->codec, format, hwaccel_context->surface, NULL, 0);
//        pix_fmt = AV_PIX_FMT_MEDIACODEC;
//    } else {
        configure = AMediaCodec_configure(context->codec, format, NULL, NULL, AMEDIACODEC_CONFIGURE_FLAG_ENCODE);
//    }
    
    if (configure == AMEDIA_OK) {
        AMediaFormat_getInt32(AMediaCodec_getOutputFormat(context->codec), AMEDIAFORMAT_KEY_COLOR_FORMAT, &fmt);
        avccontext->pix_fmt = pix_fmt != AV_PIX_FMT_NONE ? pix_fmt : getPixelFormatFromColorFormat(fmt);
        av_log(avccontext, AV_LOG_DEBUG, "Encoder configured %s\n", AMediaFormat_toString(AMediaCodec_getOutputFormat(context->codec)));
        return AMediaCodec_start(context->codec);
    } else {
        return configure;
    }
}

static int mediacodec_encode_frame(AVCodecContext *avccontext, AVPacket *avpkt, const AVFrame *frame,
                   int *got_packet_ptr) {
    
    MediaCodecEncContext *context = avccontext->priv_data;
    AMediaCodec* codec = context->codec;
    ssize_t index, indexOutput;
    uint8_t *inputBuffer;
    
    size_t bufferSize;
    media_status_t status;
    AMediaCodecBufferInfo info;

    index = AMediaCodec_dequeueInputBuffer(codec, 200);
    if (index < 0) {
        return 0;
    }
    inputBuffer = AMediaCodec_getInputBuffer(codec, index, &bufferSize);
    av_log(avccontext, AV_LOG_DEBUG, "input buffer acquired (index: %ld capacity: %ld)\n", index, bufferSize);
    
    if (frame->data) {
        bufferSize = av_image_get_buffer_size(avccontext->pix_fmt, avccontext->width, avccontext->height, 1);
        av_image_copy_to_buffer(inputBuffer, bufferSize, frame->data, frame->linesize, context->preferred_fmt, avccontext->width, avccontext->height, 1);
        status = AMediaCodec_queueInputBuffer(codec, index, 0, bufferSize, 1000000LL * frame->pts * avccontext->time_base.num / avccontext->time_base.den, 0);
        av_log(avccontext, AV_LOG_DEBUG, "input buffer queued (pts: %lld)\n", 1000000LL * frame->pts * avccontext->time_base.num / avccontext->time_base.den);
    }else{
        status = AMediaCodec_queueInputBuffer(codec, index, 0, 0, 0, AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);
        av_log(avccontext, AV_LOG_DEBUG, "end of stream reached");
    }
    
    if (status != AMEDIA_OK) {
        return status;
    }
    
    indexOutput = AMediaCodec_dequeueOutputBuffer(codec, &info, 200);
    if (indexOutput >= 0) {

        AMediaFormat* outputFormat;
        uint8_t *outputBuffer;
        
        outputBuffer = AMediaCodec_getOutputBuffer(codec, indexOutput, &bufferSize);
        outputFormat = AMediaCodec_getOutputFormat(codec);
        av_log(avccontext, AV_LOG_DEBUG, "output buffer acquired (format: %s)\n", AMediaFormat_toString(outputFormat));
               
        memcpy(avpkt->data, outputBuffer, info.size);
        avpkt->size = info.size;
        avpkt->pts = info.presentationTimeUs * avccontext->time_base.den / (avccontext->time_base.num * 1000000LL);
        
        if (info.flags) {
            avpkt->flags |= AV_PKT_FLAG_KEY;
        }

        status = AMediaCodec_releaseOutputBuffer(codec, indexOutput, false);
        
        if (status != AMEDIA_OK) {
            return status;
        }
        
        *got_packet_ptr = 1;
    } else {
        *got_packet_ptr = 0;
    }
    
    return 0;
    
    
    
}

static int mediacodec_encode_close(AVCodecContext *avccontext) {
    media_status_t status;
    MediaCodecEncContext *context = avccontext->priv_data;
    status = AMediaCodec_flush(context->codec);
    if (status != AMEDIA_OK){
        return status;
    }
    status = AMediaCodec_stop(context->codec);
    if (status != AMEDIA_OK){
        return status;
    }
    return AMediaCodec_delete(context->codec);
}

#define ATTR(x) .x
#define CODECNAME(name) mediacodec_ ## name
#define DECLARE_ENCODER(cname, cid) \
AVCodec ff_mediacodec_##cname##_encoder = { \
    ATTR(name)             = AV_STRINGIFY(CODECNAME(cname)), \
    ATTR(long_name)        = NULL_IF_CONFIG_SMALL("Android MediaCodec encoder"), \
    ATTR(type)             = AVMEDIA_TYPE_VIDEO, \
    ATTR(id)               = cid, \
    ATTR(priv_data_size)   = sizeof (MediaCodecEncContext), \
    ATTR(init)             = mediacodec_encode_init, \
    ATTR(encode2)          = mediacodec_encode_frame, \
    ATTR(close)            = mediacodec_encode_close, \
    ATTR(capabilities)     = AV_CODEC_CAP_DELAY, \
    ATTR(pix_fmts)         = (const enum AVPixelFormat[]) {AV_PIX_FMT_YUV420P, AV_PIX_FMT_NV12, AV_PIX_FMT_MEDIACODEC} \
}; \
\
AVHWAccel ff_mediacodec_##cname##_hwaccel = { \
    ATTR(name)           = AV_STRINGIFY(CODECNAME(cname)), \
    ATTR(type)           = AVMEDIA_TYPE_VIDEO, \
    ATTR(id)             = cid, \
    ATTR(pix_fmt)        = AV_PIX_FMT_MEDIACODEC, \
}; \


DECLARE_ENCODER(hevc, AV_CODEC_ID_HEVC)
DECLARE_ENCODER(h264, AV_CODEC_ID_H264)
DECLARE_ENCODER(mpeg2, AV_CODEC_ID_MPEG2VIDEO)
DECLARE_ENCODER(mpeg4, AV_CODEC_ID_MPEG4)
DECLARE_ENCODER(vp8, AV_CODEC_ID_VP8)
DECLARE_ENCODER(vp9, AV_CODEC_ID_VP9)

static void init_and_register_all_mediacodec(AVCodec* codec) {
    
    AMediaCodec* encoder;
    
    encoder = AMediaCodec_createEncoderByType("video/avc");
    if (encoder) {
        AMediaCodec_delete(encoder);
        avcodec_register(&ff_mediacodec_h264_encoder);
        av_register_hwaccel(&ff_mediacodec_h264_hwaccel);
    }

    encoder = AMediaCodec_createEncoderByType("video/hevc");
    if (encoder) {
        AMediaCodec_delete(encoder);
        avcodec_register(&ff_mediacodec_hevc_encoder);
        av_register_hwaccel(&ff_mediacodec_hevc_hwaccel);
    }
    
    encoder = AMediaCodec_createEncoderByType("video/mpeg2");
    if (encoder) {
        AMediaCodec_delete(encoder);
        avcodec_register(&ff_mediacodec_mpeg2_encoder);
        av_register_hwaccel(&ff_mediacodec_mpeg2_hwaccel);
    }
    
    encoder = AMediaCodec_createEncoderByType("video/mp4v-es");
    if (encoder) {
        AMediaCodec_delete(encoder);
        avcodec_register(&ff_mediacodec_mpeg4_encoder);
        av_register_hwaccel(&ff_mediacodec_mpeg4_hwaccel);
    }
    

    encoder = AMediaCodec_createEncoderByType("video/x-vnd.on2.vp8");
    if (encoder) {
        AMediaCodec_delete(encoder);
        avcodec_register(&ff_mediacodec_vp8_encoder);
        av_register_hwaccel(&ff_mediacodec_vp8_hwaccel);
    }

    encoder = AMediaCodec_createEncoderByType("video/x-vnd.on2.vp9");
    if (encoder) {
        AMediaCodec_delete(encoder);
        avcodec_register(&ff_mediacodec_vp9_encoder);
        av_register_hwaccel(&ff_mediacodec_vp9_hwaccel);
    }
    
}

AVCodec ff_mediacodec_encoder = {
   .name = "mediacodec", 
   .init_static_data = init_and_register_all_mediacodec
};

