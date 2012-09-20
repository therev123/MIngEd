#ifndef __M_CONFIG_FILE_H__
#define __M_CONFIG_FILE_H__

class M_CORE_EXPORT MIConfigFile
{
    // decide on API
};

// just stored here temporarily
class MIFileLoader
{
public:
    virtual void* load(const char*, void* data) = 0;
};

#endif/*__M_CONFIG_FILE_H_*/
