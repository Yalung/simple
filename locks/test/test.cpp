#include "rwspinlock.h"
#include <assert.h>
#include <stdio.h>


namespace test{
  #define TEST_123 123
}

int main() {
    simple::RWSpinLock lock;
    assert(lock.TryRLock());
    assert(lock.TryRLock());
    assert(!lock.TryWLock());
    lock.UnRLock();
    lock.UnRLock();
    assert(lock.TryWLock());
    assert(!lock.TryWLock());
    assert(!lock.TryRLock());
    printf("%d\n", TEST_123);
    return 0;
}

