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


extern "C" {
#include "libavcodec/avcodec.h"
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersrc.h"
#include "libavutil/opt.h"
#include "Global.h"
}

namespace com {
    namespace xuggle {
        namespace xuggler {

            int BufferSource::addAudioSamples(IAudioSamples* samples) {
                int retval = -1;
                if (samples) {
                    AudioSamples* inSamples = dynamic_cast<AudioSamples*> (samples);
                    AVFrame* frame = av_frame_alloc();
                    if (frame) {
                        frame->nb_samples = inSamples->getNumSamples();
                        frame->sample_rate = inSamples->getSampleRate();
                        frame->format = inSamples->getFormat();
                        frame->channels = inSamples->getChannels();
                        frame->channel_layout = av_get_default_channel_layout(frame->channels);
                        frame->pts = inSamples->getPts();
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
                    retval = av_buffersrc_write_frame(mFilterContext, NULL);
                }


                return retval;
            }

            BufferSource::BufferSource() {
                mFilter = avfilter_get_by_name("abuffer");
            }

            BufferSource::~BufferSource() {

            }

            BufferSource* BufferSource::make(AVFilterGraph* graph, IAudioSamples::Format format,
                    int channels,
                    int sample_rate,
                    IRational* time_base) {

                BufferSource* retval = NULL;


                if (sample_rate > 0 && time_base) {
                    retval = BufferSource::make();

                    if (retval) {
                        retval->mFilterContext = avfilter_graph_alloc_filter(graph, retval->mFilter, "abuffer");
                        if (!retval->mFilterContext) {
                            VS_REF_RELEASE(retval);
                        } else {
                            char ch_layout[64];
                            av_get_channel_layout_string(ch_layout,
                                    sizeof (ch_layout),
                                    0,
                                    av_get_default_channel_layout(channels));
                            av_opt_set_q(retval->mFilterContext, "time_base", (AVRational) {
                                time_base->getNumerator(), time_base->getDenominator()
                            }, AV_OPT_SEARCH_CHILDREN);
                            av_opt_set_int(retval->mFilterContext, "sample_rate", sample_rate, AV_OPT_SEARCH_CHILDREN);
                            av_opt_set(retval->mFilterContext, "sample_fmt", av_get_sample_fmt_name((AVSampleFormat) format), AV_OPT_SEARCH_CHILDREN);
                            av_opt_set(retval->mFilterContext, "channel_layout", ch_layout, AV_OPT_SEARCH_CHILDREN);
                            if (avfilter_init_str(retval->mFilterContext, NULL) < 0) {
                                VS_REF_RELEASE(retval);
                            }
                            retval->mFilterGraph = graph;
                        }
                    }
                }

                return retval;
            }

        }
    }
}

