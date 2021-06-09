/*******************************************************************************
 * Copyright (c) 2008, 2010 Xuggle Inc.  All rights reserved.
 *  
 * This file is part of Xuggle-Xuggler-Main.
 *
 * Xuggle-Xuggler-Main is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Xuggle-Xuggler-Main is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Xuggle-Xuggler-Main.  If not, see <http://www.gnu.org/licenses/>.
 *******************************************************************************/

#ifdef SWIGJAVA

%typemap (javacode) com::xuggle::xuggler::IAudioSamples,com::xuggle::xuggler::IAudioSamples*,com::xuggle::xuggler::IAudioSamples& %{

  /**
   * info about this packet
   * @return information about this packet
   */
   
  @Override
  public String toString()
  {
    StringBuilder result = new StringBuilder();
    
    result.append(this.getClass().getName()+"@"+hashCode()+"[");
    result.append("sample rate:"+getSampleRate()+";");
    result.append("channels:"+getChannels()+";");
    result.append("format:"+getFormat()+";");
    result.append("time stamp:"+getTimeStamp()+";");
    result.append("complete:"+isComplete()+";");
    result.append("num samples:"+getNumSamples()+";");
    result.append("size:"+getSize()+";");
    result.append("key:"+isKey()+";");
    IRational timeBase = IRational.make(1,(int)Global.DEFAULT_PTS_PER_SECOND);
    result.append("time base:"+timeBase+";");
    if (timeBase != null) timeBase.delete();
    result.append("]");
    return result.toString();
  }

%}

#elif SWIGKOTLIN

%extend com::xuggle::xuggler::IAudioSamples {

    /*
     * Convenience method that returns the native buffer
     * managed by getData() above.
     *
     * @param startingSample The sample to start the array at.
     *   That means that only getNumSamples()-startingSample
     *   samples are available in this AudioSamples collection.
     */
    virtual short *getRawSamples(uint32_t startingSample) {
        return $self->getRawSamples(startingSample);
    }
}

%typemap(jstype)  short* "CPointer<ShortVar>?";
%typemap(jni)  short* "short*";
%typemap(jtype)  short* "short*";
%typemap(javaout) short* {
    return $jnicall;
  }
%typemap(out) short*
%{ $result = $1; %}


%typemap (javacode) com::xuggle::xuggler::IAudioSamples,com::xuggle::xuggler::IAudioSamples*,com::xuggle::xuggler::IAudioSamples& %{

%}

#endif

%include <com/xuggle/xuggler/IAudioSamples.h>
