/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 2.0.4
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.xuggle.xuggler;
import com.xuggle.ferry.*;
public class IMetaData extends RefCounted {
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
  protected IMetaData(long cPtr, boolean cMemoryOwn) {
    super(XugglerJNI.IMetaData_SWIGUpcast(cPtr), cMemoryOwn);
    swigCPtr = cPtr;
  }
  
  /**
   * Internal Only.
   */
  protected IMetaData(long cPtr, boolean cMemoryOwn,
      java.util.concurrent.atomic.AtomicLong ref)
  {
    super(XugglerJNI.IMetaData_SWIGUpcast(cPtr),
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
  public static long getCPtr(IMetaData obj) {
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
   * Create a new IMetaData object that is actually referring to the
   * exact same underlying native object.
   *
   * @return the new Java object.
   */
  @Override
  public IMetaData copyReference() {
    if (swigCPtr == 0)
      return null;
    else
      return new IMetaData(swigCPtr, swigCMemOwn, getJavaRefCount());
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
    if (obj instanceof IMetaData)
      equal = (((IMetaData)obj).swigCPtr == this.swigCPtr);
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
   * Return the meta-data, with all values expanded.
   */
  @Override
  public String toString()
  {
    StringBuilder result = new StringBuilder();
    result.append(this.getClass().getName()).append("@").append(hashCode());
    result.append("[");
    java.util.Collection<String> keys = getKeys();
    for(String key: keys)
    {
      String value = getValue(key, IMetaData.Flags.METADATA_NONE);
      result.append(key).append("=").append(value).append(";");
    }
    result.append("]");
    return result.toString();
  }
  
  /**
   * Get the set of keys currently in this {@link IMediaData} object
   * as an unordered collection.
   * @return the keys.
   */
  public java.util.Collection<String> getKeys()
  {
    int numKeys = getNumKeys();
    
    java.util.List<String> retval = new java.util.ArrayList<String>(numKeys);
    for(int i = 0; i < getNumKeys(); i++)
    {
      String key = getKey(i);
      if (key != null && key.length() > 0)
        retval.add(key);
    }
    return retval;
  }
 
  /**
   * Return the value for this key, comparing case insensitively.
   * @param key the key
   * @return the value.
   */
  public String getValue(String key)
  {
    return getValue(key, IMetaData.Flags.METADATA_NONE);
  }

  public int getNumKeys() {
    return XugglerJNI.IMetaData_getNumKeys(swigCPtr, this);
  }

  private String getKey(int position) {
    return XugglerJNI.IMetaData_getKey(swigCPtr, this, position);
  }

  public String getValue(String key, IMetaData.Flags flag) {
    return XugglerJNI.IMetaData_getValue(swigCPtr, this, key, flag.swigValue());
  }

  public int setValue(String key, String value) {
    return XugglerJNI.IMetaData_setValue__SWIG_0(swigCPtr, this, key, value);
  }

  public static IMetaData make() {
    long cPtr = XugglerJNI.IMetaData_make();
    return (cPtr == 0) ? null : new IMetaData(cPtr, false);
  }

  public int setValue(String key, String value, IMetaData.Flags flag) {
    return XugglerJNI.IMetaData_setValue__SWIG_1(swigCPtr, this, key, value, flag.swigValue());
  }

  public enum Flags {
    METADATA_NONE(XugglerJNI.IMetaData_METADATA_NONE_get()),
    METADATA_MATCH_CASE(XugglerJNI.IMetaData_METADATA_MATCH_CASE_get()),
    METADATA_DONT_OVERWRITE(XugglerJNI.IMetaData_METADATA_DONT_OVERWRITE_get()),
    META_DATA_APPEND(XugglerJNI.IMetaData_META_DATA_APPEND_get());

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

}