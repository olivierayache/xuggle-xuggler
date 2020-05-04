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

#include "BufferSink.h"
#include "FilterChain.h"
#include "BufferSource.h"
#include "MediaFilter.h"

VS_LOG_SETUP(VS_CPP_PACKAGE);

namespace com {
    namespace xuggle {
        namespace xuggler {

            IMediaFilter* FilterChain::createFilter(const char* name) {
                return MediaFilter::make(mFilterGraph, name);
            }

            IBufferSink* FilterChain::createSink() {
                return BufferSink::make(mFilterGraph);
            }

            IBufferSource* FilterChain::createSource(IAudioSamples::Format format,
                    int channels,
                    int sample_rate,
                    IRational* time_base) {
                return BufferSource::make(mFilterGraph, format, channels, sample_rate, time_base);
            }

            int FilterChain::configure() {
                int retval = avfilter_graph_config(mFilterGraph, NULL);
                if (retval >= 0){
                    char* dump = avfilter_graph_dump(mFilterGraph, NULL);
                    if (dump) 
                    VS_LOG_WARN("Filter Graph configured %s", dump);
                }

                return retval;
            }

            FilterChain::FilterChain() {
                mFilterGraph = avfilter_graph_alloc();
                mFilterGraph->nb_threads = 10;
            }

            FilterChain::~FilterChain() {
                avfilter_graph_free(&mFilterGraph);
            }

        }
    }
}

