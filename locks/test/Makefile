CFLAGS   := -O2 -g  -Wall -Wextra 

INCLUDES := ../waiter.h ../rwspinlock.h lockers.h ../atomic.h ../spinlock.h random.h  \

BINS := TestPthreadMutex TestPthreadSpinLock TestPthreadRWLock \
	TestSimpleSpinLock TestSimpleRWSpinLock TestSPSC

all: $(BINS)  

TestPthreadSpinLock: testlock.cpp $(INCLUDES)
	g++ $(CFLAGS) -D$@ $^ -o $@ -pthread

TestPthreadRWLock: testlock.cpp $(INCLUDES)
	g++ $(CFLAGS) -D$@ $^ -o $@ -pthread

TestSimpleSpinLock: testlock.cpp $(INCLUDES)
	g++ $(CFLAGS) -D$@ $^ -o $@ -pthread

TestSimpleRWSpinLock: testlock.cpp $(INCLUDES)
	g++ $(CFLAGS) -D$@ $^ -o $@ -pthread

TestPthreadMutex: testlock.cpp $(INCLUDES) 
	g++ $(CFLAGS) -D$@ $^ -o $@ -pthread

TestSPSC: testspsc.cpp ../spsc.h ../atomic.h random.h
	g++ $(CFLAGS) $^ -o $@ -pthread
    
clean:
	rm -f $(BINS) *.o *.pch 
