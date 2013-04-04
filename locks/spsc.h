// Copyright (c) 2012, YALUNG
// All rights reserved.

// Author: yalung <yalung929@google.com>
//
// single-producer and single-consumer queue lock free
//

#ifndef SIMPLE_SPSC_H_
#define SIMPLE_SPSC_H_

#include <vector>
#include <cstdlib>
#include "atomic.h"

namespace simple {

template<typename Tp>
class SPSC {
public:
    SPSC(size_t size) : p_(size), c_(size) {
        queue_.reserve(size + 1);
    }
    // return true for success, false for full.
    bool Enqueue(const Tp& v) {
        if (p_ + 1 == c_) {
            return false;
        }
        queue_[p_.cur_] = v;
        ++p_;
        return true;
    }
    // return true for success, false for empty.
    bool Dequeue(Tp& v) {
        if (c_ == p_) {
            return false;
        }
        v = queue_[c_.cur_];
        ++c_;
        return true;
    }

private:
    std::vector<Tp> queue_;

    //helper
    class Cursor {
    public:
        Cursor(size_t limit): 
            cur_(0), limit_(limit) {
        }
        Cursor& operator=(const Cursor& cursor) {
            cur_ = cursor.cur_;
            limit_ = cursor.limit_;
            return *this;
        }

        bool operator==(const Cursor& cursor) {
            return cur_ == cursor.cur_;
        }

        Cursor operator+(size_t n) {
            Cursor result(limit_);
            result.cur_ = (cur_ + n) % (limit_ + 1);
            return result;
        }

        Cursor& operator++() {
            Cursor tmp = *this + 1;
            // make sure forwad the cursor never run first than preceding codes.
            // according to intel cpu manual.
            // loads and stores do not reorded afetr stores so compile barrier is enough.
            compile_barrier();
            cur_ = tmp.cur_; //write to awligned size_t is atomic
            return *this;
        }

        volatile size_t cur_ __attribute__ ((aligned(sizeof(size_t))));
        size_t limit_;

        };

        Cursor p_;     //producer
        Cursor c_;    //comsumer

        SPSC(const SPSC&);
        const SPSC& operator=(const SPSC&);
    };

}

#endif // SIMPLE_SPSC_H_
