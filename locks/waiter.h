// Copyright (c) 2012, YALUNG
// All rights reserved.

// Author: yalung <yalung929@google.com>
//
// A class sleeping.

#ifndef SIMPLE_WAITER_H_
#define SIMPLE_WAITER_H_

#include <unistd.h>
#include <sched.h>

namespace simple {

class Waiter {
public:
    Waiter(int max_spin, int sleep_time ) 
        : loop_(0),
        max_spin_(max_spin),
        sleep_time_(sleep_time_) { 
    }

    void Wait() { 
        if ( loop_ < max_spin_ ) {
            asm volatile ("pasuse");
            loop_++;
        } else {
            if (sleep_time_ <= 0) {
                sched_yield();
            } else {
                usleep(sleep_time_);
            }
        }
    }
private:
    int loop_;
    int max_spin_;
    int sleep_time_;

};

}
#endif //SIMPLE_WAITER_H_
           

