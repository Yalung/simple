// Copyright (c) 2012, YALUNG
// All rights reserved.

// Author: yalung <yalung929@google.com>
//
// test locks
//

#include <pthread.h>
#include "../rwspinlock.h"
#include "../spinlock.h"

using namespace simple;

class SimpleRWSpinLock : public RWSpinLock {

};

class SimpleSpinLock : public SpinLock {
public:
    void RLock() { SpinLock::Lock(); }
    void WLock() { SpinLock::Lock(); }
    void UnRLock() { SpinLock::UnLock(); }
    void UnWLock() { SpinLock::UnLock(); }
};

class PthreadMutex {
public:
    PthreadMutex() {
        pthread_mutex_init(&lock_);
    }
    void RLock() { pthread_mutex_lock(&lock_);  }
    void WLock() { pthread_mutex_lock(&lock_);  }
    void UnRLock() { pthread_mutex_unlock(&lock_);  }
    void UnWLock() { pthread_mutex_unlock(&lock_);  }
private:
    pthread_mutex_t lock_;
};


class PthreadSpinLock {
public:
    PthreadSpinLock() {
        pthread_spinlock_init (&lock_);
    }
    void RLock() { pthread_spin_lock(&lock_);  }
    void WLock() { pthread_spin_lock(&lock_);  }
    void UnRLock() { pthread_spin_unlock(&lock_);  }
    void UnWLock() { pthread_spin_unlock(&lock_);  }
private:
    pthread_spinlock_t lock_;
};

class PthreadRWLock {
public:
    PthreadRWLock() {
        pthread_rwlock_init (&lock_);
    }
    void RLock() { pthread_spin_lock(&lock_);  }
    void WLock() { pthread_spin_lock(&lock_);  }
    void UnRLock() { pthread_spin_unlock(&lock_);  }
    void UnWLock() { pthread_spin_unlock(&lock_);  }
private:
    pthread_rwlock_t lock_;
};



