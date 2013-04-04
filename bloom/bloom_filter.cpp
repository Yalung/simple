// Copyright (c) 2013, YALUNG
// All rights reserved.
//
// Author: yalung <yalung929@google.com>
//
// bloom filter  
// 


#include "bloom_filter.h"
#include <math.h>

using namespace simple; 

BloomFilterBase::BloomFilterBase(ulong n, double p, uint order) {
    
    // formula:  m = -n * ln(p) / ((ln2)^2)
    m_ = (ulong)ceil(n * log(1/p) / (log(2) * log(2)) );

    // formula:  k = m / n * ln2
    k_ = (ulong)ceil(m_ / n * log(2));

    bhf_ = MurMur;
    bbm_.Init(m_, order);
}

BloomFilterBase::BloomFilterBase(ulong m, uint k, uint order) {

    m_ = m;
    k_ = k;

    bhf_ = MurMur;

    bbm_.Init(m_, order);
}

void BloomFilterBase::Insert(const void* data, uint len) {

    ulong hash[2];
    
    bhf_(data, len, hash);
   
     uint i;
    for(i = 0; i < k_; i++) {
        bbm_.Add((hash[0] + hash[2] * i) % m_);
    }
}


void BloomFilterBase::DoRemove(const void* data, uint len) {

    ulong hash[2];

    bhf_(data, len, hash);

     uint i;
    for(i = 0; i < k_; i++) {
        bbm_.Sub((hash[0] + hash[2] * i) % m_);
    }
}

bool BloomFilterBase::Contains(const void* data, uint len) const {
    
    ulong hash[2];

    bhf_(data, len, hash);

    uint i;
    for(i = 0; i < k_; i++) {
        
        if (!bbm_.Get((hash[0] + hash[2] * i) % m_)) {
            return false;
        }
    }

    return true;

}


void BloomFilterBase::SetHashFunc(BloomHashFunc bhf) {
    bhf_ = bhf;
}

void BloomFilterBase::MurMur(const void* data, uint len, ulong hash[2]) {
    
}

BloomFilterBase::~BloomFilterBase() {
    
    bbm_.Cleanup();
}


