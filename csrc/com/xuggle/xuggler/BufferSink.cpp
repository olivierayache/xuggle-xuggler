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

extern "C" {
#include "libavutil/channel_layout.h"
}

VS_LOG_SETUP(VS_CPP_PACKAGE);

namespace com {
    namespace xuggle {
        namespace xuggler {

            int BufferSink::getChannels() {
                return av_buffersink_get_channels(mSinkFilterContext);
            }

            int BufferSink::getSampleRate() {
                return av_buffersink_get_sample_rate(mSinkFilterContext);
            }

            void BufferSink::setNumSamples(int frameSize) {
                av_buffersink_set_frame_size(mSinkFilterContext, frameSize);
            }

            int BufferSink::fillAudioSamples(IAudioSamples* samples) {
                int retval = -1;
                if (samples) {
                    AudioSamples* outSamples = dynamic_cast<AudioSamples*> (samples);
                    if (outSamples) {
                        outSamples->setComplete(false, 0, outSamples->getSampleRate(), outSamples->getChannels(), outSamples->getChannelLayout(),
                                IAudioSamples::FMT_S16, 0);
                        AVFrame* frame = av_frame_alloc();
                        if (frame) {
                            retval = av_buffersink_get_frame(mSinkFilterContext, frame);
                            if (retval == AVERROR(EAGAIN)) {
                                VS_LOG_WARN("No data available retry later");
                            } else if (retval >= 0) {
                                int buffer_size;
                                buffer_size = av_samples_get_buffer_size(NULL, frame->channels, frame->nb_samples, (AVSampleFormat) frame->format, 0);
                                outSamples->ensureCapacity(buffer_size);
                                memcpy(outSamples->getRawSamples(0), frame->extended_data[0], buffer_size);
                                outSamples->setComplete(true, frame->nb_samples, frame->sample_rate, frame->channels, (IAudioSamples::ChannelLayout)frame->channel_layout, IAudioSamples::FMT_S16, frame->pts);
                            }
                            av_frame_free(&frame);
                        }
                    }
                }
                return retval;
            }

            BufferSink::BufferSink() {
                mFilter = avfilter_get_by_name("aformat");
                mSinkFilter = avfilter_get_by_name("abuffersink");
                VS_LOG_WARN("Buffer Sink created");
            }

            BufferSink::~BufferSink() {
                //mSinkFilterContext freed by FilterChain
                mSinkFilterContext = NULL;
                mSinkFilter = NULL;
            }

            BufferSink* BufferSink::make(AVFilterGraph* graph, IAudioSamples::ChannelLayout channel_layout) {

                BufferSink* retval = NULL;

                retval = BufferSink::make();

                if (retval) {
                    retval->mFilterContext = avfilter_graph_alloc_filter(graph, retval->mFilter, "aformat");
                    if (!retval->mFilterContext) {
                        VS_REF_RELEASE(retval);
                    } else {
                        av_opt_set(retval->mFilterContext, "sample_fmts", "s16", AV_OPT_SEARCH_CHILDREN);
                        if (channel_layout != IAudioSamples::CH_NONE){
                            char ch_layout[64];
                            av_get_channel_layout_string(ch_layout,
                                    sizeof (ch_layout),
                                    0,
                                    channel_layout);
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


        }
    }
}

