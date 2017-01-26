/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.11
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.xuggle.xuggler;
import com.xuggle.ferry.*;
public class IStreamCoder extends RefCounted implements com.xuggle.xuggler.IConfigurable {
  // JNIHelper.swg: Start generated code
  // >>>>>>>>>>>>>>>>>>>>>>>>>>>
  /**
   * This method is only here to use some references and remove
   * a Eclipse compiler warning.
   */
  @SuppressWarnings("unused")
  private void noop()
  {
    IBuffer.make(null, 1);
  }
   
  private volatile long swigCPtr;

  /**
   * Internal Only.
   */
  protected IStreamCoder(long cPtr, boolean cMemoryOwn) {
    super(XugglerJNI.IStreamCoder_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }
  
  /**
   * Internal Only.
   */
  protected IStreamCoder(long cPtr, boolean cMemoryOwn,
      java.util.concurrent.atomic.AtomicLong ref)
  {
    super(XugglerJNI.IStreamCoder_SWIGUpcast(cPtr),
     cMemoryOwn, ref);
    swigCPtr = cPtr;
  }
    
  /**
   * Internal Only.  Not part of public API.
   *
   * Get the raw value of the native object that obj is proxying for.
   *   
   * @param obj The java proxy object for a native object.
   * @return The raw pointer obj is proxying for.
   */
  public static long getCPtr(IStreamCoder obj) {
    if (obj == null) return 0;
    return obj.getMyCPtr();
  }

  /**
   * Internal Only.  Not part of public API.
   *
   * Get the raw value of the native object that we're proxying for.
   *   
   * @return The raw pointer we're proxying for.
   */  
  public long getMyCPtr() {
    if (swigCPtr == 0) throw new IllegalStateException("underlying native object already deleted");
    return swigCPtr;
  }
  
  /**
   * Create a new IStreamCoder object that is actually referring to the
   * exact same underlying native object.
   *
   * @return the new Java object.
   */
  @Override
  public IStreamCoder copyReference() {
    if (swigCPtr == 0)
      return null;
    else
      return new IStreamCoder(swigCPtr, swigCMemOwn, getJavaRefCount());
  }

  /**
   * Compares two values, returning true if the underlying objects in native code are the same object.
   *
   * That means you can have two different Java objects, but when you do a comparison, you'll find out
   * they are the EXACT same object.
   *
   * @return True if the underlying native object is the same.  False otherwise.
   */
  public boolean equals(Object obj) {
    boolean equal = false;
    if (obj instanceof IStreamCoder)
      equal = (((IStreamCoder)obj).swigCPtr == this.swigCPtr);
    return equal;
  }
  
  /**
   * Get a hashable value for this object.
   *
   * @return the hashable value.
   */
  public int hashCode() {
     return (int)swigCPtr;
  }
  
  // <<<<<<<<<<<<<<<<<<<<<<<<<<<
  // JNIHelper.swg: End generated code
  

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

  

  public IStreamCoder.Direction getDirection() {
    return IStreamCoder.Direction.swigToEnum(XugglerJNI.IStreamCoder_getDirection(swigCPtr, this));
  }

  public IStream getStream() {
    long cPtr = XugglerJNI.IStreamCoder_getStream(swigCPtr, this);
    return (cPtr == 0) ? null : new IStream(cPtr, false);
  }

  public ICodec getCodec() {
    long cPtr = XugglerJNI.IStreamCoder_getCodec(swigCPtr, this);
    return (cPtr == 0) ? null : new ICodec(cPtr, false);
  }

  public ICodec.Type getCodecType() {
    return ICodec.Type.swigToEnum(XugglerJNI.IStreamCoder_getCodecType(swigCPtr, this));
  }

  public ICodec.ID getCodecID() {
    return ICodec.ID.swigToEnum(XugglerJNI.IStreamCoder_getCodecID(swigCPtr, this));
  }

  public void setCodec(ICodec codec) {
    XugglerJNI.IStreamCoder_setCodec__SWIG_0(swigCPtr, this, ICodec.getCPtr(codec), codec);
  }

  public void setCodec(ICodec.ID id) {
    XugglerJNI.IStreamCoder_setCodec__SWIG_1(swigCPtr, this, id.swigValue());
  }

  public int getBitRate() {
    return XugglerJNI.IStreamCoder_getBitRate(swigCPtr, this);
  }

  public void setBitRate(int rate) {
    XugglerJNI.IStreamCoder_setBitRate(swigCPtr, this, rate);
  }

  public int getBitRateTolerance() {
    return XugglerJNI.IStreamCoder_getBitRateTolerance(swigCPtr, this);
  }

  public void setBitRateTolerance(int tolerance) {
    XugglerJNI.IStreamCoder_setBitRateTolerance(swigCPtr, this, tolerance);
  }

  public int getHeight() {
    return XugglerJNI.IStreamCoder_getHeight(swigCPtr, this);
  }

  public void setHeight(int height) {
    XugglerJNI.IStreamCoder_setHeight(swigCPtr, this, height);
  }

  public int getWidth() {
    return XugglerJNI.IStreamCoder_getWidth(swigCPtr, this);
  }

  public void setWidth(int width) {
    XugglerJNI.IStreamCoder_setWidth(swigCPtr, this, width);
  }

  public IRational getTimeBase() {
    long cPtr = XugglerJNI.IStreamCoder_getTimeBase(swigCPtr, this);
    return (cPtr == 0) ? null : new IRational(cPtr, false);
  }

  public void setTimeBase(IRational newTimeBase) {
    XugglerJNI.IStreamCoder_setTimeBase(swigCPtr, this, IRational.getCPtr(newTimeBase), newTimeBase);
  }

  public IRational getFrameRate() {
    long cPtr = XugglerJNI.IStreamCoder_getFrameRate(swigCPtr, this);
    return (cPtr == 0) ? null : new IRational(cPtr, false);
  }

  public void setFrameRate(IRational newFrameRate) {
    XugglerJNI.IStreamCoder_setFrameRate(swigCPtr, this, IRational.getCPtr(newFrameRate), newFrameRate);
  }

  public int getNumPicturesInGroupOfPictures() {
    return XugglerJNI.IStreamCoder_getNumPicturesInGroupOfPictures(swigCPtr, this);
  }

  public void setNumPicturesInGroupOfPictures(int gops) {
    XugglerJNI.IStreamCoder_setNumPicturesInGroupOfPictures(swigCPtr, this, gops);
  }

  public IPixelFormat.Type getPixelType() {
    return IPixelFormat.Type.swigToEnum(XugglerJNI.IStreamCoder_getPixelType(swigCPtr, this));
  }

  public void setPixelType(IPixelFormat.Type pixelFmt) {
    XugglerJNI.IStreamCoder_setPixelType(swigCPtr, this, pixelFmt.swigValue());
  }

  public int getSampleRate() {
    return XugglerJNI.IStreamCoder_getSampleRate(swigCPtr, this);
  }

  public void setSampleRate(int sampleRate) {
    XugglerJNI.IStreamCoder_setSampleRate(swigCPtr, this, sampleRate);
  }

  public IAudioSamples.Format getSampleFormat() {
    return IAudioSamples.Format.swigToEnum(XugglerJNI.IStreamCoder_getSampleFormat(swigCPtr, this));
  }

  public void setSampleFormat(IAudioSamples.Format aFormat) {
    XugglerJNI.IStreamCoder_setSampleFormat(swigCPtr, this, aFormat.swigValue());
  }

  public int getChannels() {
    return XugglerJNI.IStreamCoder_getChannels(swigCPtr, this);
  }

  public void setChannels(int channels) {
    XugglerJNI.IStreamCoder_setChannels(swigCPtr, this, channels);
  }

  public int getAudioFrameSize() {
    return XugglerJNI.IStreamCoder_getAudioFrameSize(swigCPtr, this);
  }

  public int getGlobalQuality() {
    return XugglerJNI.IStreamCoder_getGlobalQuality(swigCPtr, this);
  }

  public void setGlobalQuality(int newQuality) {
    XugglerJNI.IStreamCoder_setGlobalQuality(swigCPtr, this, newQuality);
  }

  public int getFlags() {
    return XugglerJNI.IStreamCoder_getFlags(swigCPtr, this);
  }

  public void setFlags(int newFlags) {
    XugglerJNI.IStreamCoder_setFlags(swigCPtr, this, newFlags);
  }

  public boolean getFlag(IStreamCoder.Flags flag) {
    return XugglerJNI.IStreamCoder_getFlag(swigCPtr, this, flag.swigValue());
  }

  public void setFlag(IStreamCoder.Flags flag, boolean value) {
    XugglerJNI.IStreamCoder_setFlag(swigCPtr, this, flag.swigValue(), value);
  }

  public long getNextPredictedPts() {
    return XugglerJNI.IStreamCoder_getNextPredictedPts(swigCPtr, this);
  }

  @Deprecated public int open() {
    return XugglerJNI.IStreamCoder_open__SWIG_0(swigCPtr, this);
  }

  public int close() {
    return XugglerJNI.IStreamCoder_close(swigCPtr, this);
  }

  public int decodeAudio(IAudioSamples pOutSamples, IPacket packet, int byteOffset) {
    return XugglerJNI.IStreamCoder_decodeAudio(swigCPtr, this, IAudioSamples.getCPtr(pOutSamples), pOutSamples, IPacket.getCPtr(packet), packet, byteOffset);
  }

  public int decodeVideo(IVideoPicture pOutFrame, IPacket packet, int byteOffset) {
    return XugglerJNI.IStreamCoder_decodeVideo(swigCPtr, this, IVideoPicture.getCPtr(pOutFrame), pOutFrame, IPacket.getCPtr(packet), packet, byteOffset);
  }

  public int encodeVideo(IPacket pOutPacket, IVideoPicture pFrame, int suggestedBufferSize) {
    return XugglerJNI.IStreamCoder_encodeVideo(swigCPtr, this, IPacket.getCPtr(pOutPacket), pOutPacket, IVideoPicture.getCPtr(pFrame), pFrame, suggestedBufferSize);
  }

  public int encodeAudio(IPacket pOutPacket, IAudioSamples pSamples, long sampleToStartFrom) {
    return XugglerJNI.IStreamCoder_encodeAudio(swigCPtr, this, IPacket.getCPtr(pOutPacket), pOutPacket, IAudioSamples.getCPtr(pSamples), pSamples, sampleToStartFrom);
  }

  @Deprecated public static IStreamCoder make(IStreamCoder.Direction direction) {
    long cPtr = XugglerJNI.IStreamCoder_make__SWIG_0(direction.swigValue());
    return (cPtr == 0) ? null : new IStreamCoder(cPtr, false);
  }

  public int getCodecTag() {
    return XugglerJNI.IStreamCoder_getCodecTag(swigCPtr, this);
  }

  public void setCodecTag(int fourcc) {
    XugglerJNI.IStreamCoder_setCodecTag(swigCPtr, this, fourcc);
  }

  public int getNumProperties() {
    return XugglerJNI.IStreamCoder_getNumProperties(swigCPtr, this);
  }

  public IProperty getPropertyMetaData(int propertyNo) {
    long cPtr = XugglerJNI.IStreamCoder_getPropertyMetaData__SWIG_0(swigCPtr, this, propertyNo);
    return (cPtr == 0) ? null : new IProperty(cPtr, false);
  }

  public IProperty getPropertyMetaData(String name) {
    long cPtr = XugglerJNI.IStreamCoder_getPropertyMetaData__SWIG_1(swigCPtr, this, name);
    return (cPtr == 0) ? null : new IProperty(cPtr, false);
  }

  public int setProperty(String name, String value) {
    return XugglerJNI.IStreamCoder_setProperty__SWIG_0(swigCPtr, this, name, value);
  }

  public int setProperty(String name, double value) {
    return XugglerJNI.IStreamCoder_setProperty__SWIG_1(swigCPtr, this, name, value);
  }

  public int setProperty(String name, long value) {
    return XugglerJNI.IStreamCoder_setProperty__SWIG_2(swigCPtr, this, name, value);
  }

  public int setProperty(String name, boolean value) {
    return XugglerJNI.IStreamCoder_setProperty__SWIG_3(swigCPtr, this, name, value);
  }

  public int setProperty(String name, IRational value) {
    return XugglerJNI.IStreamCoder_setProperty__SWIG_4(swigCPtr, this, name, IRational.getCPtr(value), value);
  }

  public String getPropertyAsString(String name) {
    return XugglerJNI.IStreamCoder_getPropertyAsString(swigCPtr, this, name);
  }

  public double getPropertyAsDouble(String name) {
    return XugglerJNI.IStreamCoder_getPropertyAsDouble(swigCPtr, this, name);
  }

  public long getPropertyAsLong(String name) {
    return XugglerJNI.IStreamCoder_getPropertyAsLong(swigCPtr, this, name);
  }

  public IRational getPropertyAsRational(String name) {
    long cPtr = XugglerJNI.IStreamCoder_getPropertyAsRational(swigCPtr, this, name);
    return (cPtr == 0) ? null : new IRational(cPtr, false);
  }

  public boolean getPropertyAsBoolean(String name) {
    return XugglerJNI.IStreamCoder_getPropertyAsBoolean(swigCPtr, this, name);
  }

  public boolean isOpen() {
    return XugglerJNI.IStreamCoder_isOpen(swigCPtr, this);
  }

  public int getDefaultAudioFrameSize() {
    return XugglerJNI.IStreamCoder_getDefaultAudioFrameSize(swigCPtr, this);
  }

  public void setDefaultAudioFrameSize(int aNewSize) {
    XugglerJNI.IStreamCoder_setDefaultAudioFrameSize(swigCPtr, this, aNewSize);
  }

  public static IStreamCoder make(IStreamCoder.Direction direction, IStreamCoder copyCoder) {
    long cPtr = XugglerJNI.IStreamCoder_make__SWIG_1(direction.swigValue(), IStreamCoder.getCPtr(copyCoder), copyCoder);
    return (cPtr == 0) ? null : new IStreamCoder(cPtr, false);
  }

  public long getNumDroppedFrames() {
    return XugglerJNI.IStreamCoder_getNumDroppedFrames(swigCPtr, this);
  }

  public void setAutomaticallyStampPacketsForStream(boolean value) {
    XugglerJNI.IStreamCoder_setAutomaticallyStampPacketsForStream(swigCPtr, this, value);
  }

  public boolean getAutomaticallyStampPacketsForStream() {
    return XugglerJNI.IStreamCoder_getAutomaticallyStampPacketsForStream(swigCPtr, this);
  }

  public void setCodecID(ICodec.ID id) {
    XugglerJNI.IStreamCoder_setCodecID(swigCPtr, this, id.swigValue());
  }

  public int setExtraData(IBuffer src, int offset, int length, boolean allocNew) {
    return XugglerJNI.IStreamCoder_setExtraData(swigCPtr, this, IBuffer.getCPtr(src), src, offset, length, allocNew);
  }

  public int getExtraData(IBuffer dest, int offset, int maxBytesToCopy) {
    return XugglerJNI.IStreamCoder_getExtraData(swigCPtr, this, IBuffer.getCPtr(dest), dest, offset, maxBytesToCopy);
  }

  public int getExtraDataSize() {
    return XugglerJNI.IStreamCoder_getExtraDataSize(swigCPtr, this);
  }

  public IStreamCoder.CodecStandardsCompliance getStandardsCompliance() {
    return IStreamCoder.CodecStandardsCompliance.swigToEnum(XugglerJNI.IStreamCoder_getStandardsCompliance(swigCPtr, this));
  }

  public int setStandardsCompliance(IStreamCoder.CodecStandardsCompliance compliance) {
    return XugglerJNI.IStreamCoder_setStandardsCompliance(swigCPtr, this, compliance.swigValue());
  }

  public int open(IMetaData options, IMetaData unsetOptions) {
    return XugglerJNI.IStreamCoder_open__SWIG_1(swigCPtr, this, IMetaData.getCPtr(options), options, IMetaData.getCPtr(unsetOptions), unsetOptions);
  }

  public static IStreamCoder make(IStreamCoder.Direction direction, ICodec codec) {
    long cPtr = XugglerJNI.IStreamCoder_make__SWIG_2(direction.swigValue(), ICodec.getCPtr(codec), codec);
    return (cPtr == 0) ? null : new IStreamCoder(cPtr, false);
  }

  public static IStreamCoder make(IStreamCoder.Direction direction, ICodec.ID id) {
    long cPtr = XugglerJNI.IStreamCoder_make__SWIG_3(direction.swigValue(), id.swigValue());
    return (cPtr == 0) ? null : new IStreamCoder(cPtr, false);
  }

  public int setProperty(IMetaData valuesToSet, IMetaData valuesNotFound) {
    return XugglerJNI.IStreamCoder_setProperty__SWIG_5(swigCPtr, this, IMetaData.getCPtr(valuesToSet), valuesToSet, IMetaData.getCPtr(valuesNotFound), valuesNotFound);
  }

  public enum Direction {
    ENCODING,
    DECODING;

    public final int swigValue() {
      return swigValue;
    }

    public static Direction swigToEnum(int swigValue) {
      Direction[] swigValues = Direction.class.getEnumConstants();
      if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
        return swigValues[swigValue];
      for (Direction swigEnum : swigValues)
        if (swigEnum.swigValue == swigValue)
          return swigEnum;
      throw new IllegalArgumentException("No enum " + Direction.class + " with value " + swigValue);
    }

    @SuppressWarnings("unused")
    private Direction() {
      this.swigValue = SwigNext.next++;
    }

    @SuppressWarnings("unused")
    private Direction(int swigValue) {
      this.swigValue = swigValue;
      SwigNext.next = swigValue+1;
    }

    @SuppressWarnings("unused")
    private Direction(Direction swigEnum) {
      this.swigValue = swigEnum.swigValue;
      SwigNext.next = this.swigValue+1;
    }

    private final int swigValue;

    private static class SwigNext {
      private static int next = 0;
    }
  }

  public enum Flags {
    FLAG_QSCALE(XugglerJNI.IStreamCoder_FLAG_QSCALE_get()),
    FLAG_4MV(XugglerJNI.IStreamCoder_FLAG_4MV_get()),
    FLAG_QPEL(XugglerJNI.IStreamCoder_FLAG_QPEL_get()),
    FLAG_GMC(XugglerJNI.IStreamCoder_FLAG_GMC_get()),
    FLAG_MV0(XugglerJNI.IStreamCoder_FLAG_MV0_get()),
    FLAG_INPUT_PRESERVED(XugglerJNI.IStreamCoder_FLAG_INPUT_PRESERVED_get()),
    FLAG_PASS1(XugglerJNI.IStreamCoder_FLAG_PASS1_get()),
    FLAG_PASS2(XugglerJNI.IStreamCoder_FLAG_PASS2_get()),
    FLAG_GRAY(XugglerJNI.IStreamCoder_FLAG_GRAY_get()),
    FLAG_EMU_EDGE(XugglerJNI.IStreamCoder_FLAG_EMU_EDGE_get()),
    FLAG_PSNR(XugglerJNI.IStreamCoder_FLAG_PSNR_get()),
    FLAG_TRUNCATED(XugglerJNI.IStreamCoder_FLAG_TRUNCATED_get()),
    FLAG_NORMALIZE_AQP(XugglerJNI.IStreamCoder_FLAG_NORMALIZE_AQP_get()),
    FLAG_INTERLACED_DCT(XugglerJNI.IStreamCoder_FLAG_INTERLACED_DCT_get()),
    FLAG_LOW_DELAY(XugglerJNI.IStreamCoder_FLAG_LOW_DELAY_get()),
    FLAG_GLOBAL_HEADER(XugglerJNI.IStreamCoder_FLAG_GLOBAL_HEADER_get()),
    FLAG_BITEXACT(XugglerJNI.IStreamCoder_FLAG_BITEXACT_get()),
    FLAG_AC_PRED(XugglerJNI.IStreamCoder_FLAG_AC_PRED_get()),
    FLAG_CBP_RD(XugglerJNI.IStreamCoder_FLAG_CBP_RD_get()),
    FLAG_QP_RD(XugglerJNI.IStreamCoder_FLAG_QP_RD_get()),
    FLAG_LOOP_FILTER(XugglerJNI.IStreamCoder_FLAG_LOOP_FILTER_get()),
    FLAG_INTERLACED_ME(XugglerJNI.IStreamCoder_FLAG_INTERLACED_ME_get()),
    FLAG_CLOSED_GOP(XugglerJNI.IStreamCoder_FLAG_CLOSED_GOP_get()),
    FLAG2_FAST(XugglerJNI.IStreamCoder_FLAG2_FAST_get()),
    FLAG2_STRICT_GOP(XugglerJNI.IStreamCoder_FLAG2_STRICT_GOP_get()),
    FLAG2_NO_OUTPUT(XugglerJNI.IStreamCoder_FLAG2_NO_OUTPUT_get()),
    FLAG2_LOCAL_HEADER(XugglerJNI.IStreamCoder_FLAG2_LOCAL_HEADER_get()),
    FLAG2_SKIP_RD(XugglerJNI.IStreamCoder_FLAG2_SKIP_RD_get()),
    FLAG2_CHUNKS(XugglerJNI.IStreamCoder_FLAG2_CHUNKS_get()),
    FLAG2_SHOW_ALL(XugglerJNI.IStreamCoder_FLAG2_SHOW_ALL_get());

    public final int swigValue() {
      return swigValue;
    }

    public static Flags swigToEnum(int swigValue) {
      Flags[] swigValues = Flags.class.getEnumConstants();
      if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
        return swigValues[swigValue];
      for (Flags swigEnum : swigValues)
        if (swigEnum.swigValue == swigValue)
          return swigEnum;
      throw new IllegalArgumentException("No enum " + Flags.class + " with value " + swigValue);
    }

    @SuppressWarnings("unused")
    private Flags() {
      this.swigValue = SwigNext.next++;
    }

    @SuppressWarnings("unused")
    private Flags(int swigValue) {
      this.swigValue = swigValue;
      SwigNext.next = swigValue+1;
    }

    @SuppressWarnings("unused")
    private Flags(Flags swigEnum) {
      this.swigValue = swigEnum.swigValue;
      SwigNext.next = this.swigValue+1;
    }

    private final int swigValue;

    private static class SwigNext {
      private static int next = 0;
    }
  }

  public enum CodecStandardsCompliance {
    COMPLIANCE_VERY_STRICT(XugglerJNI.IStreamCoder_COMPLIANCE_VERY_STRICT_get()),
    COMPLIANCE_STRICT(XugglerJNI.IStreamCoder_COMPLIANCE_STRICT_get()),
    COMPLIANCE_NORMAL(XugglerJNI.IStreamCoder_COMPLIANCE_NORMAL_get()),
    COMPLIANCE_UNOFFICIAL(XugglerJNI.IStreamCoder_COMPLIANCE_UNOFFICIAL_get()),
    COMPLIANCE_EXPERIMENTAL(XugglerJNI.IStreamCoder_COMPLIANCE_EXPERIMENTAL_get());

    public final int swigValue() {
      return swigValue;
    }

    public static CodecStandardsCompliance swigToEnum(int swigValue) {
      CodecStandardsCompliance[] swigValues = CodecStandardsCompliance.class.getEnumConstants();
      if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
        return swigValues[swigValue];
      for (CodecStandardsCompliance swigEnum : swigValues)
        if (swigEnum.swigValue == swigValue)
          return swigEnum;
      throw new IllegalArgumentException("No enum " + CodecStandardsCompliance.class + " with value " + swigValue);
    }

    @SuppressWarnings("unused")
    private CodecStandardsCompliance() {
      this.swigValue = SwigNext.next++;
    }

    @SuppressWarnings("unused")
    private CodecStandardsCompliance(int swigValue) {
      this.swigValue = swigValue;
      SwigNext.next = swigValue+1;
    }

    @SuppressWarnings("unused")
    private CodecStandardsCompliance(CodecStandardsCompliance swigEnum) {
      this.swigValue = swigEnum.swigValue;
      SwigNext.next = this.swigValue+1;
    }

    private final int swigValue;

    private static class SwigNext {
      private static int next = 0;
    }
  }

}
