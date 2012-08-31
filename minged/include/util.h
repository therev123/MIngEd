#ifndef __MINGED_UTIL_H__
#define __MINGED_UTIL_H__

typedef unsigned int uint32;

namespace minged
{
namespace util
{


    uint32 GetTimeMS();

    void Sleep(uint32 ms);

    //----------------------------------------
    // Hash
    // A general purpose hash function
    // http://www.partow.net/programming/hashfunctions/
    //----------------------------------------
    uint32 Hash(const char* Hash);
    
    // returns smallest power of 2 which is greater than the value supplied
    int Pow2(int x);
    
}
};

#endif/*__MINGED_UTIL_H__*/
