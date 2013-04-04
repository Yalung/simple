// Copyright (c) 2012, YALUNG
// All rights reserved.

// Author: yalung <yalung929@google.com>
// automic operation, for gcc only

#ifndef SIMPLE_ATOMIC_H_
#define SIMPLE_ATOMIC_H_

#define atomic_compare_and_swap __sync_bool_compare_and_swap
#define atomic_add __sync_fetch_and_add
#define atomic_sub __sync_fetch_and_sub
#define compile_barrier() asm volatile ( "" ::: "memory" )

#define mb()    asm volatile("mfence":::"memory")
#define rmb()   asm volatile("lfence":::"memory")
#define wmb()   asm volatile("sfence" ::: "memory")

#endif //SIMPLE_ATOMIC_H_
