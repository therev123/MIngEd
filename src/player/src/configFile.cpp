#include "configFile.h"

#include <MFileTools.h>

ConfigFile::ConfigFile(const char* name)
    : m_Name(name)
{
    m_Context = tldrInitContext();
    // currently crashes :(
    //Load();
}

void ConfigFile::Save()
{
    static char buf[4096];
    long int size = 4096;
    
    if(tldrGenerateBinary(m_Context, buf, size) != TLDR_SUCCESS)
	return;
    
    MFile* fp = M_fopen(m_Name.c_str(), "wb");
    if(!fp)
	return;
    
    M_fwrite(buf, sizeof(char), size, fp);

    M_fclose(fp);
}

void ConfigFile::Load()
{
    MFile* fp = M_fopen(m_Name.c_str(), "rb");
    if(!fp)
	return;
    M_fseek(fp, 0, SEEK_END);
    long size = M_ftell(fp);
    M_rewind(fp);

    char* buf = new char[size];
    if(size != M_fread(buf, sizeof(char), size, fp))
    {
	M_fclose(fp);
	delete [] buf;
	return;
    }

    M_fclose(fp);

    tldrParseBinary(m_Context, buf, size);
    
    delete [] buf;
}

void ConfigFile::_Write(const char* key, const char* val)
{
    tldrWriteValue(m_Context, key, val);
}

const char* ConfigFile::_Read(const char* key)
{
    static char val[TLDR_STRING_MAX];
    *val = 0;
    tldrReadValue(m_Context, key, val);
    return val;
}
