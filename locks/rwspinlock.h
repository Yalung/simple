// Copyright (c) 2012, YALUNG
// All rights reserved.

// Author: yalung <yalung929@google.com>
//
// A rwspinlock spin a while and sleep.

#ifndef SIMPLE_RWSPINLOCK_H_
#define SIMPLE_RWSPINLOCK_H_

#include "atomic.h"
#include "waiter.h"

namespace simple {

class RWSpinLock {
public:

    enum { FREE = 0, WRITE = 1, READ = 2 };

    RWSpinLock() : lock_(FREE) { }

    bool TryWLock() {
        return atomic_compare_and_swap(&lock_, FREE, WRITE);
    }

    bool TryRLock() {
        atomic_add(&lock_, READ);
        if (lock_ & WRITE) {
            atomic_sub(&lock_, READ);
            return false; 
        }
        return true;
    }
    
    void WLock() {
        Waiter w(2000, 0) ;
        do {
            while( lock_ != FREE ) {
                w.Wait();
            }
        } while(!TryWLock());
    }
    void RLock() {
        Waiter w(2000, 0) ;
        do {
            while( lock_ & WRITE ) {
                w.Wait();
            }
        } while(!TryRLock());
    }
    void UnWLock() {
        // make sure holder's reads and writes do not run after lock_ = FREE
        // ref intel cpu manual.
        // x86 do not reorder writes, compile barrier is enough.
        compile_barrier();
        lock_ = FREE; //aligned int is atomic 
    }

    void UnRLock() {
        atomic_sub(&lock_, READ);
    }

private:
    // support 2^31 concurrent readers.
    volatile unsigned int lock_ __attribute__ ((aligned(sizeof(int)))); 

    RWSpinLock(const RWSpinLock&);
    const RWSpinLock& operator=(const RWSpinLock&);
};

}
#endif //SIMPLE_RWSPINLOCK_H_
           

