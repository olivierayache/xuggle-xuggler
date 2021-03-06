/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.2
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.xuggle.xuggler;
import com.xuggle.ferry.*;
/**
 * Represents a stream of similar data (eg video) in a {IContainer}.<br>
 * <p><br>
 * Streams are really virtual concepts; {IContainer}s really just contain<br>
 * a bunch of {IPacket}s.  But each {IPacket} usually has a stream<br>
 * id associated with it, and all {IPacket}s with that stream id represent<br>
 * the same type of (usually time-based) data.  For example in many FLV<br>
 * video files, there is a stream with id "0" that contains all video data, and<br>
 * a stream with id "1" that contains all audio data.<br>
 * </p><p><br>
 * You use an {IStream} object to get properly configured {IStreamCoder}<br>
 * for decoding, and to tell {IStreamCoder}s how to encode {IPacket}s when<br>
 * decoding.<br>
 * </p>
 */
public class IStream extends RefCounted {
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
  protected IStream(long cPtr, boolean cMemoryOwn) {
    super(XugglerJNI.IStream_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }
  
  /**
   * Internal Only.
   */
  protected IStream(long cPtr, boolean cMemoryOwn,
      java.util.concurrent.atomic.AtomicLong ref)
  {
    super(XugglerJNI.IStream_SWIGUpcast(cPtr),
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
  public static long getCPtr(IStream obj) {
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
   * Create a new IStream object that is actually referring to the
   * exact same underlying native object.
   *
   * @return the new Java object.
   */
  @Override
  public IStream copyReference() {
    if (swigCPtr == 0)
      return null;
    else
      return new IStream(swigCPtr, swigCMemOwn, getJavaRefCount());
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
    if (obj instanceof IStream)
      equal = (((IStream)obj).swigCPtr == this.swigCPtr);
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
   * info about this stream
   * @return information about this stream
   */
   
  @Override
  public String toString()
  {
    StringBuilder result = new StringBuilder();
    
    result.append(this.getClass().getName()+"@"+hashCode()+"[");
    result.append("index:"+getIndex()+";");
    result.append("id:"+getId()+";");
    result.append("streamcoder:"+getStreamCoder()+";");
    result.append("framerate:"+getFrameRate()+";");
    result.append("timebase:"+getTimeBase()+";");
    result.append("direction:"+getDirection()+";");
    result.append("]");
    return result.toString();
  }

  /**
   * Get an ordered sequence of index entries in this {@link IStream}.
   * 
   * @return A list of entries.  Will always return a non-null
   *   list, but if there are no entries the list size will be zero.
   */
  public java.util.List<IIndexEntry> getIndexEntries()
  {
    final int numEntries = getNumIndexEntries();
    java.util.List<IIndexEntry> retval = new java.util.ArrayList<IIndexEntry>(Math.max(numEntries, 10));
    for(int i = 0; i < numEntries; i++) {
      final IIndexEntry entry = getIndexEntry(i);
      if (entry != null) {
       retval.add(entry); 
      }
    }
    
    return retval;
  }


  /**
   * Get the {Direction} this stream is pointing in.<br>
   * @return The direction of this stream.
   */
  public IStream.Direction getDirection() {
    return IStream.Direction.swigToEnum(XugglerJNI.IStream_getDirection(swigCPtr, this));
  }

  /**
   * Get the relative position this stream has in the hosting<br>
   * {IContainer} object.<br>
   * @return The Index within the Container of this stream.
   */
  public int getIndex() {
    return XugglerJNI.IStream_getIndex(swigCPtr, this);
  }

  /**
   * Return a container format specific id for this stream.<br>
   * @return The (container format specific) id of this stream.
   */
  public int getId() {
    return XugglerJNI.IStream_getId(swigCPtr, this);
  }

  /**
   * Get the StreamCoder than can manipulate this stream.<br>
   * If the stream is an INBOUND stream, then the StreamCoder can<br>
   * do a IStreamCoder::DECODE.  IF this stream is an OUTBOUND stream,<br>
   * then the StreamCoder can do all IStreamCoder::ENCODE methods.<br>
   * <br>
   * @return The StreamCoder assigned to this object.
   */
  public IStreamCoder getStreamCoder() {
    long cPtr = XugglerJNI.IStream_getStreamCoder(swigCPtr, this);
    return (cPtr == 0) ? null : new IStreamCoder(cPtr, false);
  }

  /**
   * Get the (sometimes estimated) frame rate of this container.<br>
   * For variable frame-rate containers (they do exist) this is just<br>
   * an approimation.  Better to use getTimeBase().<br>
   * <br>
   * For contant frame-rate containers, this will be 1 / ( getTimeBase() )<br>
   * <br>
   * @return The frame-rate of this container.
   */
  public IRational getFrameRate() {
    long cPtr = XugglerJNI.IStream_getFrameRate(swigCPtr, this);
    return (cPtr == 0) ? null : new IRational(cPtr, false);
  }

  /**
   * The time base in which all timestamps (e.g. Presentation Time Stamp (PTS)<br>
   * and Decompression Time Stamp (DTS)) are represented.  For example<br>
   * if the time base is 1/1000, then the difference between a PTS of 1 and<br>
   * a PTS of 2 is 1 millisecond.  If the timebase is 1/1, then the difference<br>
   * between a PTS of 1 and a PTS of 2 is 1 second.<br>
   * <br>
   * @return The time base of this stream.
   */
  public IRational getTimeBase() {
    long cPtr = XugglerJNI.IStream_getTimeBase(swigCPtr, this);
    return (cPtr == 0) ? null : new IRational(cPtr, false);
  }

  /**
   * Return the start time, in {#getTimeBase()} units, when this stream<br>
   * started.<br>
   * @return The start time.
   */
  public long getStartTime() {
    return XugglerJNI.IStream_getStartTime(swigCPtr, this);
  }

  /**
   * Return the duration, in {#getTimeBase()} units, of this stream,<br>
   * or {Global#NO_PTS} if unknown.<br>
   * @return The duration (in getTimeBase units) of this stream, if known.
   */
  public long getDuration() {
    return XugglerJNI.IStream_getDuration(swigCPtr, this);
  }

  /**
   * The current Decompression Time Stamp that will be used on this stream,<br>
   * in {#getTimeBase()} units.<br>
   * @return The current Decompression Time Stamp that will be used on this stream.
   */
  public long getCurrentDts() {
    return XugglerJNI.IStream_getCurrentDts(swigCPtr, this);
  }

  /**
   * Get the number of index entries in this stream.<br>
   * @return The number of index entries in this stream.<br>
   * @see #getIndexEntry(int)
   */
  public int getNumIndexEntries() {
    return XugglerJNI.IStream_getNumIndexEntries(swigCPtr, this);
  }

  /**
   * Returns the number of encoded frames if known.  Note that frames here means<br>
   * encoded frames, which can consist of many encoded audio samples, or<br>
   * an encoded video frame.<br>
   * <br>
   * @return The number of frames (encoded) in this stream.
   */
  public long getNumFrames() {
    return XugglerJNI.IStream_getNumFrames(swigCPtr, this);
  }

  /**
   * Gets the sample aspect ratio.<br>
   * <br>
   * @return The sample aspect ratio.
   */
  public IRational getSampleAspectRatio() {
    long cPtr = XugglerJNI.IStream_getSampleAspectRatio(swigCPtr, this);
    return (cPtr == 0) ? null : new IRational(cPtr, false);
  }

  /**
   * Sets the sample aspect ratio.<br>
   * <br>
   * @param newRatio The new ratio.
   */
  public void setSampleAspectRatio(IRational newRatio) {
    XugglerJNI.IStream_setSampleAspectRatio(swigCPtr, this, IRational.getCPtr(newRatio), newRatio);
  }

  /**
   * Get the 4-character language setting for this stream.<br>
   * <br>
   * This will return null if no setting.  When calling<br>
   * from C++, callers must ensure that the IStream outlives the<br>
   * value returned.
   */
  public String getLanguage() {
    return XugglerJNI.IStream_getLanguage(swigCPtr, this);
  }

  /**
   * Set the 4-character language setting for this stream.<br>
   * <br>
   * If a string longer than 4 characters is passed in, only the<br>
   * first 4 characters is copied.<br>
   * <br>
   * @param language The new language setting.  null is equivalent to the<br>
   *   empty string.  strings longer than 4 characters will be truncated<br>
   *   to first 4 characters.
   */
  public void setLanguage(String language) {
    XugglerJNI.IStream_setLanguage(swigCPtr, this, language);
  }

  /**
   * Get the underlying container for this stream, or null if Xuggler<br>
   * doesn't know.<br>
   * <br>
   * @return the container, or null if we don't know.
   */
  public IContainer getContainer() {
    long cPtr = XugglerJNI.IStream_getContainer(swigCPtr, this);
    return (cPtr == 0) ? null : new IContainer(cPtr, false);
  }

  /**
   * Sets the stream coder to use for this stream.<br>
   * <br>
   * This method will only cause a change if the IStreamCoder currently set on this<br>
   * IStream is not open.  Otherwise the call is ignore and an error is returned.<br>
   * <br>
   * @param newCoder The new stream coder to use.<br>
   * @return &gt;= 0 on success; &lt; 0 on error.
   */
  public int setStreamCoder(IStreamCoder newCoder) {
    return XugglerJNI.IStream_setStreamCoder__SWIG_0(swigCPtr, this, IStreamCoder.getCPtr(newCoder), newCoder);
  }

  /**
   * Get how the decoding codec should parse data from this stream.<br>
   * @return the parse type.<br>
   * @since 3.0
   */
  public IStream.ParseType getParseType() {
    return IStream.ParseType.swigToEnum(XugglerJNI.IStream_getParseType(swigCPtr, this));
  }

  /**
   * Set the parse type the decoding codec should use.  Set to<br>
   * {ParseType#PARSE_NONE} if you don't want any parsing<br>
   * to be done.<br>
   * <p><br>
   * Warning: do not set this flag unless you know what you're doing,<br>
   * and do not set after you've started decoding.<br>
   * </p><br>
   * <br>
   * @param type The type to set.<br>
   * @since 3.0
   */
  public void setParseType(IStream.ParseType type) {
    XugglerJNI.IStream_setParseType(swigCPtr, this, type.swigValue());
  }

  /**
   * Get the {IMetaData} for this object,<br>
   * or null if none.<br>
   * <p><br>
   * If the {IContainer} or {IStream} object<br>
   * that this {IMetaData} came from was opened<br>
   * for reading, then changes via {IMetaData#setValue(String, String)}<br>
   * will have no effect on the underlying media.<br>
   * </p><br>
   * <p><br>
   * If the {IContainer} or {IStream} object<br>
   * that this {IMetaData} came from was opened<br>
   * for writing, then changes via {IMetaData#setValue(String, String)}<br>
   * will have no effect after {IContainer#writeHeader()}<br>
   * is called.<br>
   * </p><br>
   * @return the {IMetaData}.<br>
   * @since 3.1
   */
  public IMetaData getMetaData() {
    long cPtr = XugglerJNI.IStream_getMetaData(swigCPtr, this);
    return (cPtr == 0) ? null : new IMetaData(cPtr, false);
  }

  /**
   * Set the {IMetaData} on this object, overriding<br>
   * any previous meta data.  You should call this<br>
   * method on writable containers and<br>
   * before you call {IContainer#writeHeader}, as<br>
   * it probably won't do anything after that.<br>
   * <br>
   * @see #getMetaData()<br>
   * @since 3.1
   */
  public void setMetaData(IMetaData data) {
    XugglerJNI.IStream_setMetaData(swigCPtr, this, IMetaData.getCPtr(data), data);
  }

  /**
   * Takes a packet destined for this stream, and stamps<br>
   * the stream index, and converts the time stamp to the<br>
   * correct units (adjusting for rounding errors between<br>
   * stream conversions).<br>
   * <br>
   * @param packet to stamp<br>
   * @return &gt;= 0 on success; &lt;0 on failure.<br>
   * @since 3.2
   */
  public int stampOutputPacket(IPacket packet) {
    return XugglerJNI.IStream_stampOutputPacket(swigCPtr, this, IPacket.getCPtr(packet), packet);
  }

  /**
   * Sets the stream coder to use for this stream.<br>
   * <br>
   * This method will only cause a change if the IStreamCoder currently set on this<br>
   * IStream is not open.  Otherwise the call is ignored and an error is returned.<br>
   * <br>
   * @param newCoder The new stream coder to use.<br>
   * @param assumeOnlyStream If true then this {IStream} will notify the {IStreamCoder} that it is the only stream and the {IStreamCoder} may use it to determine time stamps to output packets with.<br>
   *   If false then the {IStreamCoder}<br>
   *   does not support automatic stamping of packets with stream index IDs<br>
   *   and users must call {#stampOutputPacket(IPacket)} themselves.<br>
   * @return &gt;= 0 on success; &lt; 0 on error.<br>
   * @since 3.2
   */
  public int setStreamCoder(IStreamCoder newCoder, boolean assumeOnlyStream) {
    return XugglerJNI.IStream_setStreamCoder__SWIG_1(swigCPtr, this, IStreamCoder.getCPtr(newCoder), newCoder, assumeOnlyStream);
  }

  /**
   * Search for the given time stamp in the key-frame index for this {IStream}.<br>
   * <p><br>
   * Not all {IContainerFormat} implementations<br>
   * maintain key frame indexes, but if they have one,<br>
   * then this method searches in the {IStream} index<br>
   * to quickly find the byte-offset of the nearest key-frame to<br>
   * the given time stamp.<br>
   * </p><br>
   * @param wantedTimeStamp the time stamp wanted, in the stream's<br>
   *                        time base units.<br>
   * @param flags A bitmask of the <code>SEEK_FLAG_*</code> flags, or 0 to turn<br>
   *              all flags off.  If {IContainer#SEEK_FLAG_BACKWARDS} then the returned<br>
   *              index will correspond to the time stamp which is &lt;=<br>
   *              the requested one (not supported by all demuxers).<br>
   *              If {IContainer#SEEK_FLAG_BACKWARDS} is not set then it will be &gt;=.<br>
   *              if {IContainer#SEEK_FLAG_ANY} seek to any frame, only<br>
   *              keyframes otherwise (not supported by all demuxers).<br>
   * @return The {IIndexEntry} for the nearest appropriate timestamp<br>
   *   in the index, or null if it can't be found.<br>
   * @since 3.4
   */
  public IIndexEntry findTimeStampEntryInIndex(long wantedTimeStamp, int flags) {
    long cPtr = XugglerJNI.IStream_findTimeStampEntryInIndex(swigCPtr, this, wantedTimeStamp, flags);
    return (cPtr == 0) ? null : new IIndexEntry(cPtr, false);
  }

  /**
   * Search for the given time stamp in the key-frame index for this {IStream}.<br>
   * <p><br>
   * Not all {IContainerFormat} implementations<br>
   * maintain key frame indexes, but if they have one,<br>
   * then this method searches in the {IStream} index<br>
   * to quickly find the index entry position of the nearest key-frame to<br>
   * the given time stamp.<br>
   * </p><br>
   * @param wantedTimeStamp the time stamp wanted, in the stream's<br>
   *                        time base units.<br>
   * @param flags A bitmask of the <code>SEEK_FLAG_*</code> flags, or 0 to turn<br>
   *              all flags off.  If {IContainer#SEEK_FLAG_BACKWARDS} then the returned<br>
   *              index will correspond to the time stamp which is &lt;=<br>
   *              the requested one (not supported by all demuxers).<br>
   *              If {IContainer#SEEK_FLAG_BACKWARDS} is not set then it will be &gt;=.<br>
   *              if {IContainer#SEEK_FLAG_ANY} seek to any frame, only<br>
   *              keyframes otherwise (not supported by all demuxers).<br>
   * @return The position in this {IStream} index, or -1 if it cannot<br>
   *   be found or an index is not maintained.<br>
   * @see #getIndexEntry(int)<br>
   * @since 3.4
   */
  public int findTimeStampPositionInIndex(long wantedTimeStamp, int flags) {
    return XugglerJNI.IStream_findTimeStampPositionInIndex(swigCPtr, this, wantedTimeStamp, flags);
  }

  /**
   * Get the {IIndexEntry} at the given position in this<br>
   * {IStream} object's index.<br>
   * <p><br>
   * Not all {IContainerFormat} types maintain<br>
   * {IStream} indexes, but if they do,<br>
   * this method can return those entries.<br>
   * </p><br>
   * <p><br>
   * Do not modify the {IContainer} this stream<br>
   * is from between calls to this method and<br>
   * {#getNumIndexEntries()} as indexes may<br>
   * be compacted while processing.<br>
   * </p><br>
   * @param position The position in the index table.<br>
   * @since 3.4
   */
  public IIndexEntry getIndexEntry(int position) {
    long cPtr = XugglerJNI.IStream_getIndexEntry(swigCPtr, this, position);
    return (cPtr == 0) ? null : new IIndexEntry(cPtr, false);
  }

  /**
   * Adds an index entry into the stream's sorted index list.<br>
   * Updates the entry if the list<br>
   * already contains it.<br>
   * <br>
   * @param entry The entry to add.<br>
   * @return &gt;=0 on success; &lt;0 on error.<br>
   * @since 3.4
   */
  public int addIndexEntry(IIndexEntry entry) {
    return XugglerJNI.IStream_addIndexEntry(swigCPtr, this, IIndexEntry.getCPtr(entry), entry);
  }

  /**
   * Set the format-specific stream id.<br>
   * <br>
   * @param id The id to set.<br>
   * @see #getId()<br>
   * @since 5.0
   */
  public void setId(int id) {
    XugglerJNI.IStream_setId(swigCPtr, this, id);
  }

  /**
   * The direction this stream is going (based on the container).<br>
   * <br>
   * If the container Container is opened in Container::READ mode<br>
   * then this will be INBOUND.  If it's opened in Container::WRITE<br>
   * mode, then this will be OUTBOUND.
   */
  public enum Direction {
    INBOUND,
    OUTBOUND;

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

  /**
   * What types of parsing can we do on a call to<br>
   * {IContainer#readNextPacket(IPacket)}
   */
  public enum ParseType {
    PARSE_NONE,
    /**
     *  full parsing and repack 
     */
    PARSE_FULL,
    /**
     *  Only parse headers, do not repack. 
     */
    PARSE_HEADERS,
    /**
     *  full parsing and interpolation of timestamps for frames not starting on a packet boundary 
     */
    PARSE_TIMESTAMPS,
    /**
     *  full parsing and repack of the first frame only, only implemented for H.264 currently 
     */
    PARSE_FULL_ONCE,
    /**
     *  full parsing and repack with timestamp and position generation by parser for raw<br>
     *                                                              this assumes that each packet in the file contains no demuxer level headers and<br>
     *                                                              just codec level data, otherwise position generation would fail 
     */
    PARSE_FULL_RAW(XugglerJNI.IStream_PARSE_FULL_RAW_get());

    public final int swigValue() {
      return swigValue;
    }

    public static ParseType swigToEnum(int swigValue) {
      ParseType[] swigValues = ParseType.class.getEnumConstants();
      if (swigValue < swigValues.length && swigValue >= 0 && swigValues[swigValue].swigValue == swigValue)
        return swigValues[swigValue];
      for (ParseType swigEnum : swigValues)
        if (swigEnum.swigValue == swigValue)
          return swigEnum;
      throw new IllegalArgumentException("No enum " + ParseType.class + " with value " + swigValue);
    }

    @SuppressWarnings("unused")
    private ParseType() {
      this.swigValue = SwigNext.next++;
    }

    @SuppressWarnings("unused")
    private ParseType(int swigValue) {
      this.swigValue = swigValue;
      SwigNext.next = swigValue+1;
    }

    @SuppressWarnings("unused")
    private ParseType(ParseType swigEnum) {
      this.swigValue = swigEnum.swigValue;
      SwigNext.next = this.swigValue+1;
    }

    private final int swigValue;

    private static class SwigNext {
      private static int next = 0;
    }
  }

}
