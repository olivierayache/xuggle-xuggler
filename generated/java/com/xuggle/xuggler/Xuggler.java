/* ----------------------------------------------------------------------------
 * This file was automatically generated by SWIG (http://www.swig.org).
 * Version 4.0.2
 *
 * Do not make changes to this file unless you know what you are doing--modify
 * the SWIG interface file instead.
 * ----------------------------------------------------------------------------- */

package com.xuggle.xuggler;

/**
 * Internal Only.
 * 
 * @see com.xuggle.xuggler.Converter
 */

public class Xuggler implements XugglerConstants {

  static {
    // Force the JNI library to load
    XugglerJNI.noop();
  }

  /**
   * Method to force loading of all native methods in the library.
   */
  public static void load() {}
  /**
   * 
   * A simple test of xuggler, this program takes an input
   * file, and outputs it as an output file.
   * 
   * @param args The command line args passed to this program.
   */
  public static void main(String[] args)
  {
    System.out.println("WARNING: The Converter main class has moved to: com.xuggle.xuggler.Converter");
  }
  /**
   * Internal Only.  Do not use.
   */
  public native static void init();


}
