/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BufferSource.cpp
 * Author: ayache
 * 
 * Created on March 6, 2020, 1:34 PM
 */

#include <com/xuggle/xuggler/BufferSource.h>
#include <com/xuggle/xuggler/AudioSamples.h>
#include <com/xuggle/xuggler/VideoPicture.h>
#include <com/xuggle/ferry/Logger.h>

VS_LOG_SETUP(VS_CPP_PACKAGE);


extern "C" {
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersrc.h"
#include "libavutil/opt.h"
}

namespace com {
    namespace xuggle {
        namespace xuggler {

            int BufferSource::addAudioSamples(IAudioSamples* samples) {
                int retval = -1;
                if (!mFilterContext || !ready) {
                    VS_LOG_ERROR("Try to add samples to an unitialized abuffer");
                    return retval;
                }
                if (samples) {
                    AudioSamples* inSamples = static_cast<AudioSamples*> (samples);
                    AVFrame* frame = av_frame_alloc();
                    if (frame) {
                        frame->nb_samples = inSamples->getNumSamples();
                        frame->sample_rate = inSamples->getSampleRate();
                        frame->format = inSamples->getFormat();
                        frame->channels = inSamples->getChannels();
                        frame->channel_layout = (uint64_t) inSamples->getChannelLayout();
                        IRational* timeBase = inSamples->getTimeBase();
                        frame->pts = mTimeBase->rescale(inSamples->getPts(), timeBase);
                        VS_REF_RELEASE(timeBase);
                        int data_size = av_samples_get_buffer_size(&frame->linesize[0],
                                frame->channels,
                                frame->nb_samples,
                                (AVSampleFormat) frame->format,
                                0);
                        if (data_size >= 0) {
                            retval = avcodec_fill_audio_frame(frame,
                                    frame->channels,
                                    (AVSampleFormat) frame->format,
                                    (const uint8_t*) inSamples->getRawSamples(0),
                                    data_size,
                                    0);
                            retval = av_buffersrc_write_frame(mFilterContext, frame);
                        } else {
                            av_frame_free(&frame);
                        }
                        if (retval < 0) {
                            av_frame_free(&frame);
                        }
                        av_frame_free(&frame);
                    }
                } else {
                    retval = av_buffersrc_add_frame_flags(mFilterContext, NULL, AV_BUFFERSRC_FLAG_PUSH);
                }


                return retval;
            }

            int BufferSource::addVideoPicture(IVideoPicture* picture) {
                int retval = -1;
                if (!mFilterContext || !ready) {
                    VS_LOG_ERROR("Try to add picture to an unitialized buffer");
                    return retval;
                }
                if (picture) {
		    AVFrame* frame = av_frame_alloc();
		    if (frame) {
                    	VideoPicture* inPicture = static_cast<VideoPicture*> (picture);
                    	IRational* timeBase = inPicture->getTimeBase();
                    	int64_t pts = mTimeBase->rescale(inPicture->getPts(), timeBase);
                    	VS_REF_RELEASE(timeBase);
                    	av_frame_ref(frame, inPicture->getAVFrame());
			frame->pts = pts;
			retval = av_buffersrc_add_frame(mFilterContext, frame);
		    	av_frame_free(&frame);
		    }
                } else {
                    retval = av_buffersrc_add_frame_flags(mFilterContext, NULL, AV_BUFFERSRC_FLAG_PUSH);
                }

		return retval;
            }

            BufferSource::BufferSource() {
                mTimeBase = IRational::make(1, 1000000);
            }

            BufferSource::~BufferSource() {
                mTimeBase = NULL;
            }

            BufferSource* BufferSource::make(AVFilterGraph* graph, IAudioSamples::Format format,
                    int channels,
                    int sample_rate,
                    IRational* time_base,
                    IAudioSamples::ChannelLayout channel_layout) {

                BufferSource* retval = NULL;


                if (sample_rate > 0 && time_base) {
                    retval = BufferSource::make();

                    if (retval) {
                        retval->mFilter = avfilter_get_by_name("abuffer");
                        retval->mFilterContext = avfilter_graph_alloc_filter(graph, retval->mFilter, "abuffer");
                        if (!retval->mFilterContext) {
                            VS_REF_RELEASE(retval);
                        } else {
                            char ch_layout[64];
                            av_get_channel_layout_string(ch_layout,
                                    sizeof (ch_layout),
                                    0,
                                    channel_layout == IAudioSamples::ChannelLayout::CH_NONE ? av_get_default_channel_layout(channels) : (uint64_t) channel_layout);
                            av_opt_set_q(retval->mFilterContext, "time_base", (AVRational) {
                                time_base->getNumerator(), time_base->getDenominator()
                            }, AV_OPT_SEARCH_CHILDREN);
                            av_opt_set_int(retval->mFilterContext, "sample_rate", sample_rate, AV_OPT_SEARCH_CHILDREN);
                            av_opt_set(retval->mFilterContext, "sample_fmt", av_get_sample_fmt_name((AVSampleFormat) format), AV_OPT_SEARCH_CHILDREN);
                            av_opt_set(retval->mFilterContext, "channel_layout", ch_layout, AV_OPT_SEARCH_CHILDREN);
                            av_opt_set_int(retval->mFilterContext, "channels", channels, AV_OPT_SEARCH_CHILDREN);
                            if (avfilter_init_str(retval->mFilterContext, NULL) < 0) {
                                VS_REF_RELEASE(retval);
                            }
                            retval->mTimeBase.reset(time_base, true);
                            retval->mFilterGraph = graph;
                        }
                    }
                }

                return retval;
            }

            BufferSource* BufferSource::make(AVFilterGraph* graph, IPixelFormat::Type format,
                    int width,
                    int height,
                    IRational* frame_rate,
                    IRational* time_base) {

                BufferSource* retval = NULL;


                if (width > 0 && height > 0 && time_base && frame_rate) {
                    retval = BufferSource::make();

                    if (retval) {
                        retval->mFilter = avfilter_get_by_name("buffer");
                        retval->mFilterContext = avfilter_graph_alloc_filter(graph, retval->mFilter, "buffer");
                        if (!retval->mFilterContext) {
                            VS_REF_RELEASE(retval);
                        } else {
                            av_opt_set_q(retval->mFilterContext, "frame_rate", (AVRational) {
                                frame_rate->getNumerator(), frame_rate->getDenominator()
                            }, AV_OPT_SEARCH_CHILDREN);
                            av_opt_set_q(retval->mFilterContext, "time_base", (AVRational) {
                                time_base->getNumerator(), time_base->getDenominator()
                            }, AV_OPT_SEARCH_CHILDREN);
                            av_opt_set_q(retval->mFilterContext, "pixel_aspect", (AVRational) {
                                1, 1
                            }, AV_OPT_SEARCH_CHILDREN);
                            av_opt_set_int(retval->mFilterContext, "width", width, AV_OPT_SEARCH_CHILDREN);
                            av_opt_set_int(retval->mFilterContext, "height", height, AV_OPT_SEARCH_CHILDREN);
                            av_opt_set(retval->mFilterContext, "pix_fmt", av_get_pix_fmt_name((AVPixelFormat) format), AV_OPT_SEARCH_CHILDREN);
                            if (avfilter_init_str(retval->mFilterContext, NULL) < 0) {
                                VS_REF_RELEASE(retval);
                            }
                            retval->mTimeBase.reset(time_base, true);
                            retval->mFilterGraph = graph;
                        }
                    }
                }

                return retval;
            }


        }
    }
}

