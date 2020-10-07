/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BufferSource.h
 * Author: ayache
 *
 * Created on March 6, 2020, 1:34 PM
 */

#ifndef BUFFERSOURCE_H
#define BUFFERSOURCE_H

#include <com/xuggle/xuggler/IAudioSamples.h>
#include <com/xuggle/xuggler/IVideoPicture.h>
#include <com/xuggle/xuggler/IRational.h>
#include <com/xuggle/xuggler/IBufferSource.h>
#include <com/xuggle/xuggler/MediaFilter.h>

extern "C" {
#include "libavfilter/avfilter.h"
#include "libavutil/opt.h"
}



namespace com {
    namespace xuggle {
        namespace xuggler {

            class BufferSource : public IBufferSource, public MediaFilter {
                VS_JNIUTILS_REFCOUNTED_OBJECT_PRIVATE_MAKE(BufferSource)
            public:
                static BufferSource* make(AVFilterGraph* graph, IAudioSamples::Format format,
                    int channels,    
                    int sample_rate,
                    IRational* time_base,
                    IAudioSamples::ChannelLayout channel_layout);
                virtual int addAudioSamples(IAudioSamples* samples);

    
            protected:
                BufferSource();
                virtual ~BufferSource();
            private:
                AVFilterGraph* mFilterGraph;
                int mSampleRate;
            };

        }
    }
}

#endif /* BUFFERSOURCE_H */

