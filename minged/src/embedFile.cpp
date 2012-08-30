#include "embedFile.h"

#include "MCore.h"
#include "MFileTools.h"
#include "MEngine.h"

namespace minged
{


    EmbedFileOpenHook::EmbedFileOpenHook()
    {
	m_DefaultFileOpenHook = M_getFileOpenHook();

	M_registerFileOpenHook(this);
    }

    void EmbedFileOpenHook::AddEmbeddedFile(const char* name, const char* file, long size)
    {
	EmbedFile toAdd(name, file, size);
	m_Files.push_back(toAdd);
    }

    MFile* EmbedFileOpenHook::open(const char* path, const char* mode)
    {
	for(fileListIter iFile = m_Files.begin(); iFile != m_Files.end(); iFile++)
	    if(iFile->IsFile(path))
	    {
		EmbedFile& file = *iFile;
		file.open(path, mode);
		return &file;
	    }

	if(m_DefaultFileOpenHook)
	    return m_DefaultFileOpenHook->open(path, mode);
	return NULL;
    }

    EmbedFile::EmbedFile(const char* name, const char* file, long size)
    : m_Open(false)
    , m_Filename(name)
    , m_File(file)
    , m_Size(size)
    , m_Ptr(file)
    {
    }

    void EmbedFile::open(const char* path, const char* mode)
    {
	if(IsFile(path))
	{
	    m_Open = true;
	    m_Ptr = m_File;
	}
    }

    int	EmbedFile::close()
    {
	m_Open = false;
    }

    size_t EmbedFile::read(void* dest, size_t size, size_t count)
    {
	memcpy(dest, m_Ptr, size * count);
	m_Ptr += size * count;
    }

    int	EmbedFile::seek(long offset, int whence)
    {
	if(whence == SEEK_SET)
	    m_Ptr = m_File;
	else if(whence == SEEK_END)
	    m_Ptr = m_File + m_Size;

	m_Ptr += offset;
    }

    long EmbedFile::tell()
    {
	return m_Ptr - m_File;
    }

    void EmbedFile::rewind()
    {
	m_Ptr = m_File;
    }

    bool EmbedFile::isOpen()
    {
	return m_Open;
    }

    void EmbedFile::destroy()
    {
	//delete this;
    }

    bool EmbedFile::IsFile(const char* name)
    {
	MEngine* engine = MEngine::getInstance();
	MSystemContext* system = engine->getSystemContext();
	
	char localFilename[256];
	getLocalFilename(localFilename, system->getWorkingDirectory(), name);

	return strcmp(localFilename, m_Filename) == 0;
    }
};
