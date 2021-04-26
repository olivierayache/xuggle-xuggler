/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.2
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.xuggle.xuggler;
import com.xuggle.ferry.*;
public class IMediaFilter extends RefCounted {
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
  protected IMediaFilter(long cPtr, boolean cMemoryOwn) {
    super(XugglerJNI.IMediaFilter_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }
  
  /**
   * Internal Only.
   */
  protected IMediaFilter(long cPtr, boolean cMemoryOwn,
      java.util.concurrent.atomic.AtomicLong ref)
  {
    super(XugglerJNI.IMediaFilter_SWIGUpcast(cPtr),
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
  public static long getCPtr(IMediaFilter obj) {
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
   * Create a new IMediaFilter object that is actually referring to the
   * exact same underlying native object.
   *
   * @return the new Java object.
   */
  @Override
  public IMediaFilter copyReference() {
    if (swigCPtr == 0)
      return null;
    else
      return new IMediaFilter(swigCPtr, swigCMemOwn, getJavaRefCount());
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
    if (obj instanceof IMediaFilter)
      equal = (((IMediaFilter)obj).swigCPtr == this.swigCPtr);
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
   * Sets an integer parameter for this filter<br>
   * <br>
   * @param name the name of the parameter<br>
   * @param value the value for the parameter<br>
   * @return 0 on success or &lt;0 if an error occurs
   */
  public int setIntProperty(String name, int value) {
    return XugglerJNI.IMediaFilter_setIntProperty(swigCPtr, this, name, value);
  }

  /**
   * Sets a double parameter for this filter<br>
   * <br>
   * @param name the name of the parameter<br>
   * @param value the value for the parameter<br>
   * @return 0 on success or &lt;0 if an error occurs
   */
  public int setDoubleProperty(String name, double value) {
    return XugglerJNI.IMediaFilter_setDoubleProperty(swigCPtr, this, name, value);
  }

  /**
   * Sets a string parameter for this filter<br>
   * <br>
   * @param name the name of the parameter<br>
   * @param value the value for the parameter<br>
   * @return 0 on success or &lt;0 if an error occurs
   */
  public int setProperty(String name, String value) {
    return XugglerJNI.IMediaFilter_setProperty(swigCPtr, this, name, value);
  }

  /**
   * Sets a rational parameter for this filter<br>
   * <br>
   * @param name the name of the parameter<br>
   * @param value the value for the parameter<br>
   * @return 0 on success or &lt;0 if an error occurs
   */
  public int setRationalProperty(String name, IRational value) {
    return XugglerJNI.IMediaFilter_setRationalProperty(swigCPtr, this, name, IRational.getCPtr(value), value);
  }

  /**
   * Initialize filter. Should be called after setting parameters<br>
   * <br>
   * @return 0 on success &lt;0 if an error occurs 
   */
  public int initFilter() {
    return XugglerJNI.IMediaFilter_initFilter(swigCPtr, this);
  }

  /**
   * Add a filter as an output of this filter<br>
   * <br>
   * @param filter the filter to add
   */
  public int addFilter(IMediaFilter filter) {
    return XugglerJNI.IMediaFilter_addFilter(swigCPtr, this, IMediaFilter.getCPtr(filter), filter);
  }

  /**
   * Add a filter as an output of this filter<br>
   * <br>
   * 
   */
  public int addSink(IBufferSink filterSink) {
    return XugglerJNI.IMediaFilter_addSink(swigCPtr, this, IBufferSink.getCPtr(filterSink), filterSink);
  }

}