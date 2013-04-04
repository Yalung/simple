//
// Author: yalung <yalung929@google.com>
//
// A pinlock spin a while and sleep.

#ifndef SIMPLE_SPINLOCK_H_
#define SIMPLE_SPINLOCK_H_

#include "atomic.h"
#include "waiter.h"

namespace simple {

class SpinLock {
public:
    enum { FREE = 0, LOCKED = 1 };

    SpinLock() : lock_(FREE) { }

    bool TryLock() {
        return atomic_compare_and_swap(&lock_, FREE, LOCKED);
    }

    void Lock() {
        Waiter w(2000, 0) ;
        do {
            while( lock_ != FREE ) {
                w.Wait();
            }
        } while(!TryLock());
    }

    void UnLock() {
        // make sure holder's reads and writes do not run after lock_ = FREE
        // ref intel cpu manual.
        // x86 do not reorder writes, compile barrier is enough.
        compile_barrier();
        lock_ = FREE; //aligned int is atomic
    }

private:
    volatile unsigned char lock_  ;

    SpinLock(const SpinLock&);
    const SpinLock& operator=(const SpinLock&);
};

}
#endif //SIMPLE_SPINLOCK_H_


