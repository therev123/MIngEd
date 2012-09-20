#ifndef __CONFIG_FILE_H__
#define __CONFIG_FILE_H__

#include "MCore.h"
#include "MConfigFile.h"

class ConfigFile : public MIConfigFile
{
public:
    ConfigFile(const char* name);
};

class ConfigFileLoader : public MIFileLoader
{
public:
    void* load(const char* name, void* data) { return new ConfigFile(name); }
};

#endif/*__CONFIG_FILE_H__*/
