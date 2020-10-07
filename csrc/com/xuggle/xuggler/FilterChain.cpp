/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FilterChain.cpp
 * Author: ayache
 * 
 * Created on March 9, 2020, 5:42 PM
 */

#include <com/xuggle/ferry/Logger.h>
#include <iostream>

#include "BufferSink.h"
#include "FilterChain.h"
#include "BufferSource.h"
#include "MediaFilter.h"

VS_LOG_SETUP(VS_CPP_PACKAGE);

using namespace std;

namespace com {
    namespace xuggle {
        namespace xuggler {

            IMediaFilter* FilterChain::createFilter(const char* name) {
                MediaFilter* make = MediaFilter::make(mFilterGraph, name);
                if (make){
                    VS_REF_ACQUIRE(make);
                    mFilters.push_back(make);
                }
                return make;
            }

            IBufferSink* FilterChain::createSink(IAudioSamples::ChannelLayout channel_layout) {
                BufferSink* make = BufferSink::make(mFilterGraph, channel_layout);
                if (make){
                    VS_REF_ACQUIRE(make);
                    mFilters.push_back(make);
                }
                return make;
            }

            IBufferSource* FilterChain::createSource(IAudioSamples::Format format,
                    int channels,
                    int sample_rate,
                    IRational* time_base,
                    IAudioSamples::ChannelLayout channel_layout) {
                BufferSource* make = BufferSource::make(mFilterGraph, format, channels, sample_rate, time_base, channel_layout);
                if (make){
                    VS_REF_ACQUIRE(make);
                    mFilters.push_back(make);
                }
                return make;
            }

            int FilterChain::configure() {
                int retval = avfilter_graph_config(mFilterGraph, NULL);
                if (retval >= 0){
                    char* dump = avfilter_graph_dump(mFilterGraph, NULL);
                    if (dump) {
                        VS_LOG_WARN("Filter Graph configured %s", dump);
                        av_free(dump);
                    }
                }

                return retval;
            }
            
            int FilterChain::clear() {
                if (mFilterGraph) {
                    while (mFilterGraph->nb_filters){
                        avfilter_free(mFilterGraph->filters[0]);
                    }
                }
                mFilters.clear();
                return 0;
            }

            FilterChain::FilterChain() {
                mFilterGraph = avfilter_graph_alloc();
                mFilterGraph->nb_threads = 10;
            }

            FilterChain::~FilterChain() {
                avfilter_graph_free(&mFilterGraph);
                mFilters.clear();
                //TODO: add created IMediaFilter(s) to std::vector for Refcount to avoid GC of IMediaFilter
            }

        }
    }
}

