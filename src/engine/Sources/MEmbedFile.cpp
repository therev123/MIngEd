#include "MEmbedFile.h"

#include "MCore.h"
#include "MFileTools.h"
#include "MEngine.h"


MEmbedFileOpenHook::MEmbedFileOpenHook()
{
    m_DefaultFileOpenHook = M_getFileOpenHook();
    
    M_registerFileOpenHook(this);
}

void MEmbedFileOpenHook::AddEmbeddedFile(const char* name, const char* file, long size)
{
    MEmbedFile toAdd(name, file, size);
    m_Files.push_back(toAdd);
}

MFile* MEmbedFileOpenHook::open(const char* path, const char* mode)
{
    for(fileListIter iFile = m_Files.begin(); iFile != m_Files.end(); iFile++)
	if(iFile->IsFile(path))
	{
	    MEmbedFile& file = *iFile;
	    file.open(path, mode);
	    return &file;
	}
    
    if(m_DefaultFileOpenHook)
	return m_DefaultFileOpenHook->open(path, mode);
    return NULL;
}

MEmbedFile::MEmbedFile(const char* name, const char* file, long size)
    : m_Open(false)
    , m_Filename(name)
    , m_File(file)
    , m_Size(size)
    , m_Ptr(file)
{
}

void MEmbedFile::open(const char* path, const char* mode)
{
    if(IsFile(path))
    {
	m_Open = true;
	m_Ptr = m_File;
    }
}

int	MEmbedFile::close()
{
    m_Open = false;
}

size_t MEmbedFile::read(void* dest, size_t size, size_t count)
{
    memcpy(dest, m_Ptr, size * count);
    m_Ptr += size * count;
}

int	MEmbedFile::seek(long offset, int whence)
{
    if(whence == SEEK_SET)
	m_Ptr = m_File;
    else if(whence == SEEK_END)
	m_Ptr = m_File + m_Size;
    
    m_Ptr += offset;
}

long MEmbedFile::tell()
{
    return m_Ptr - m_File;
}

void MEmbedFile::rewind()
{
    m_Ptr = m_File;
}

bool MEmbedFile::isOpen()
{
    return m_Open;
}

void MEmbedFile::destroy()
{
    //delete this;
}

bool MEmbedFile::IsFile(const char* name)
{
    MEngine* engine = MEngine::getInstance();
    MSystemContext* system = engine->getSystemContext();
    
    char localFilename[256];
    getLocalFilename(localFilename, system->getWorkingDirectory(), name);
    
    return strcmp(localFilename, m_Filename) == 0;
}
