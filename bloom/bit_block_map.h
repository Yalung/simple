//
// Author: yalung <yalung929@google.com>
//
// a muti-bits bit map used for standard/counter bloom filter 
// 

#ifndef SIMPLE_BITBLOCKMAP_H_
#define SIMPLE_BITBLOCKMAP_H_

namespace simple {

typedef unsigned long ulong;
typedef unsigned int uint;

class BitBlockMap {
public:
    BitBlockMap() : data_(0) {}
    // num - how many block 
    // order - bits for one block = 2 ^ order
    // aligned to 2^X so we can use fast shift operation   
    void Init(ulong num, uint order);
    void Cleanup();

    ulong Get(ulong pos) const;
    void Set(ulong pos);

    void Add(ulong pos);
    void Sub(ulong pos);

private:
    typedef ulong DataUnitType;
    DataUnitType* data_;

    uint block_bits_order_;    
    uint block_bits_;
    ulong block_max_value_;

    void GetPos(ulong pos, ulong& p1, ulong& p2) const;
    void DoAdd(ulong pos, long value);    

    static const int kBitsOrderOfDataUnit = 
        sizeof(DataUnitType) == 8 ? 6 : 5; // 64 = 1 << 6, 32 = 1 << 5;
};

}

#endif //
