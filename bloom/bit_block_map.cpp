// Copyright (c) 2013, YALUNG
// All rights reserved.

// Author: yalung <yalung929@google.com>
//
// a muti-bits bit map used for standard/counter bloom filter 
// 

#include "bit_block_map.h" 
#include <string.h>
#include <assert.h>

using namespace simple; 

void BitBlockMap::Init(ulong num, uint order) {
    
    assert(num > 0);

    block_bits_order_ = order;
    block_bits_ = 1 << order;
    block_max_value_ = (1 << block_bits_) - 1;

    ulong data_size = ((num << order) >> kBitsOrderOfDataUnit) + 1;   

    data_ = new DataUnitType[data_size];
    assert(data_);
    memset(data_, 0, data_size * sizeof(DataUnitType) );
}

void BitBlockMap::Cleanup() {
    
    if (data_){
      delete []data_; 
      data_ = NULL;
    }
}

ulong BitBlockMap::Get(ulong pos) const {
    
    ulong p1, p2;
    GetPos(pos, p1, p2);
    
    return (data_[p1] >> p2) & block_max_value_;
}

void BitBlockMap::Set(ulong pos) {
    
    ulong p1, p2;
    
    GetPos(pos, p1, p2);
    
    
    data_[p1] |= 1UL << p2;
}

void BitBlockMap::Add(ulong pos) {
   
    DoAdd(pos, 1);
} 

void BitBlockMap::Sub(ulong pos) {
    
    DoAdd(pos, -1);
}

inline void BitBlockMap::DoAdd(ulong pos, long value) {
    
    ulong p1, p2;
    
    GetPos(pos, p1, p2);
    long tmp = (data_[p1] >> p2) & block_max_value_;
    tmp += value;

    if (tmp < 0) tmp = 0;
    if ((ulong)tmp > block_max_value_) tmp = block_max_value_;

    data_[p1] &= ~(block_max_value_ << p2);
    data_[p1] |= (ulong)tmp << p2;
}

inline void BitBlockMap::GetPos(ulong pos, ulong& p1, ulong& p2) const {
    
    p1 = (pos << block_bits_order_) >> kBitsOrderOfDataUnit;
    p2 = (pos << block_bits_order_) - (p1 << kBitsOrderOfDataUnit); 
}

