/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MediaFilter.h
 * Author: ayache
 *
 * Created on March 6, 2020, 11:36 AM
 */

#ifndef MEDIAFILTER_H
#define MEDIAFILTER_H

#include <typeinfo>

#include <com/xuggle/ferry/RefCounted.h>
#include <com/xuggle/xuggler/IMediaFilter.h>

extern "C" {
#include "libavfilter/avfilter.h"
#include "libavutil/opt.h"
}

namespace com {
    namespace xuggle {
        namespace xuggler {

            class MediaFilter : public virtual IMediaFilter {
                VS_JNIUTILS_REFCOUNTED_OBJECT_PRIVATE_MAKE(MediaFilter)
            public:
                static MediaFilter* make(AVFilterGraph* graph, const char* name);

                virtual int setIntProperty(const char* name, int value) {
                    if (mFilterContext) {
                        return av_opt_set_int(mFilterContext, name, value, AV_OPT_SEARCH_CHILDREN);
                    }
                    return -1;
                }

                virtual int setDoubleProperty(const char* name, double value) {
                    if (mFilterContext) {
                        return av_opt_set_double(mFilterContext, name, value, AV_OPT_SEARCH_CHILDREN);
                    }
                    return -1;

                }


                virtual int setProperty(const char* name, const char* value) {
                    if (mFilterContext) {
                        return av_opt_set(mFilterContext, name, value, AV_OPT_SEARCH_CHILDREN);
                    }
                    return -1;
                }

                virtual int setRationalProperty(const char* name, IRational* value) {
                    if (mFilterContext) {

                        return av_opt_set_q(mFilterContext, name, (AVRational) {
                            value->getNumerator(), value->getDenominator()
                        }, AV_OPT_SEARCH_CHILDREN);
                    }
                    return -1;
                }

                virtual int initFilter() {
                    return avfilter_init_str(mFilterContext, NULL);
                }

                virtual int addFilter(IMediaFilter* filter);
                
                virtual int addSink(IBufferSink* filterSink);
                
            protected:

                AVFilter* mFilter;
                AVFilterContext* mFilterContext;

                MediaFilter();

                virtual ~MediaFilter();

                int getAvailableInput() {
                    for (int i = 0; i < mFilterContext->nb_inputs; i++) {
                        if (!mFilterContext->inputs[i]) {
                            return i;
                        }
                    }
                    return -1;
                }

                int getAvailableOutput() {
                    for (int i = 0; i < mFilterContext->nb_outputs; i++) {
                        if (!mFilterContext->outputs[i]) {
                            return i;
                        }
                    }
                    return -1;
                }

                AVFilterContext* getAVFilter() {
                    return mFilterContext;
                }

            private:
                //TODO: add RefPoint to next filter


            };

        }
    }
}

#endif /* MEDIAFILTER_H */

