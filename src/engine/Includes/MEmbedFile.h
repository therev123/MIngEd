#ifndef __MEMBED_FILE_H__
#define __MEMBED_FILE_H__

#include <MEngine.h>

#include <list>
class M_ENGINE_EXPORT MEmbedFile : public MFile
{
public:	
    MEmbedFile(const char* name, const char* file, long size);
    
    void	open(const char* path, const char* mode);
    int	close();
    size_t	read(void* dest, size_t size, size_t count);
    size_t	write(const void* str, size_t size, size_t count) { return 0; }
    int	print(const char* format, ...) { return 0; }
    int	print(const char* format, va_list args) { return 0; }
    int	seek(long offset, int whence);
    long	tell();
    void	rewind();
    bool 	isOpen();
    void	destroy();
    
    bool IsFile(const char* name);
private:
    const char* m_Filename;
    const char* m_File;
    long        m_Size;
    const char* m_Ptr;
    bool        m_Open;
};

class M_ENGINE_EXPORT MEmbedFileOpenHook : public MFileOpenHook
{
public:
    MEmbedFileOpenHook();
    
    void AddEmbeddedFile(const char* name, const char* file, long size);
    
    MFile* open(const char* path, const char* mode);
    
private:
    
    typedef std::list<MEmbedFile> fileList;
    typedef fileList::iterator    fileListIter;
    fileList m_Files;
    
    
    MFileOpenHook* m_DefaultFileOpenHook;
};


#endif/*__MEMBED_FILE_H__*/
