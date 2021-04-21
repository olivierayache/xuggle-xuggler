/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   BufferSink.h
 * Author: ayache
 *
 * Created on March 9, 2020, 1:25 PM
 */

#ifndef BUFFERSINK_H
#define BUFFERSINK_H

#include <com/xuggle/xuggler/IAudioSamples.h>
#include <com/xuggle/xuggler/IVideoPicture.h>
#include <com/xuggle/xuggler/IRational.h>
#include <com/xuggle/xuggler/IBufferSink.h>
#include <com/xuggle/xuggler/MediaFilter.h>
#include <com/xuggle/xuggler/MediaFilter.h>
#include <com/xuggle/ferry/RefPointer.h>

extern "C" {
#include "libavfilter/avfilter.h"
#include "libavfilter/buffersink.h"
#include "libavutil/opt.h"
}

namespace com {
    namespace xuggle {
        namespace xuggler {
            
            class BufferSink : public IBufferSink, public MediaFilter {
                VS_JNIUTILS_REFCOUNTED_OBJECT(BufferSink)
            public:
                static BufferSink* make(AVFilterGraph*, IAudioSamples::ChannelLayout channel_layout);
                static BufferSink* make(AVFilterGraph*, IPixelFormat::Type pixel_type);
                virtual int getSampleRate();
                virtual int getChannels();
                virtual int getWidth();
                virtual int getHeight();
                virtual IRational* getFrameRate();
                virtual IRational* getTimeBase();
                virtual void setNumSamples(int frameSize);
                virtual int fillAudioSamples(IAudioSamples* samples); 
                virtual int fillVideoPicture(IVideoPicture* picture);
                virtual int setReady();

            protected:
                BufferSink();
                virtual ~BufferSink();
            private:
                AVFilterGraph* mFilterGraph;
                int mSampleRate;
                AVFilter* mSinkFilter;
                AVFilterContext* mSinkFilterContext;
                com::xuggle::ferry::RefPointer<IRational> mTimeBase;
            };

        }
    }
}

#endif /* BUFFERSINK_H */

