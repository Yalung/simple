// Copyright (c) 2012, YALUNG
// All rights reserved.

// Author: yalung <yalung929@google.com>
//
// test locks
//  

#include "random.h"
#include <time.h>
#include <pthread.h>
#include "../rwspinlock.h"
#include "../spinlock.h"

template<typename LockType>
class TestLock {
public:
    void Run(int thread_num, int lock_time, int write_ratio) {
        start_time_ = time(NULL);
        thread_num_ = thread_num > kMaxTreads ? kMaxTreads : thread_num;
        memset(ptc_, 0, sizeof(PerThreadCounter) * thread_num_);
        lock_time_ = lock_time;
        write_ratio_ = write_ratio;
        time_t start = time(NULL);
        pthread_t tid;
        int i;
        for (i = 0; i < thread_num; i++) {
            pthread_create(&tid, NULL, Worker, &ptc_[i]);
        }
    }
    
    void PrintResult() {
        time_used = time(NULL) - start_time_;
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
                    ptc_[i].write_count / time_used
                    (ptc_[i].write_count + ptc_[i].read_count)/ time_used,);
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
} ptc_[kMaxTreads];

int thread_num_;
int lock_time_;
int write_ratio_;
time_t start_time_;


static void Delay(long loop) {
    volatile long  i __attribute__ ((aligned(sizeof(long)))); 
    for (i = 0; i < loop; i++);
}

static void* Worker(void* arg) {

        PerThreadCounter* ptc = (PerThreadCounter*)arg;

        while(1) {

            if( Random(1,100) < write_ratio_ + 1 ) {
                lock_.WLock();
                long start = read_tsc();
                Delay(Random(110, 120) * lock_time_);
                ptc->write_count++;
                ptc->locked_time = read_tsc() - start;
                lock_UnWLock();
            } 
            else {
                lock_.RLock();
                long start = read_tsc();
                Delay(Random(110, 120) * lock_time_);
                ptc->read_count++;
                ptc->locked_time = read_tsc() - start;
                lock_.UnRLock();
            }

            Delay(Random(110,120)) * lock_time_ / 2; //give other thread chance
        }
    }
};


int main(int argc, char* argv[]) {
    
}
