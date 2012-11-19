#ifndef __CONFIG_FILE_H__
#define __CONFIG_FILE_H__

#include "MCore.h"
#include "MConfigFile.h"

#include <string>
#include <tldr.h>

class ConfigFile : public MIConfigFile
{
public:
    ConfigFile(const char* name);

    bool KeyExists(const char* key);

    void Save();
    void Load();
protected:
    void _Write(const char* key, const char* val);
    const char* _Read(const char* key);

private:
    std::string  m_Name;
    tldrContext* m_Context;
};

class ConfigFileLoader : public MIFileLoader
{
public:
    void* load(const char* name, void* data) { return new ConfigFile(name); }
};

#endif/*__CONFIG_FILE_H__*/
