/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.xuggle.xuggler;
import com.xuggle.ferry.*;
public class IMediaDataWrapper extends IMediaData {
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
  protected IMediaDataWrapper(long cPtr, boolean cMemoryOwn) {
    super(XugglerJNI.IMediaDataWrapper_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }
  
  /**
   * Internal Only.
   */
  protected IMediaDataWrapper(long cPtr, boolean cMemoryOwn,
      java.util.concurrent.atomic.AtomicLong ref)
  {
    super(XugglerJNI.IMediaDataWrapper_SWIGUpcast(cPtr),
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
  public static long getCPtr(IMediaDataWrapper obj) {
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
   * Create a new IMediaDataWrapper object that is actually referring to the
   * exact same underlying native object.
   *
   * @return the new Java object.
   */
  @Override
  public IMediaDataWrapper copyReference() {
    if (swigCPtr == 0)
      return null;
    else
      return new IMediaDataWrapper(swigCPtr, swigCMemOwn, getJavaRefCount());
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
    if (obj instanceof IMediaDataWrapper)
      equal = (((IMediaDataWrapper)obj).swigCPtr == this.swigCPtr);
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
   * Return the object being wrapped
   * 
   * @return the wrapped object
   */
  public IMediaData get()
  {
    IMediaData retval = null;
    if ((retval = getVideoPicture()) == null)
      if ((retval = getAudioSamples()) == null)
        if ((retval = getPacket()) == null)
          if ((retval = getMediaDataWrapper()) == null)
            retval = null;
    return retval;
  }
  
  /**
   * Gets the non IMediaDataWrapper object ultimately wrapped in this
   * wrapper, or null if there isn't one.
   * 
   * @return The non IMediaDataWrapper object ultimately wrapped
   */
  public IMediaData unwrap()
  {
    IMediaData retval = null;
    if ((retval = unwrapVideoPicture()) == null)
      if ((retval = unwrapAudioSamples()) == null)
        if ((retval = unwrapPacket()) == null)
          if ((retval = unwrapMediaDataWrapper()) == null)
            retval = null;
    return retval;
  }

  public void wrap(IMediaData aObj) {
    XugglerJNI.IMediaDataWrapper_wrap(swigCPtr, this, IMediaData.getCPtr(aObj), aObj);
  }

  public void setKey(boolean aIsKey) {
    XugglerJNI.IMediaDataWrapper_setKey(swigCPtr, this, aIsKey);
  }

  public static IMediaDataWrapper make(IMediaData obj) {
    long cPtr = XugglerJNI.IMediaDataWrapper_make(IMediaData.getCPtr(obj), obj);
    return (cPtr == 0) ? null : new IMediaDataWrapper(cPtr, false);
  }

  protected IPacket getPacket() {
    long cPtr = XugglerJNI.IMediaDataWrapper_getPacket(swigCPtr, this);
    return (cPtr == 0) ? null : new IPacket(cPtr, false);
  }

  protected IAudioSamples getAudioSamples() {
    long cPtr = XugglerJNI.IMediaDataWrapper_getAudioSamples(swigCPtr, this);
    return (cPtr == 0) ? null : new IAudioSamples(cPtr, false);
  }

  protected IVideoPicture getVideoPicture() {
    long cPtr = XugglerJNI.IMediaDataWrapper_getVideoPicture(swigCPtr, this);
    return (cPtr == 0) ? null : new IVideoPicture(cPtr, false);
  }

  protected IMediaDataWrapper getMediaDataWrapper() {
    long cPtr = XugglerJNI.IMediaDataWrapper_getMediaDataWrapper(swigCPtr, this);
    return (cPtr == 0) ? null : new IMediaDataWrapper(cPtr, false);
  }

  protected IPacket unwrapPacket() {
    long cPtr = XugglerJNI.IMediaDataWrapper_unwrapPacket(swigCPtr, this);
    return (cPtr == 0) ? null : new IPacket(cPtr, false);
  }

  protected IAudioSamples unwrapAudioSamples() {
    long cPtr = XugglerJNI.IMediaDataWrapper_unwrapAudioSamples(swigCPtr, this);
    return (cPtr == 0) ? null : new IAudioSamples(cPtr, false);
  }

  protected IVideoPicture unwrapVideoPicture() {
    long cPtr = XugglerJNI.IMediaDataWrapper_unwrapVideoPicture(swigCPtr, this);
    return (cPtr == 0) ? null : new IVideoPicture(cPtr, false);
  }

  protected IMediaDataWrapper unwrapMediaDataWrapper() {
    long cPtr = XugglerJNI.IMediaDataWrapper_unwrapMediaDataWrapper(swigCPtr, this);
    return (cPtr == 0) ? null : new IMediaDataWrapper(cPtr, false);
  }

}