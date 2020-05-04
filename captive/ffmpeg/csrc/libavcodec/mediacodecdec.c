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

#include "mediacodec.h"
#include "avcodec.h"
#include "bytestream.h"
#include "internal.h"
#include "h2645_parse.h"
#include "h264_parse.h"
#include "hevc_parse.h"
#include <android/native_window_jni.h>
#include <media/NdkMediaCodec.h>
#include <media/NdkImageReader.h>
#include <media/NdkImage.h>
#include <libavutil/imgutils.h>
#include <libavutil/opt.h>

const uint8_t startCode [] = {0x00, 0x00, 0x00, 0x01};

typedef struct MediaCodecDecContext {
    AMediaCodec* codec;
    AMediaFormat* format;
    uint8_t* csd;
    AVCodecContext *avctx;
    H264ParamSets h264_ps;
    HEVCParamSets hevc_ps;
    HEVCSEIContext hevc_sei;
    H2645Packet pkt;
    int is_nalff; ///< this flag is != 0 if bitstream is encapsulated
    int nal_length_size; ///< Number of bytes used for nal length (1, 2 or 4)
    int nuh_layer_id;
    const AVClass *class; // needed by private avoptions
    enum AVPixelFormat preferred_fmt;

} MediaCodecDecContext;

typedef struct MediaCodecSurfaceContext {
    JNIEnv* env;
    jobject surface_object;
    ANativeWindow* surface;
} MediaCodecSurfaceContext;

typedef struct MediaCodecSurfaceRenderContext {
    AMediaCodec* codec;
    int index;
} MediaCodecSurfaceRenderContext;


static const AVOption options[] = {
    { "preferred_fmt", "Sets the preferred pixel format ", offsetof(MediaCodecDecContext, preferred_fmt), AV_OPT_TYPE_PIXEL_FMT, AV_OPT_FLAG_VIDEO_PARAM | AV_OPT_FLAG_DECODING_PARAM},
    { NULL}
};

static const AVClass mediacodec_class = {
    .class_name = "MediaCodec decoder",
    .item_name = av_default_item_name,
    .option = options,
    .version = LIBAVUTIL_VERSION_INT,
};

void mediacodec_alloc_context(AVCodecContext* avcontext, JNIEnv* env, jobject surface_object) {
    MediaCodecSurfaceContext* context = av_mallocz(sizeof (MediaCodecSurfaceContext));
    if (avcontext && env && surface_object && context) {
        context->env = env;
        context->surface_object = (*env)->NewGlobalRef(env, surface_object);
        avcontext->hwaccel_context = context;
    } else {
        av_free(context);
    }
}

static MediaCodecSurfaceRenderContext* mediacodec_alloc_surface_context(AMediaCodec* codec, int index) {
    MediaCodecSurfaceRenderContext* context = av_mallocz(sizeof (MediaCodecSurfaceRenderContext));
    if (codec && context) {
        context->codec = codec;
        context->index = index;
    } else {
        av_freep(&context);
    }
    return context;
}

void mediacodec_free_context(AVCodecContext* avcontext) {
    if (avcontext->hwaccel_context) {
        MediaCodecSurfaceContext* context = avcontext->hwaccel_context;
        JNIEnv* env = context->env;
        (*env)->DeleteGlobalRef(env, context->surface_object);
    }
    av_freep(&avcontext->hwaccel_context);
}

void mediacodec_render_frame(AVFrame* frame) {
    MediaCodecSurfaceRenderContext* context = (MediaCodecSurfaceRenderContext*) frame->data[3];
    AMediaCodec_releaseOutputBuffer(context->codec, context->index, true);
    av_freep(&frame->data[3]);
}

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

static media_status_t configure_and_start(MediaCodecDecContext* context, int error_on_bad_dimensions) {
    media_status_t status;
    int fmt;
    enum AVPixelFormat pix_fmt = AV_PIX_FMT_NONE;
    AVCodecContext* avccontext = context->avctx;
    
    if (avccontext->width <= 0 || avccontext->height <= 0) {
        av_log(avccontext, AV_LOG_WARNING, "width (%d) or height (%d) is invalid\n", avccontext->width, avccontext->height);
        return error_on_bad_dimensions ? AMEDIA_ERROR_INVALID_PARAMETER : AMEDIA_OK;
    }
    AMediaFormat_setInt32(context->format, AMEDIAFORMAT_KEY_HEIGHT, avccontext->height);
    AMediaFormat_setInt32(context->format, AMEDIAFORMAT_KEY_WIDTH, avccontext->width);   
    
    if (avccontext->hwaccel_context) {
        MediaCodecSurfaceContext* hwaccel_context = ((MediaCodecSurfaceContext*) avccontext->hwaccel_context);
        status = AMediaCodec_configure(context->codec, context->format, hwaccel_context->surface, NULL, 0);
        pix_fmt = AV_PIX_FMT_MEDIACODEC;
    } else {
        status = AMediaCodec_configure(context->codec, context->format, NULL, NULL, 0);
    }
    
    status |= AMediaFormat_delete(context->format);

    if (status == AMEDIA_OK) {
        AMediaFormat_getInt32(AMediaCodec_getOutputFormat(context->codec), AMEDIAFORMAT_KEY_COLOR_FORMAT, &fmt);
        avccontext->pix_fmt = pix_fmt != AV_PIX_FMT_NONE ? pix_fmt : getPixelFormatFromColorFormat(fmt);
        av_log(avccontext, AV_LOG_DEBUG, "Decoder configured %s\n", AMediaFormat_toString(AMediaCodec_getOutputFormat(context->codec)));
        status = AMediaCodec_start(context->codec);
    }

    return status;
}

static int mediacodec_decode_init(AVCodecContext *avccontext) {
    MediaCodecDecContext *context = avccontext->priv_data;
    int csd_size, index;
    const char* mime_type;

    context->avctx = avccontext;
    csd_size = 0;
    
    if (avccontext->codec_id == AV_CODEC_ID_HEVC) {
        mime_type = "video/hevc";
        ff_hevc_decode_extradata(avccontext->extradata, avccontext->extradata_size, &context->hevc_ps, &context->hevc_sei, &context->is_nalff, &context->nal_length_size, avccontext->err_recognition, 0, avccontext);
        if (!context->hevc_ps.vps_list[0] || !context->hevc_ps.sps_list[0] || !context->hevc_ps.pps_list[0]) {
           return AVERROR(EINVAL); 
        }
        csd_size = sizeof (uint8_t) * (((HEVCVPS*)context->hevc_ps.vps_list[0]->data)->data_size + ((HEVCPPS*)context->hevc_ps.pps_list[0]->data)->data_size + ((HEVCSPS*)context->hevc_ps.sps_list[0]->data)->data_size) + sizeof (startCode)*2;
        av_log(avccontext, AV_LOG_DEBUG, "extradata decoded, stream %s (nal length: %d)\n", context->is_nalff ? "not encapsulated" : "encapsulated", context->nal_length_size);
    } else if (avccontext->codec_id == AV_CODEC_ID_H264) {
        mime_type = "video/avc";
        ff_h264_decode_extradata(avccontext->extradata, avccontext->extradata_size, &context->h264_ps, &context->is_nalff, &context->nal_length_size, avccontext->err_recognition, avccontext);
        if (!context->h264_ps.pps_list[0] || !context->h264_ps.sps_list[0]) {
            return AVERROR(EINVAL); 
        }
        csd_size = sizeof (uint8_t) * (((PPS*)context->h264_ps.pps_list[0]->data)->data_size + ((SPS*)context->h264_ps.sps_list[0]->data)->data_size) + sizeof (startCode)*2;
        av_log(avccontext, AV_LOG_DEBUG, "extradata decoded, stream %s (nal length: %d)\n", context->is_nalff ? "not encapsulated" : "encapsulated", context->nal_length_size);

    } else if (avccontext->codec_id == AV_CODEC_ID_MPEG4) {
        mime_type = "video/mp4v-es";
        if (!avccontext->extradata) {
            return AVERROR(EINVAL);
        }
        csd_size = sizeof (uint8_t) * (avccontext->extradata_size);
    } else if (avccontext->codec_id == AV_CODEC_ID_VP8) {
        mime_type = "video/x-vnd.on2.vp8";
    } else if (avccontext->codec_id == AV_CODEC_ID_VP9) {
        mime_type = "video/x-vnd.on2.vp9";
    }

    index = 0;

    if (csd_size > 0) {
        context->csd = av_malloc(csd_size);
        if (!context->csd) {
            return AVERROR(ENOMEM);
        }
    }
    
    if (context->hevc_ps.vps_list[0]) {
        struct HEVCVPS* vps = ((HEVCVPS*)context->hevc_ps.vps_list[0]->data);
        struct HEVCSPS* sps = ((HEVCSPS*)context->hevc_ps.sps_list[0]->data);
        struct HEVCPPS* pps = ((HEVCPPS*)context->hevc_ps.pps_list[0]->data);        
        memcpy(context->csd, startCode, sizeof (startCode));
        index += sizeof (startCode);
        memcpy(context->csd + index, vps->data, vps->data_size);
        index += vps->data_size;
        memcpy(context->csd + index, startCode, sizeof (startCode));
        index += sizeof (startCode);
        memcpy(context->csd + index, sps->data, sps->data_size);
        index += sps->data_size;
        memcpy(context->csd + index, startCode, sizeof (startCode));
        index += sizeof (startCode);
        memcpy(context->csd + index, pps->data, pps->data_size);
    }
    
    if (context->h264_ps.pps_list[0]) {
        struct SPS* sps = ((SPS*)context->h264_ps.sps_list[0]->data);
        struct PPS* pps = ((PPS*)context->h264_ps.pps_list[0]->data);
        memcpy(context->csd + index, startCode, sizeof (startCode));
        index += sizeof (startCode);
        memcpy(context->csd + index, sps->data, sps->data_size);
        index += sps->data_size;
        memcpy(context->csd + index, startCode, sizeof (startCode));
        index += sizeof (startCode);
        memcpy(context->csd + index, pps->data, pps->data_size);
    }
    
    if (avccontext->codec_id == AV_CODEC_ID_MPEG4) {
        memcpy(context->csd + index, avccontext->extradata, avccontext->extradata_size); 
    }
    
    av_log(avccontext, AV_LOG_DEBUG, "decoder private data initialized (%d bytes)\n", csd_size);

    context->codec = AMediaCodec_createDecoderByType(mime_type);

    context->format = AMediaFormat_new();
    
    AMediaFormat* format = context->format; 
    AMediaFormat_setString(format, AMEDIAFORMAT_KEY_MIME, mime_type);
    AMediaFormat_setInt32(format, AMEDIAFORMAT_KEY_COLOR_FORMAT, getColorFormatFromPixelFormat(avccontext->pix_fmt != AV_PIX_FMT_NONE ? avccontext->pix_fmt : avccontext->codec->pix_fmts[0]));
    if (context->csd) {
        AMediaFormat_setBuffer(format, "csd-0", context->csd, csd_size);
    }

    ff_get_format(avccontext, avccontext->codec->pix_fmts);
        
    if (avccontext->hwaccel_context && ((MediaCodecSurfaceContext*) avccontext->hwaccel_context)->surface_object) {
        MediaCodecSurfaceContext* hwaccel_context = ((MediaCodecSurfaceContext*) avccontext->hwaccel_context);
        hwaccel_context->surface = ANativeWindow_fromSurface(hwaccel_context->env, hwaccel_context->surface_object);
    }
    
    return configure_and_start(context, 0);
    
}

static int decode_nal_units(MediaCodecDecContext *s, const uint8_t *buf, int length) {
    int ret = 0;

    /* split the input packet into NAL units, so we know the upper bound on the
     * number of slices in the frame */
    ret = ff_h2645_packet_split(&s->pkt, buf, length, s->avctx, s->is_nalff,
            s->nal_length_size, s->avctx->codec_id, 1);
    if (ret < 0) {
        av_log(s->avctx, AV_LOG_ERROR, "Error splitting the input into NAL units.\n");
    }

    return ret;
}

static int mediacodec_decode_frame(AVCodecContext *avccontext, void *data,
        int *got_frame_ptr, AVPacket *avpkt) {

    MediaCodecDecContext *context = avccontext->priv_data;
    AMediaCodec* codec = context->codec;
    AVFrame* frame = data;

    ssize_t index, indexOutput;
    uint8_t *inputBuffer;

    size_t bufferSize;
    media_status_t status;
    AMediaCodecBufferInfo info;
    
    if (avccontext->width <= 0 || avccontext->height <= 0) {
        AVCodecParserContext* parser_context;
        uint8_t* out;
        int out_size;
        
        if (!avpkt->data || !avpkt->size) {
            return 0;
        }
        
        parser_context = av_parser_init(avccontext->codec_id);
        parser_context->flags |= PARSER_FLAG_COMPLETE_FRAMES;
        
        av_parser_parse2(parser_context, avccontext, &out, &out_size, avpkt->data, avpkt->size, avpkt->pts, avpkt->dts, avpkt->pos);
        av_parser_close(parser_context);
        
        status = configure_and_start(context, 1);    
        
        if (status != AMEDIA_OK) {
            return status;
        }
    }
    
    index = AMediaCodec_dequeueInputBuffer(codec, -1);
    if (index < 0) {
        return 0;
    }
    inputBuffer = AMediaCodec_getInputBuffer(codec, index, &bufferSize);
    av_log(avccontext, AV_LOG_DEBUG, "input buffer acquired (index: %ld capacity: %ld nalLength: %d)\n", index, bufferSize, context->nal_length_size);

    if (avpkt->data) {

        if (avccontext->codec_id == AV_CODEC_ID_H264 || avccontext->codec_id == AV_CODEC_ID_HEVC) {
        int nalIndex, inputOffset = 0;

        decode_nal_units(context, avpkt->data, avpkt->size);

        if (context->nal_length_size == 4) {
            memcpy(inputBuffer, avpkt->data, avpkt->size);

            for (nalIndex = 0; nalIndex < context->pkt.nb_nals; nalIndex++) {
                memcpy(inputBuffer + inputOffset, startCode, sizeof (startCode));
                inputOffset += context->pkt.nals[nalIndex].raw_size + sizeof (startCode);

            }
        } else {
            for (nalIndex = 0; nalIndex < context->pkt.nb_nals; nalIndex++) {
                memcpy(inputBuffer + inputOffset, startCode, sizeof (startCode));
                memcpy(inputBuffer + inputOffset + sizeof (startCode), context->pkt.nals[nalIndex].data, context->pkt.nals[nalIndex].size);
                inputOffset += context->pkt.nals[nalIndex].size + sizeof (startCode);
            }
        }
        }else{
            memcpy(inputBuffer, avpkt->data, avpkt->size);
        }

        status = AMediaCodec_queueInputBuffer(codec, index, 0, avpkt->size, 1000000LL * avpkt->pts * avccontext->pkt_timebase.num / avccontext->pkt_timebase.den, 0);
        av_log(avccontext, AV_LOG_DEBUG, "input buffer queued (pts: %lld)\n", 1000000LL * avpkt->pts * avccontext->pkt_timebase.num / avccontext->pkt_timebase.den);
    } else {
        status = AMediaCodec_queueInputBuffer(codec, index, 0, 0, 0, AMEDIACODEC_BUFFER_FLAG_END_OF_STREAM);
        av_log(avccontext, AV_LOG_DEBUG, "end of stream reached");
    }

    if (status != AMEDIA_OK) {
        return status;
    }

    indexOutput = AMediaCodec_dequeueOutputBuffer(codec, &info, 200);
    if (indexOutput >= 0) {

        int outputColorFormat, width, height, ret, fill;
        AMediaFormat* outputFormat;
        uint8_t *outputBuffer;

        outputBuffer = AMediaCodec_getOutputBuffer(codec, indexOutput, &bufferSize);
        outputFormat = AMediaCodec_getOutputFormat(codec);
        av_log(avccontext, AV_LOG_DEBUG, "output buffer acquired (format: %s)\n", AMediaFormat_toString(outputFormat));

        AMediaFormat_getInt32(outputFormat, AMEDIAFORMAT_KEY_COLOR_FORMAT, &outputColorFormat);
        AMediaFormat_getInt32(outputFormat, AMEDIAFORMAT_KEY_WIDTH, &width);
        AMediaFormat_getInt32(outputFormat, AMEDIAFORMAT_KEY_HEIGHT, &height);

        frame->format = getPixelFormatFromColorFormat(outputColorFormat);
        frame->width = width;
        frame->height = height;
        if (!avccontext->hwaccel_context) {
            if ((ret = ff_get_buffer(avccontext, frame, 0)) < 0)
                return ret;
        }
        frame->pts = info.presentationTimeUs * avccontext->pkt_timebase.den / (avccontext->pkt_timebase.num * 1000000LL);

        av_log(avccontext, AV_LOG_DEBUG, "output frame will be filled (width: %d, height: %d, pixel format: %d, pts: %ld)\n", frame->width, frame->height, frame->format, frame->pts);

        if (!avccontext->hwaccel_context) {
            fill = av_image_fill_arrays(frame->data, frame->linesize, outputBuffer + info.offset, getPixelFormatFromColorFormat(outputColorFormat), width, height, 1);
            if (fill >= 0) {
                *got_frame_ptr = 1;
            } else {
                av_log(avccontext, AV_LOG_ERROR, av_err2str(fill));
                return fill;
            }
            status = AMediaCodec_releaseOutputBuffer(codec, indexOutput, false);
        } else {
            MediaCodecSurfaceRenderContext* render_context = mediacodec_alloc_surface_context(codec, indexOutput);
            if (render_context) {
                frame->buf[0] = av_buffer_alloc(1);
                frame->format = AV_PIX_FMT_MEDIACODEC;
                frame->data[3] = (void*) render_context;
                *got_frame_ptr = 1;
            }
        }
        if (status != AMEDIA_OK) {
            return status;
        }
    }

    return avpkt->size;
}

static void ff_hevc_ps_uninit(HEVCParamSets *ps)
{
    int i;

    for (i = 0; i < HEVC_MAX_VPS_COUNT; i++)
        av_buffer_unref(&ps->vps_list[i]);
    
    for (i = 0; i < HEVC_MAX_SPS_COUNT; i++)
        av_buffer_unref(&ps->sps_list[i]);

    for (i = 0; i < HEVC_MAX_PPS_COUNT; i++)
        av_buffer_unref(&ps->pps_list[i]);

    ps->vps = NULL;
    ps->pps = NULL;
    ps->sps = NULL;
}

static void ff_hevc_sei_uninit(HEVCSEIContext *ps)
{
    av_freep(&ps->a53_caption.a53_caption);
    av_freep(&ps->picture_hash.md5_ctx);
}

static int mediacodec_decode_close(AVCodecContext *avccontext) {
    media_status_t status;
    MediaCodecDecContext *context = avccontext->priv_data;
    status = AMediaCodec_flush(context->codec);
    if (status != AMEDIA_OK) {
        return status;
    }
    status = AMediaCodec_stop(context->codec);
    av_freep(&context->csd);
    ff_h264_ps_uninit(&context->h264_ps);
    ff_hevc_ps_uninit(&context->hevc_ps);
    ff_hevc_sei_uninit(&context->hevc_sei);
    ff_h2645_packet_uninit(&context->pkt);
    if (avccontext->hwaccel_context && ((MediaCodecSurfaceContext*) avccontext->hwaccel_context)->surface) {
        ANativeWindow_release(((MediaCodecSurfaceContext*) avccontext->hwaccel_context)->surface);
    }
    if (status != AMEDIA_OK) {
        return status;
    }

    av_log(avccontext, AV_LOG_DEBUG, "decoder closed");
    return AMediaCodec_delete(context->codec);
}

#define ATTR(x) .x
#define CODECNAME(name) mediacodec_ ## name
#define DECLARE_DECODER(cname, cid) \
AVCodec ff_mediacodec_##cname##_decoder = { \
    ATTR(name)             = AV_STRINGIFY(CODECNAME(cname)), \
    ATTR(long_name)        = NULL_IF_CONFIG_SMALL("Android MediaCodec decoder"), \
    ATTR(type)             = AVMEDIA_TYPE_VIDEO, \
    ATTR(id)               = cid, \
    ATTR(priv_data_size)   = sizeof (MediaCodecDecContext), \
    ATTR(init)             = mediacodec_decode_init, \
    ATTR(decode)           = mediacodec_decode_frame, \
    ATTR(close)            = mediacodec_decode_close, \
    ATTR(capabilities)     = AV_CODEC_CAP_DELAY, \
    ATTR(pix_fmts)         = supported_formats \
}; \
supported_codecs[current] = ff_mediacodec_##cname##_decoder; \
avcodec_register(&supported_codecs[current]); \
\
AVHWAccel ff_mediacodec_##cname##_hwaccel = { \
    ATTR(name)           = AV_STRINGIFY(CODECNAME(cname)), \
    ATTR(type)           = AVMEDIA_TYPE_VIDEO, \
    ATTR(id)             = cid, \
    ATTR(pix_fmt)        = AV_PIX_FMT_MEDIACODEC \
}; \
supported_hwaccel[current] = ff_mediacodec_##cname##_hwaccel; \
av_register_hwaccel(&supported_hwaccel[current]); \
current++; \
\

const enum AVPixelFormat supported_formats[] = {AV_PIX_FMT_YUV420P, AV_PIX_FMT_NV12, AV_PIX_FMT_MEDIACODEC};
struct AVCodec supported_codecs[6];
struct AVHWAccel supported_hwaccel[6];
int current = 0;

static void init_and_register_all_mediacodec() {
    DECLARE_DECODER(hevc, AV_CODEC_ID_HEVC)
    DECLARE_DECODER(h264, AV_CODEC_ID_H264)
    DECLARE_DECODER(mpeg2, AV_CODEC_ID_MPEG2VIDEO)
    DECLARE_DECODER(mpeg4, AV_CODEC_ID_MPEG4)
    DECLARE_DECODER(vp8, AV_CODEC_ID_VP8)
    DECLARE_DECODER(vp9, AV_CODEC_ID_VP9)
}

AVCodec ff_mediacodec_decoder = {
    .name = "mediacodec",
    .init_static_data = init_and_register_all_mediacodec
};

