/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IMediaFilter.h
 * Author: ayache
 *
 * Created on March 9, 2020, 16:07 AM
 */

#ifndef IMEDIAFILTER_H
#define IMEDIAFILTER_H

#include <com/xuggle/xuggler/IRational.h>
#include <com/xuggle/ferry/RefCounted.h>

namespace com {
    namespace xuggle {
        namespace xuggler {

            class IBufferSink;
            
            class VS_API_XUGGLER IMediaFilter : public com::xuggle::ferry::RefCounted {
            public:

                /**
                 * Sets an integer parameter for this filter
                 * 
                 * @param name the name of the parameter
                 * @param value the value for the parameter
                 * @return 0 on success or <0 if an error occurs
                 * 
                 */
                virtual int setIntProperty(const char* name, int value) = 0;

                /**
                 * Sets a double parameter for this filter
                 * 
                 * @param name the name of the parameter
                 * @param value the value for the parameter
                 * @return 0 on success or <0 if an error occurs
                 * 
                 */
                virtual int setDoubleProperty(const char* name, double value) = 0;
                
                /**
                 * Sets a string parameter for this filter
                 * 
                 * @param name the name of the parameter
                 * @param value the value for the parameter
                 * @return 0 on success or <0 if an error occurs
                 * 
                 */
                virtual int setProperty(const char* name, const char* value) = 0;

                /**
                 * Sets a rational parameter for this filter
                 * 
                 * @param name the name of the parameter
                 * @param value the value for the parameter
                 * @return 0 on success or <0 if an error occurs
                 * 
                 */
                virtual int setRationalProperty(const char* name, IRational* value) = 0;

                /**
                 * Initialize filter. Should be called after setting parameters
                 * 
                 * @return 0 on success <0 if an error occurs 
                 */
                virtual int initFilter() = 0;
                
                /**
                 * Add a filter as an output of this filter
                 * 
                 * @param filter the filter to add
                 * 
                 */
                virtual int addFilter(IMediaFilter* filter) = 0;
                
                /**
                 * Add a filter as an output of this filter
                 * 
                 * @param filter the filter to add
                 * 
                 */
                virtual int addSink(IBufferSink* filterSink) = 0;

            protected:
                virtual ~IMediaFilter() = default;

            };

        }
    }
}

#endif /* IMEDIAFILTER_H */

