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
%typemap (javacode) com::xuggle::xuggler::IVideoPicture,com::xuggle::xuggler::IVideoPicture*,com::xuggle::xuggler::IVideoPicture& %{

  /**
   * info about this packet
   * @return information about this packet
   */
   
  @Override
  public String toString()
  {
    StringBuilder result = new StringBuilder();
    
    result.append(this.getClass().getName()+"@"+hashCode()+"[");
    result.append("pixel type:"+getPixelType()+";");
    result.append("width:"+getWidth()+";");
    result.append("height:"+getHeight()+";");
    result.append("time stamp:"+getTimeStamp()+";");
    result.append("complete:"+isComplete()+";");
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

%extend com::xuggle::xuggler::IVideoPicture {

#ifdef Mac
    /**
     * Try to change current decoder to an HW decoder compatible with
     * pixel format
     * 
     * @param type an HW accelerated pixel format
     * @return 0 on success; non-zero on failure
     */
    CMSampleBufferRef getCMSampleBuffer(){
        return (CMSampleBufferRef)$self->getOpaqueData();
    }
#endif
}

#ifdef Mac

%typemap (expectbody) com::xuggle::xuggler::IVideoPicture,com::xuggle::xuggler::IVideoPicture*,com::xuggle::xuggler::IVideoPicture& %{

  internal constructor(cPtr: Long, cMemoryOwn: Boolean)

  public fun render(drop: Boolean, timestamp: Long)

%}
%typemap (javabody_derived) com::xuggle::xuggler::IVideoPicture,com::xuggle::xuggler::IVideoPicture*,com::xuggle::xuggler::IVideoPicture& %{

  private var swigCPtr: Long = -1

  actual internal constructor(cPtr: Long, cMemoryOwn: Boolean): super($imclassname.$javaclazznameSWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = cPtr
    freeze()
  }

  val mLayer: AtomicReference<platform.AVFoundation.AVSampleBufferDisplayLayer?> = AtomicReference(null)

  internal fun setDisplayLayer(layer: platform.AVFoundation.AVSampleBufferDisplayLayer?) {
    mLayer.compareAndSet(null, layer)
  }

  actual fun render(drop: Boolean, timestamp: Long) {
    render_internal(drop, timestamp)
    mLayer.value?.enqueueSampleBuffer(getCMSampleBuffer())
  }

%}

%javamethodmodifiers com::xuggle::xuggler::IVideoPicture::render(bool, int64_t) "internal"
%rename(render_internal) com::xuggle::xuggler::IVideoPicture::render(bool, int64_t);
%typemap(jstype)  CMSampleBufferRef, const CMSampleBufferRef & "platform.CoreMedia.CMSampleBufferRef";
%typemap(jni)  CMSampleBufferRef, const CMSampleBufferRef & "CMSampleBufferRef";
%typemap(jtype)  CMSampleBufferRef, const CMSampleBufferRef & "CMSampleBufferRef";
%typemap(javaout) CMSampleBufferRef {
    return $jnicall;
  }
%typemap(out) CMSampleBufferRef
%{ $result = $1; %}

#endif

#endif

%include <com/xuggle/xuggler/IVideoPicture.h>
