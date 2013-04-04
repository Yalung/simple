//
// Author: yalung <yalung929@google.com>
//
// bloom filter  
// 

#ifndef SIMPLE_BLOOM_FILTER_H_
#define SIMPLE_BLOOM_FILTER_H_

#include "bit_block_map.h"

namespace simple {

// convention names
// n - how many items u want put in the filter
// p - error rate u can accept
// m - how many bits(bit blocks for counter bloom filter)  
// k - how many hashes

// base class, u do not use this directly

// hash function, return two hash values
// we use formula: hash1 + hash2 * i (i = 0 ~ k - 1) generate k hash values  
typedef void (*BloomHashFunc)(const void* data, uint len, ulong hash[2]);


class BloomFilterBase {
public:
    // auto set m and k from n and p
    // order - bits(1 << order) for one block
    BloomFilterBase(ulong n, double p, uint order = 0);

    // set m and k by caller
    BloomFilterBase(ulong m, uint k, uint order = 0);

    void Insert(const void* data, uint len);

    bool Contains(const void* data, uint len) const;

    ~BloomFilterBase();

    void SetHashFunc(BloomHashFunc bhf);

protected:
    void DoRemove(const void* data, uint len);

private:
    BitBlockMap bbm_;
    uint order_;
    
    ulong m_;
    uint k_;

    BloomHashFunc bhf_;
    static void MurMur(const void* data, uint len, ulong hash[2]);
};

class StdBloomFilter : public BloomFilterBase {

public:
    StdBloomFilter(ulong n, double p) : BloomFilterBase(n, p) { 
    }
    StdBloomFilter(ulong m, uint k)
        : BloomFilterBase(m, k) {
    }
 
};

class CounterBloomFilter : public BloomFilterBase {

public:
    CounterBloomFilter(ulong n, double p) 
        : BloomFilterBase(n, p, kCounterOrder) {
    }
    CounterBloomFilter(ulong m, uint k) 
        : BloomFilterBase(m, k, kCounterOrder) {
    } 
    void Remove(const void* data, uint len) { 
        BloomFilterBase::DoRemove(data, len); 
    }

private:
    // according to paper, 1 << 2 = 4 bit for counter is enough for 
    // most applications 
    static const uint kCounterOrder = 2;
};

}
#endif //
