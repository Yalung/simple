#include "../bloom_filter.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>
#include <time.h>
#include <assert.h>
#include "md5.h"

using namespace simple;

void Md5HashFunc(const void* data, uint len, ulong hash[2]) {
   
     md5_buffer((const char*)data, len, (char*)hash);
}


void TestBasicLogic() {

    ulong n = 1000000;
    double p = 0.01;
    StdBloomFilter sbf(n, p);
    
    ulong i;

    for (i = 0; i < n; i+=2) {
        sbf.Insert(&i, sizeof(ulong));
    }

    for (i = 0; i< n; i+=2) {
        assert(sbf.Contains(&i, sizeof(ulong)));
    }

    CounterBloomFilter cbf(n, p);

    for (i = 0; i < n; i+=2) {
        cbf.Insert(&i, sizeof(ulong));
    }

    for (i = 0; i< n; i+=2) {
        assert(cbf.Contains(&i, sizeof(ulong)));
    }

    for (i = 0; i < n; i+=2) {
        cbf.Remove(&i, sizeof(ulong));
    }

    for (i = 0; i< n; i++) {
        assert(!cbf.Contains(&i, sizeof(ulong)));
    }
}

inline static void MakeULong(ulong* data) {
  
    uint *ptr = (uint*)data;
    ptr[0] = rand();
    ptr[1] = rand();

}
void TestSearch(BloomFilterBase* bf, ulong n) {
    
    ulong i;
    ulong data;
    
    printf("m:      %ld\n", bf->m());
    printf("k:      %d\n", bf->k());

    srand(time(NULL));   
 
    for (i = 0; i < n; i++) {
        MakeULong(&data);
        data |= 1; //odd number
        bf->Insert(&data, sizeof(ulong));        
    }

    struct timeval tv_start, tv_end;
    struct timezone tz;

    gettimeofday(&tv_start, &tz);

    // do not use rand, we just test speed of search.
    // rand will eat some cpu.
    for (i = 0; i < n * 10; i++) {
         bf->Contains(&i, sizeof(ulong));
    }
    
    gettimeofday(&tv_end, &tz);

    ulong diff = (tv_end.tv_sec * 1000000 + tv_end.tv_usec) -
                 (tv_start.tv_sec * 1000000 + tv_start.tv_usec);

    printf("speed: %ld/s\n", ulong(n * 10.0 / diff * 1000000)  );

    ulong error = 0;
    for (i = 0; i < n * 10; i++) {

       MakeULong(&data);
       data |= 1; 
       data += 1; //even numer

       if ( bf->Contains(&data, sizeof(ulong)) )
           error++;
    }

    printf("p:      %lf\n", ((double)error) / (n * 10.0) );
}

int main(int argc, char** argv) {

    TestBasicLogic();

    ulong n, m;

    uint k;
    double p;

    if (argc < 2) {
        return 0;
    }

    if (!strcmp(argv[1], "sbf")) {

        assert(argc == 4);

        n = atol(argv[2]);
        p = 1.0 / (double)(atol(argv[3]));
        StdBloomFilter sbf(n, p);
        TestSearch(&sbf, n);
        
        return 0;
    }

    if (!strcmp(argv[1], "sbfmk")) {

        assert(argc == 5);
        
        n = atol(argv[2]);
        m = atol(argv[3]);
        k = atol(argv[4]);
        
        StdBloomFilter sbf(m, k);
        
        TestSearch(&sbf, n);
        
        return 0;
    }

    
    if (!strcmp(argv[1], "cbf")) {

        assert(argc == 4);

        n = atol(argv[2]);
        p = 1.0 / (double)(atol(argv[3]));

        CounterBloomFilter sbf(n, p);

        TestSearch(&sbf, n);

        return 0;
    }


    if (!strcmp(argv[1], "md5")) {

        assert(argc == 4);

        n = atol(argv[2]);
        p = 1.0 / (double)(atol(argv[3]));
        StdBloomFilter sbf(n, p);
        sbf.SetHashFunc(Md5HashFunc);
        TestSearch(&sbf, n);

        return 0;
    }


}







