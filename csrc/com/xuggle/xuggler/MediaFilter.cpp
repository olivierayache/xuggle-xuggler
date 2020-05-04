/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   MediaFilter.cpp
 * Author: ayache
 * 
 * Created on March 6, 2020, 11:36 AM
 */

#include "MediaFilter.h"
#include "BufferSink.h"
#include <com/xuggle/ferry/Logger.h>

VS_LOG_SETUP(VS_CPP_PACKAGE);

namespace com {
    namespace xuggle {
        namespace xuggler {

            MediaFilter* MediaFilter::make(AVFilterGraph* graph, const char* name) {
                MediaFilter* retval = MediaFilter::make();
                if (retval) {
                    retval->mFilter = avfilter_get_by_name(name);
                    retval->mFilterContext = avfilter_graph_alloc_filter(graph, retval->mFilter, name);
                    if (!retval->mFilterContext) {
                        VS_REF_RELEASE(retval);
                    }
                }
                return retval;
            }

            int MediaFilter::addFilter(IMediaFilter* filter) {
                MediaFilter* f = dynamic_cast<MediaFilter*> (filter);
                VS_LOG_WARN("type of filter %s %p %p", typeid (filter).name(), f, filter);
                if (f) {
                    return avfilter_link(mFilterContext, getAvailableOutput(), f->getAVFilter(), f->getAvailableInput());
                }
                return -1;
            }
            
            int MediaFilter::addSink(IBufferSink* filter) {
                BufferSink* f = dynamic_cast<BufferSink*> (filter);
                VS_LOG_WARN("type of filter %s %p %p", typeid (filter).name(), f, filter);
                if (f) {
                    return avfilter_link(mFilterContext, getAvailableOutput(), f->getAVFilter(), f->getAvailableInput());
                }
                return -1;
            }
            
            

            MediaFilter::MediaFilter() {
                VS_LOG_WARN("create MF");
                VS_LOG_WARN("type of filter %s %p", typeid (this).name(), this);
                mFilter = NULL;
                mFilterContext = NULL;
            }

            MediaFilter::~MediaFilter() {
                if (mFilterContext) {
                    avfilter_free(mFilterContext);
                }
                mFilterContext = NULL;
                mFilter = NULL;
            }


        }
    }
}

