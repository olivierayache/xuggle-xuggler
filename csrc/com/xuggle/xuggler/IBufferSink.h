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

#ifndef IBUFFERSINK_H
#define IBUFFERSINK_H

#include <com/xuggle/xuggler/IAudioSamples.h>
#include <com/xuggle/xuggler/IMediaFilter.h>


namespace com {
    namespace xuggle {
        namespace xuggler {

            class VS_API_XUGGLER IBufferSink : public virtual IMediaFilter {
            public:

                /**
                 * Returns the sample rate of output filtered samples.
                 * 
                 * @return the sample rate
                 */
                virtual int getSampleRate() = 0;
                
                /**
                 * Returns the channel number of output filtered samples.
                 * 
                 * @return the channel number
                 */
                virtual int getChannels() = 0;
                

                /**
                 * Returns the width of output filtered picture.
                 * 
                 * @return the width
                 */
                virtual int getWidth() = 0;

                /**
                 * Returns the height of output filtered picture.
                 * 
                 * @return the height
                 */
                virtual int getHeight() = 0;
                
                /**
                 * Returns the frame rate output filtered picture.
                 * 
                 * @return the frame rate
                 */
                virtual IRational* getFrameRate() = 0;
                
                /**
                 * Returns the frame rate output filtered picture.
                 * 
                 * @return the frame rate
                 */
                virtual IRational* getTimeBase() = 0;
                
                /**
                 * Sets the number of samples for each output filtered samples.
                 * The last buffer will be padded with 0.
                 * 
                 * @param frameSize the number of samples of output
                 */
                virtual void setNumSamples(int frameSize) = 0;
                
                /**
                 * Fills this audio samples with filtered data 
                 * 
                 * @param samples the audio samples filled with filtered data
                 * 
                 * @return 0 on success or <0 if an error occurs
                 */
                virtual int fillAudioSamples(IAudioSamples* samples) = 0;
                
                /**
                 * Fills this video picture with filtered data 
                 * 
                 * @param samples the video picture filled with filtered data
                 * 
                 * @return 0 on success or <0 if an error occurs
                 */
                virtual int fillVideoPicture(IVideoPicture* picture) = 0; 
                
            protected:
                virtual ~IBufferSink() = default;
            };

        }
    }
}

#endif /* BUFFERSINK_H */

