// Copyright (c) 2012, YALUNG
// All rights reserved.

// Author: yalung <yalung929@google.com>
//
// test locks
//

#include "random.h"
#include "../rwspinlock.h"
#include "../spinlock.h"
#include "lockers.h"
#include <signal.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

template<typename LockType>
class TestLock {
public:
    void Run(int thread_num, int lock_time, int write_ratio) {
        start_time_ = time(NULL);
        thread_num_ = thread_num > kMaxTreads ? kMaxTreads : thread_num;
        memset(ptc_, 0, sizeof(PerThreadCounter) * thread_num_);
        lock_time_ = lock_time;
        write_ratio_ = write_ratio;
        pthread_t tid;
        int i;
        for (i = 0; i < thread_num; i++) {
            ptc_[i].tl_ptr = this;
            pthread_create(&tid, NULL, Worker, &ptc_[i]);
        }
    }

    void PrintResult() {
        unsigned long time_used = time(NULL) - start_time_;
        printf("Time used %ld s\n", time_used);
        printf("%6s%16s%16s%16s%16s%16s\n", "Thead", "Read", "Read/s", "Write", "Write/s", "R&W/s");
        int i;
        unsigned long rsum = 0, wsum = 0, locked_time_sum = 0;
        for(i = 0; i < thread_num_; i++) {
            rsum += ptc_[i].read_count;
            wsum += ptc_[i].write_count;
            locked_time_sum += ptc_[i].locked_time;
            printf("%6d%16ld%16ld%16ld%16ld%16ld\n", i,
                    ptc_[i].read_count,
                    ptc_[i].read_count / time_used,
                    ptc_[i].write_count,
                    ptc_[i].write_count / time_used,
                    (ptc_[i].write_count + ptc_[i].read_count)/ time_used);
        }
        printf("%6s%16ld%16ld%16ld%16ld%16ld\n", "Total:",
                rsum,
                rsum / time_used,
                wsum,
                wsum / time_used,
                (wsum + rsum )/ time_used);
        printf("Avg locked time: %ld per thread\n", locked_time_sum / thread_num_ );
    }

private:

static const int kMaxTreads = 100;

struct PerThreadCounter {
    unsigned long write_count;
    unsigned long read_count;
    unsigned long locked_time;
    TestLock<LockType>* tl_ptr;
} ptc_[kMaxTreads];

int thread_num_;
int lock_time_;
int write_ratio_;
time_t start_time_;
LockType lock_;


static void* Worker(void* arg) {

        PerThreadCounter* ptc = (PerThreadCounter*)arg;

        while(1) {

            if( (int)Random(1,100) < ptc->tl_ptr->write_ratio_ + 1 ) {
                ptc->tl_ptr->lock_.WLock();
                long start = read_tsc();
                Delay(Random(110, 120) * ptc->tl_ptr->lock_time_);
                ptc->write_count++;
                ptc->locked_time = read_tsc() - start;
                ptc->tl_ptr->lock_.UnWLock();
            }
            else {
                ptc->tl_ptr->lock_.RLock();
                long start = read_tsc();
                Delay(Random(110, 120) * ptc->tl_ptr->lock_time_);
                ptc->read_count++;
                ptc->locked_time = read_tsc() - start;
                ptc->tl_ptr->lock_.UnRLock();
            }

            Delay(Random(110,120) * ptc->tl_ptr->lock_time_ / 2); //give other thread chance
        }

        return NULL;
    }
};



#ifdef TestPthreadMutex
TestLock<PthreadMutex> tl;
#endif // TestPthreadMutex

#ifdef TestPthreadSpinLock
TestLock<PthreadSpinLock> tl;
#endif // TestPthreadSpinlock

#ifdef TestPthreadRWLock
TestLock<PthreadRWLock> tl;
#endif // TestPthreadRWLock


#ifdef TestSimpleSpinLock
    TestLock<SimpleSpinLock> tl;
#endif //TestSimpleSpinlock

#ifdef TestSimpleRWSpinLock
    TestLock<SimpleRWSpinLock> tl;
#endif //TestSimpleRWSpinlock

void PrintResult(int)
{
    tl.PrintResult();
    exit(0);
}

int main(int argc, char* argv[]) {

//basic logic
#ifdef TestSimpleSpinLock

    SpinLock sl;
    assert(sl.TryLock());
    assert(!sl.TryLock());
    sl.UnLock();
    assert(sl.TryLock());
#endif //TestSimpleSpinlock

#ifdef TestSimpleRWSpinLock
    RWSpinLock sl;
    assert(sl.TryRLock());
    assert(sl.TryRLock());
    assert(!sl.TryWLock());
    sl.UnRLock();
    sl.UnRLock();
    assert(sl.TryWLock());
    assert(!sl.TryRLock());
    sl.UnWLock();
    assert(sl.TryRLock());
#endif //TestSimpleRWSpinlock

    if (argc < 4) {
        return -1;
    }

    signal(SIGINT, PrintResult);

    tl.Run(atoi(argv[1]), atoi(argv[2]), atoi(argv[3]));
    while(1) { 
        sleep(10);
    }
}
