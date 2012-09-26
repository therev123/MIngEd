#include "configFile.h"

#include "minidom_error.h"

extern "C" {
#include "c_tokenizer.h"
}

#include <MFileTools.h>

ConfigFile::ConfigFile(const char* name)
    : m_Name(name)
{
    //Load();
}

void ConfigFile::Save()
{
    static char buf[4096];
    unsigned int size = 4096;
    
    int res = 0;
    if(strstr(m_Name.c_str(), ".json"))
	res = m_State.dumpString(minidom::doc::JSON, buf, &size);
    else if(strstr(m_Name.c_str(), ".ini"))
	res = m_State.dumpString(minidom::doc::INI, buf, &size);
    else if(strstr(m_Name.c_str(), ".xml"))
	res = m_State.dumpString(minidom::doc::XML, buf, &size);
    if(res != MINIDOM_SUCCESS)
    {
	printf("MINIDOM Error: %d\n", res);
	return;
    }

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

    if(strstr(m_Name.c_str(), ".json"))
	m_State.loadString(minidom::doc::JSON, buf);
    else if(strstr(m_Name.c_str(), ".ini"))
	m_State.loadString(minidom::doc::INI, buf);
    else if(strstr(m_Name.c_str(), ".xml"))
	m_State.loadString(minidom::doc::XML, buf);
    delete [] buf;
}

void ConfigFile::_Write(const char* key, const char* val)
{
    tokenizer_t tok = tokenizer(key, "/", TOKENIZER_EMPTIES_OK);
    minidom::node* node = 0;
    const char* token;
    const char* next = tokenize(&tok);
    while(true)
    {
	token = next;
	next = tokenize(&tok);
	if(!next)
	    break;

	if(node == 0)
	    if(m_State.get(token) == 0)
		node = m_State.add(token, " ");
	    else
		node = m_State.get(token);
	else if(node->get(token) == 0)
	    node = node->add(token, " ");
	else
	    node = node->get(token);
    }
    if(node)
	node->add(token, val);
}

const char* ConfigFile::_Read(const char* key)
{
    return m_State.get(key)->toChars();
}
