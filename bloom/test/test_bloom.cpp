#include "../bloom_filter.h"
#include <string.h>
#include <stdlib.h>

using namespace simple;

int main(int argc, char** argv) {

    if (argc < 3)
        return 1;

    ulong n, m;
    uint k;
    double p;

    if (argc == 3)
    {
        n = atol(argv[1]);    
        p = 1.0 / (double)(atol(argv[2]));    
    }

    else if (argc == 5)
    {
        m = atol(argv[3]);
        k = atoi(argv[4]);
    }
    else
        return 1;

}







