
//copyed from kernel source for test only, must run on x86 64bit
#define DECLARE_ARGS(val, low, high)    unsigned low, high
#define EAX_EDX_VAL(val, low, high) ((low) | ((unsigned long)(high) << 32))
#define EAX_EDX_ARGS(val, low, high)    "a" (low), "d" (high)
#define EAX_EDX_RET(val, low, high) "=a" (low), "=d" (high)

inline static unsigned long read_tsc(void)
{
    DECLARE_ARGS(val, low, high);

    asm volatile("rdtsc" : EAX_EDX_RET(val, low, high));

    return EAX_EDX_VAL(val, low, high);
}

// because c's rand may call ptread_mutex, so we use tsc to make randoms
// for my test it is enough
unsigned long Random(unsigned long  start, unsigned long end) {
    return start + ((read_tsc() >> 3) % (end + 1 - start));
}

void Delay(unsigned long loop) {
    volatile unsigned long  i __attribute__ ((aligned(sizeof(long))));
    for (i = 0; i < loop; i++){}
}

