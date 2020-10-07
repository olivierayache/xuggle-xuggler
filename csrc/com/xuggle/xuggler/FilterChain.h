/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   FilterChain.h
 * Author: ayache
 *
 * Created on March 9, 2020, 5:42 PM
 */

#ifndef FILTERCHAIN_H
#define FILTERCHAIN_H

#include <com/xuggle/ferry/RefPointer.h>
#include <com/xuggle/xuggler/IBufferSource.h>
#include <com/xuggle/xuggler/IBufferSink.h>
#include <com/xuggle/xuggler/IFilterChain.h>
#include <list>

extern "C" {
#include <libavfilter/avfilter.h>
#include <libavutil/channel_layout.h>
}

namespace com {
    namespace xuggle {
        namespace xuggler {

            class FilterChain : public IFilterChain {
                VS_JNIUTILS_REFCOUNTED_OBJECT(FilterChain)
            public:

                virtual IMediaFilter* createFilter(const char* name);
                virtual IBufferSource* createSource(IAudioSamples::Format format,
                    int channels,
                    int sample_rate,
                    IRational* time_base,
                    IAudioSamples::ChannelLayout channel_layout);
                virtual IBufferSink* createSink(IAudioSamples::ChannelLayout channel_layout);
                virtual int configure();
                virtual int clear();
                
                FilterChain();
                virtual ~FilterChain();
            private:
                AVFilterGraph* mFilterGraph;
                std::list<
                        com::xuggle::ferry::RefPointer<IMediaFilter>
                > mFilters;
            };

        }
    }
}
#endif /* FILTERCHAIN_H */

