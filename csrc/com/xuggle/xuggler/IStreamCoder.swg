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
%typemap (javacode) com::xuggle::xuggler::IStreamCoder,com::xuggle::xuggler::IStreamCoder*,com::xuggle::xuggler::IStreamCoder& %{

  /**
   * Returns the fourcc tag, in order of least significant byte
   * to most significant byte.
   *
   * @return a 4 char array of the fourcc
   */
  
  public char[] getCodecTagArray()
  {
    char[] retval = new char[4];
    int fourcc = this.getCodecTag();
    
    retval[0] = (char)(fourcc & 0xFF);
    retval[1] = (char)((fourcc >> 8) & 0xFF);
    retval[2] = (char)((fourcc >> 16) & 0xFF);
    retval[3] = (char)((fourcc >> 24) & 0xFF);
    return retval;
  }
  /**
   * Set the fourcc tag.
   *
   * @param fourcc A four char array, in order of least significant byte
   *  to most significant byte.
   *
   * @throws IllegalArgumentException if the array passed in is not exactly 4 bytes.
   */
   
   public void setCodecTag(char[] fourcc)
   {
     if (fourcc == null || fourcc.length != 4)
      throw new IllegalArgumentException();
     int tag = 0;
     tag = (fourcc[3]<<24)+(fourcc[2]<<16)+(fourcc[1]<<8)+fourcc[0];
     this.setCodecTag(tag);
   }
   
   /**
    * Prints  details on this IStreamCoder
    *
    * @return key details for this IStreamCoder
    */
    
  @Override
  public String toString()
  {
    StringBuilder result = new StringBuilder();
    ICodec.Type type = getCodecType();
    
    result.append(this.getClass().getName()+"@"+hashCode()+"[");
    result.append("codec="+getCodec()+";");
    result.append("time base="+getTimeBase()+";");
    result.append("frame rate="+getFrameRate()+";");
    switch(type)
    {
      case CODEC_TYPE_VIDEO:
        result.append("pixel type="+getPixelType()+";");
        result.append("width="+getWidth()+";");
        result.append("height="+getHeight()+";");
        break;
      case CODEC_TYPE_AUDIO:
        result.append("sample rate="+getSampleRate()+";");
        result.append("channels="+getChannels()+";");
        break;
      default:
        break;
    }
    result.append("]");
    return result.toString();
  }
  
  /**
   * {@inheritDoc}
   */
  public java.util.Collection<String> getPropertyNames()
  {
    java.util.Collection<String> retval = new java.util.LinkedList<String>();
    int numProperties = this.getNumProperties();
    for(int i = 0; i < numProperties; i++)
    {
      IProperty property = this.getPropertyMetaData(i);
      String name = property.getName();
      retval.add(name);
    }
    return retval;
  }

  /**
   * Returns <strong>a read-only copy</strong> of the extra data in this stream coder as a new {@link IBuffer}.
   * @return the extra data, or null if none or error.
   * @since 3.3
   */
  public IBuffer getExtraData()
  {
    int size = getExtraDataSize();
    if (size <= 0)
      return null;
    
    IBuffer retval = IBuffer.make(this, size);
    if (retval == null)
      return null;
    if (getExtraData(retval, 0, size) != size) {
      retval.delete();
      retval = null;
    }
    return retval;
  }
 

%}

%extend com::xuggle::xuggler::IStreamCoder {

    /**
     * Try to change current decoder to an HW decoder compatible with
     * pixel format
     * 
     * @param type an HW accelerated pixel format
     * @return 0 on success; non-zero on failure
     */
    int32_t setHardwareDecoding(IPixelFormat::Type type){
        return $self->setHardwareDecoding(type);
    }

#ifdef Android

    /**
     * Try to change current decoder to an HW decoder compatible with
     * pixel format
     * 
     * @param type an HW accelerated pixel format
     * @return 0 on success; non-zero on failure
     */
    int32_t setHardwareDecoding(IPixelFormat::Type type, void* surface){
        return $self->setHardwareDecoding(type, surface);
    }
    
    void* getHardwareSurface(){
        return $self->getHardwareSurface();
    }

#endif

} 

%typemap(jtype)  void*, const void* &  "android.view.Surface";
%typemap(jstype) void*, const void* &  "android.view.Surface";
%typemap(jni) void*       "jobject"
%typemap(javaout) void* {
    return $jnicall;
  }
%typemap(javain) void*
    "$javainput"

#elif SWIGKOTLIN

%extend com::xuggle::xuggler::IStreamCoder {

    /**
     * Try to change current decoder to an HW decoder compatible with
     * pixel format
     * 
     * @param type an HW accelerated pixel format
     * @return 0 on success; non-zero on failure
     */
    int32_t setHardwareDecoding(IPixelFormat::Type type){
        return $self->setHardwareDecoding(type);
    }

//%proxycode %{
//fun setHardwareDecoding(type: IPixelFormat.Type):Int
//%}    

#ifdef Android

    /**
     * Try to change current decoder to an HW decoder compatible with
     * pixel format
     * 
     * @param type an HW accelerated pixel format
     * @return 0 on success; non-zero on failure
     */
    int32_t setHardwareDecoding(IPixelFormat::Type type, void* surface){
        return $self->setHardwareDecoding(type, surface);
    }
    
    void* getHardwareSurface(){
        return $self->getHardwareSurface();
    }

#endif

}

#ifdef Mac

%typemap (expectbody) com::xuggle::xuggler::IStreamCoder,com::xuggle::xuggler::IStreamCoder*,com::xuggle::xuggler::IStreamCoder& %{ 
  
  internal constructor(cPtr: Long, cMemoryOwn: Boolean)

  /**
   * Try to change current decoder to an HW decoder compatible with
   * pixel format
   * 
   * @param type an HW accelerated pixel format
   * @return 0 on success; non-zero on failure
   */
  fun setHardwareDecoding(type: IPixelFormat.Type):Int

  fun decodeVideo(pOutFrame: IVideoPicture, packet: IPacket, byteOffset: Int):Int
%}

%typemap (javabody_derived) com::xuggle::xuggler::IStreamCoder,com::xuggle::xuggler::IStreamCoder*,com::xuggle::xuggler::IStreamCoder& %{

  private var swigCPtr: Long = -1

  actual internal constructor(cPtr: Long, cMemoryOwn: Boolean): super($imclassname.$javaclazznameSWIGUpcast(cPtr), cMemoryOwn) {
    swigCPtr = cPtr
    freeze()
  }

  val mSurface: AtomicReference<platform.AVFoundation.AVSampleBufferDisplayLayer?> = AtomicReference(null)

  /**
   * Try to change current decoder to an HW decoder compatible with
   * pixel format
   * 
   * @param type an HW accelerated pixel format
   * @param surface a display layer
   * @return 0 on success; non-zero on failure
   */
  fun setHardwareDecoding(type: IPixelFormat.Type,  surface: platform.AVFoundation.AVSampleBufferDisplayLayer) {
    mSurface.compareAndSet(null, surface)
    setHardwareDecoding(type)
  }

  actual fun decodeVideo(pOutFrame: IVideoPicture, packet: IPacket, byteOffset: Int):Int {
    val ret = decodeVideo_internal(pOutFrame, packet, byteOffset)
    if (ret >= 0 && pOutFrame.isComplete()){
      pOutFrame.setDisplayLayer(mSurface.value)        
    }
    return ret;
  }

%}
%javamethodmodifiers com::xuggle::xuggler::IStreamCoder::decodeVideo "internal"
%rename(decodeVideo_internal) com::xuggle::xuggler::IStreamCoder::decodeVideo;
%typemap(jstype)  void*, const void* & "platform.AVFoundation.AVSampleBufferDisplayLayer";
%javamethodmodifiers com::xuggle::xuggler::IStreamCoder::getHardwareSurface() "internal"
#else

%typemap (expectbody) com::xuggle::xuggler::IStreamCoder,com::xuggle::xuggler::IStreamCoder*,com::xuggle::xuggler::IStreamCoder& %{ 
  
  internal constructor(cPtr: Long, cMemoryOwn: Boolean)

  /**
   * Try to change current decoder to an HW decoder compatible with
   * pixel format
   * 
   * @param type an HW accelerated pixel format
   * @return 0 on success; non-zero on failure
   */
  fun setHardwareDecoding(type: IPixelFormat.Type):Int

%}

#endif

#endif

#ifdef SWIGJAVA
%typemap(javainterfaces) com::xuggle::xuggler::IStreamCoder, com::xuggle::xuggler::IStreamCoder*, com::xuggle::xuggler::IStreamCoder& "com.xuggle.xuggler.IConfigurable";
%javamethodmodifiers com::xuggle::xuggler::IStreamCoder::open() "@Deprecated public"
%javamethodmodifiers com::xuggle::xuggler::IStreamCoder::make(Direction) "@Deprecated public"
#elif SWIGKOTLIN
%javamethodmodifiers com::xuggle::xuggler::IStreamCoder::open() "@Deprecated(\"\")"
%javamethodmodifiers com::xuggle::xuggler::IStreamCoder::make(Direction) "@Deprecated(\"\")"
#endif

%include <com/xuggle/xuggler/IStreamCoder.h>

