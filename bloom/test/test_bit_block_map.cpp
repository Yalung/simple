#include <assert.h>
#include "../bit_block_map.h"

using namespace simple;

const ulong kBlocks = 1000;

void TestBBM(BitBlockMap& bbm, int block_bits) {
    ulong i;

    for(i = 0; i < kBlocks; i++) {
        assert(!bbm.Get(i));
    }

    for(i = 0; i < kBlocks; i+=2) {
        bbm.Set(i);
    }

    for(i = 0; i < kBlocks; i+=2) {
        assert(bbm.Get(i));
    }

    for(i = 1; i < kBlocks; i+=2)
        assert(!bbm.Get(i));

    ulong j;
    if (block_bits > 1) {
       
       ulong block_max = (1 << block_bits) - 1;
    
       for(i = 0; i < kBlocks; i+=2) {
           bbm.Add(i);
       }

       for(i = 1; i < kBlocks; i+=2) 
           assert(bbm.Get(i) == 0);

       for(i = 0; i < kBlocks; i+=2)
           assert(bbm.Get(i) == 2);       

       //up overflow  
       for(i = 0; i < kBlocks; i+=2)
           for(j = 0; j < block_max * 2; j++ ) 
            bbm.Add(i);

       for(i = 0; i < kBlocks; i+=2)
            assert(bbm.Get(i) == block_max);

       for(i = 1; i < kBlocks; i+=2)
           assert(bbm.Get(i) == 0);

       //down onverflow 
       for(i = 0; i < kBlocks; i+=2)
           for(j = 0; j < block_max * 3; j++ )
            bbm.Sub(i);

       for(i = 0; i < kBlocks; i++)
           assert(bbm.Get(i) == 0);
    }
}

int main() {
    simple::BitBlockMap bbm;

    // bit map
    bbm.Init(kBlocks, 0);
    TestBBM(bbm, 1);
    bbm.Cleanup();

    // 2-bits map
    bbm.Init(kBlocks, 1);
    TestBBM(bbm, 1 << 1);
    bbm.Cleanup();

    // 4-bits map
    bbm.Init(kBlocks, 2);
    TestBBM(bbm, 1 << 2);
    bbm.Cleanup();

}

