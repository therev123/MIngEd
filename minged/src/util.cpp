#include "util.h"

#include <time.h>
#include <cstring>

#define MStoNS(_ms) (_ms * 1000000)
#define NStoMS(_ns) (_ns / 1000000)

namespace minged
{
namespace util
{

uint32 GetTimeMS()
{
#ifdef WIN32
    return (uint32)((( (float)clock() ) / CLOCKS_PER_SEC) * 1000);
#else
    struct timespec t;
    clock_gettime(CLOCK_MONOTONIC, &t);
    return (t.tv_sec * 1000) + NStoMS(t.tv_nsec);
#endif
}

void Sleep(uint32 ms)
{
#ifdef WIN32
    Sleep(ms);
#else
    struct timespec t;
    t.tv_nsec = MStoNS(ms);
    t.tv_sec = 0;
    nanosleep(&t, NULL);
#endif
}

// BPHash function as found on
// http://www.partow.net/programming/hashfunctions/
uint32 Hash(const char* str)
{
    uint32 len = strlen(str);
    uint32 hash = 0;
    uint32 i    = 0;
    for(i = 0; i < len; str++, i++)
	hash = hash << 7 ^ (*str);
    
    return hash;
}
    
int Pow2(int x)
{
    --x;
    x |= x >> 1;
    x |= x >> 2;
    x |= x >> 4;
    x |= x >> 8;
    x |= x >> 16;
    return x+1;
}


}
}
