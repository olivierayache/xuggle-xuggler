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

#ifndef IBUFFERSOURCE_H
#define IBUFFERSOURCE_H

#include <com/xuggle/xuggler/IAudioSamples.h>
#include <com/xuggle/xuggler/IVideoPicture.h>
#include <com/xuggle/xuggler/IRational.h>
#include <com/xuggle/xuggler/IMediaFilter.h>

namespace com {
    namespace xuggle {
        namespace xuggler {

            class VS_API_XUGGLER IBufferSource : public virtual IMediaFilter {
            public:

                /**
                 * Adds audio samples to this filter
                 * 
                 * @param samples the audio samples to add
                 * @return 0 on success or <0 if an error occurs
                 * 
                 */
                virtual int addAudioSamples(IAudioSamples* samples) = 0;
                
                /**
                 * Adds picture to this filter
                 * 
                 * @param picture the picture to add
                 * @return 0 on success or <0 if an error occurs
                 * 
                 */
                virtual int addVideoPicture(IVideoPicture* picture) = 0;

            protected:
                virtual ~IBufferSource() = default;
            };

        }
    }
}

#endif /* BUFFERSOURCE_H */

