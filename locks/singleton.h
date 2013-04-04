// Copyright (c) 2012, YALUNG
// All rights reserved.

// Author: yalung <yalung929@google.com>
//
// singleton support mutithread but lock free
//  

#ifndef SIMPLE_SINGLETON_H
#define SIMPLE_SINGLETON_H

#include "aotmic.h"

namespace simple {

template<typename Tp>
class Singleton {
public:
    static Tp* Instance() {
        Tp* inst;

        if(inst_ == () {
            inst = new Tp; 
            if (!atomic_compare_and_swap(&inst_, NULL, inst)) {
                delete inst; //other one set it.
            }
        }
        return inst;
    }

    static void DeleteInstance() {
        Tp* tmp = inst_;
        if (inst_)    {
            if(atomic_compare_and_swap(&inst_, tmp, NULL)) {
                delete tmp; 
            }
        }
    }
private:
    static Tp* inst_;

};

}

#endif //SIMPLE_SINGLETON_H