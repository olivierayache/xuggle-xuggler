/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BufferSink.cpp
 * Author: ayache
 * 
 * Created on March 9, 2020, 1:25 PM
 */

#include <com/xuggle/xuggler/BufferSink.h>
#include <com/xuggle/xuggler/AudioSamples.h>
#include <com/xuggle/ferry/Logger.h>
#include <stdbool.h>

extern "C" {
#include "libavutil/channel_layout.h"
#include "VideoPicture.h"
#include "Rational.h"
}

VS_LOG_SETUP(VS_CPP_PACKAGE);



namespace com {
    namespace xuggle {
        namespace xuggler {

            using namespace com::xuggle::ferry;

            int BufferSink::getChannels() {
                return av_buffersink_get_channels(mSinkFilterContext);
            }

            int BufferSink::getSampleRate() {
                return av_buffersink_get_sample_rate(mSinkFilterContext);
            }

            int BufferSink::getWidth() {
                return av_buffersink_get_w(mSinkFilterContext);
            }

            int BufferSink::getHeight() {
                return av_buffersink_get_h(mSinkFilterContext);
            }

            IRational* BufferSink::getFrameRate() {
                AVRational frameRate = av_buffersink_get_frame_rate(mSinkFilterContext);
                return IRational::make(frameRate.num, frameRate.den);
            }

            IRational* BufferSink::getTimeBase() {
                AVRational frameRate = av_buffersink_get_time_base(mSinkFilterContext);
                return IRational::make(frameRate.num, frameRate.den);
            }

            void BufferSink::setNumSamples(int frameSize) {
                av_buffersink_set_frame_size(mSinkFilterContext, frameSize);
            }

            int BufferSink::setReady() {
                mTimeBase.reset(getTimeBase(), true);
                return MediaFilter::setReady();
            }

            int BufferSink::fillAudioSamples(IAudioSamples* samples) {
                int retval = -1;
                if (!ready) {
                    return -1;
                }
                if (samples) {
                    AudioSamples* outSamples = static_cast<AudioSamples*> (samples);
                    if (outSamples) {
                        outSamples->setComplete(false, 0, outSamples->getSampleRate(), outSamples->getChannels(), outSamples->getChannelLayout(),
                                IAudioSamples::FMT_S16, 0);
                        AVFrame* frame = av_frame_alloc();
                        if (frame) {
                            retval = av_buffersink_get_frame(mSinkFilterContext, frame);
                            if (retval == AVERROR(EAGAIN)) {
                                //VS_LOG_WARN("No data available retry later");
                            } else if (retval >= 0) {
                                int buffer_size;
                                buffer_size = av_samples_get_buffer_size(NULL, frame->channels, frame->nb_samples, (AVSampleFormat) frame->format, 0);
                                outSamples->ensureCapacity(buffer_size);
                                memcpy(outSamples->getRawSamples(0), frame->extended_data[0], buffer_size);
                                IRational* timeBase = outSamples->getTimeBase();
                                outSamples->setComplete(true,
                                        frame->nb_samples,
                                        frame->sample_rate,
                                        frame->channels,
                                        (IAudioSamples::ChannelLayout)frame->channel_layout,
                                        IAudioSamples::FMT_S16,
                                        timeBase->rescale(frame->pts, mTimeBase.value()));
                                VS_REF_RELEASE(timeBase);
                            }
                            av_frame_free(&frame);
                        }
                    }
                }
                return retval;
            }

            int BufferSink::fillVideoPicture(IVideoPicture* picture) {
                int retval = -1;
                if (!ready) {
                    return -1;
                }
                if (picture) {
                    VideoPicture* outPicture = static_cast<VideoPicture*> (picture);

                    if (picture) {
                        outPicture->setComplete(false, IPixelFormat::NONE, outPicture->getWidth(), outPicture->getHeight(), 0);
                        AVFrame* frame = av_frame_alloc();
                        if (frame) {
                            retval = av_buffersink_get_frame(mSinkFilterContext, frame);
                            if (retval == AVERROR(EAGAIN)) {
                                //VS_LOG_WARN("No data available retry later");
                            } else if (retval >= 0) {
                                outPicture->copyAVFrame(frame, (IPixelFormat::Type)frame->format, frame->width, frame->height);
                                IRational* timeBase = outPicture->getTimeBase();
                                outPicture->setComplete(true, 
                                        (IPixelFormat::Type)frame->format, 
                                        frame->width, 
                                        frame->height, 
                                        timeBase->rescale(frame->pts, mTimeBase.value()));
                                VS_REF_RELEASE(timeBase);
                            }
                            av_frame_free(&frame);
                        }
                    }
                }
                return retval;
            }

            BufferSink::BufferSink() {
                mSinkFilterContext = NULL;
                mSinkFilter = NULL;
                mTimeBase = IRational::make(1, 1000000);
            }

            BufferSink::~BufferSink() {
                //mSinkFilterContext freed by FilterChain
                mSinkFilterContext = NULL;
                mSinkFilter = NULL;
                mTimeBase = NULL;
            }

            BufferSink* BufferSink::make(AVFilterGraph* graph, IAudioSamples::ChannelLayout channel_layout) {

                BufferSink* retval = NULL;

                retval = BufferSink::make();

                if (retval) {
                    retval->mFilter = avfilter_get_by_name("aformat");
                    retval->mSinkFilter = avfilter_get_by_name("abuffersink");
                    retval->mFilterContext = avfilter_graph_alloc_filter(graph, retval->mFilter, "aformat");
                    if (!retval->mFilterContext) {
                        VS_REF_RELEASE(retval);
                    } else {
                        av_opt_set(retval->mFilterContext, "sample_fmts", "s16", AV_OPT_SEARCH_CHILDREN);
                        if (channel_layout != IAudioSamples::ChannelLayout::CH_NONE) {
                            char ch_layout[64];
                            av_get_channel_layout_string(ch_layout,
                                    sizeof (ch_layout),
                                    0,
                                    (uint64_t) channel_layout);
                            av_opt_set(retval->mFilterContext, "channel_layouts", ch_layout, AV_OPT_SEARCH_CHILDREN);
                        }
                        if (avfilter_init_str(retval->mFilterContext, NULL) < 0) {
                            VS_REF_RELEASE(retval);
                        } else {
                            retval->mSinkFilterContext = avfilter_graph_alloc_filter(graph, retval->mSinkFilter, "abuffersink");
                            if (!retval->mSinkFilterContext) {
                                VS_REF_RELEASE(retval);
                            } else {
                                av_opt_set_int(retval->mSinkFilterContext, "all_channel_counts", 1, AV_OPT_SEARCH_CHILDREN);
                                if (avfilter_init_str(retval->mSinkFilterContext, NULL) < 0) {
                                    VS_REF_RELEASE(retval);
                                } else {
                                    retval->mFilterGraph = graph;
                                    if (avfilter_link(retval->mFilterContext, 0, retval->mSinkFilterContext, 0) < 0) {
                                        VS_REF_RELEASE(retval);
                                    }
                                }

                            }
                        }

                    }

                }

                return retval;
            }

            BufferSink* BufferSink::make(AVFilterGraph* graph, IPixelFormat::Type pixel_type) {
                BufferSink* retval = NULL;

                retval = BufferSink::make();

                if (retval) {
                    retval->mFilter = avfilter_get_by_name("format");
                    retval->mSinkFilter = avfilter_get_by_name("buffersink");
                    retval->mFilterContext = avfilter_graph_alloc_filter(graph, retval->mFilter, "format");
                    if (!retval->mFilterContext) {
                        VS_REF_RELEASE(retval);
                    } else {
                        av_opt_set(retval->mFilterContext, "pix_fmts", pixel_type == IPixelFormat::NONE ? "yuv420p" : av_get_pix_fmt_name((AVPixelFormat) pixel_type), AV_OPT_SEARCH_CHILDREN);
                        if (avfilter_init_str(retval->mFilterContext, NULL) < 0) {
                            VS_REF_RELEASE(retval);
                        } else {
                            retval->mSinkFilterContext = avfilter_graph_alloc_filter(graph, retval->mSinkFilter, "buffersink");
                            if (!retval->mSinkFilterContext) {
                                VS_REF_RELEASE(retval);
                            } else {
                                if (avfilter_init_str(retval->mSinkFilterContext, NULL) < 0) {
                                    VS_REF_RELEASE(retval);
                                } else {
                                    retval->mFilterGraph = graph;
                                    if (avfilter_link(retval->mFilterContext, 0, retval->mSinkFilterContext, 0) < 0) {
                                        VS_REF_RELEASE(retval);
                                    }
                                }

                            }
                        }

                    }

                }

                return retval;
            }

        }
    }
}

