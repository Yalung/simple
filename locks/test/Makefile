CFLAGS   := -O2 -g  -Wall -Wextra 
INCLUDES := ../rwspinlock.h ../spinlock.h random.h  \

TestPthreadMutex: testlock.cpp $(INCLUDES) 
	g++ -c $^ $(CFLAGS) -o $@
    
clean:
	rm -rf *.o *.pch 
