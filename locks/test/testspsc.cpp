#include "../spsc.h"
#include "random.h"
#include <pthread.h>
#include <string>
#include <assert.h>

using namespace simple;

SPSC<std::string*> q(100);

void* Producer(void* ) {

   while(1) {
      
      std::string* ptr = new std::string("MM");
      
      while(!q.Enqueue(ptr)) {
         Delay(Random(1, 100));      
      }
      
      Delay(Random(1, 100));
   } 

   return NULL;
}

void* Consumer(void* ) {

    while(1) {
    
        std::string* ptr;
    
        while(!q.Dequeue(ptr)) {
            Delay(Random(1, 100));
        }
    
        assert(*ptr == "MM");
    
        *ptr = "GG";
    
        delete ptr;
        Delay(Random(1, 100));
    }
}

int main() {

    //basic logic
    SPSC<int> qi(3);   
    int i;

    assert(qi.Enqueue(1));
    assert(qi.Enqueue(1));
    assert(qi.Enqueue(1));
    assert(!qi.Enqueue(1));

    assert(qi.Dequeue(i));
    assert(qi.Enqueue(1));
    assert(qi.Dequeue(i));
    assert(qi.Dequeue(i));
    assert(qi.Dequeue(i));
    assert(!qi.Dequeue(i));

    pthread_t tid;
    pthread_create(&tid, NULL, Consumer, NULL);
    Producer(NULL);
}

