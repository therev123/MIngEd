#ifndef __M_CONFIG_FILE_H__
#define __M_CONFIG_FILE_H__

#include "MStringTools.h"

class M_CORE_EXPORT MIConfigFile
{
public:
    MIConfigFile() {}
    virtual void Save() = 0;
    virtual void Load() = 0;

    template<typename T>
    T Read(const char* key)
    { return ConvertFromString<T>(_Read(key)); }

    template<typename T>
    void Write(const char* key, T val)
    { _Write(key, ConvertToString(val).c_str()); }

protected:
    virtual void _Write(const char* key, const char* val) = 0;
    virtual const char* _Read(const char* key) = 0;
};

// just stored here temporarily
class MIFileLoader
{
public:
    virtual void* load(const char*, void* data) = 0;
};


#include <string>

// complex type specialisation

// MVector3
template<>
inline MVector3 MIConfigFile::Read<MVector3>(const char* key)
{
    std::string root = key;
    return MVector3(Read<float>((root+"/x").c_str()),
		    Read<float>((root+"/y").c_str()),
		    Read<float>((root+"/z").c_str()));
}

template<>
inline void MIConfigFile::Write<MVector3>(const char* key, MVector3 val)
{
    std::string root = key;
    Write((root + "/x").c_str(), val.x);
    Write((root + "/y").c_str(), val.y);
    Write((root + "/z").c_str(), val.z);
}

// MVector2
template<>
inline MVector2 MIConfigFile::Read<MVector2>(const char* key)
{
    std::string root = key;
    return MVector2(Read<float>((root+"/x").c_str()),
		    Read<float>((root+"/y").c_str()));
}

template<>
inline void MIConfigFile::Write<MVector2>(const char* key, MVector2 val)
{
    std::string root = key;
    Write((root + "/x").c_str(), val.x);
    Write((root + "/y").c_str(), val.y);
}


#endif/*__M_CONFIG_FILE_H_*/
