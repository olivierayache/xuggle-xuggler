/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   IFilterChain.cpp
 * Author: ayache
 * 
 * Created on March 9, 2020, 11:01 PM
 */

#include <com/xuggle/xuggler/IFilterChain.h>

#include "FilterChain.h"

namespace com {
    namespace xuggle {
        namespace xuggler {

            IFilterChain* IFilterChain::make(){
                return FilterChain::make();
            }
            
            IFilterChain::IFilterChain() {
            }

            IFilterChain::~IFilterChain() {
            }
        }
    }
}

