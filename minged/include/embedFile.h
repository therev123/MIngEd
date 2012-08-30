#ifndef __MINGED_EMBED_FILE_H__
#define __MINGED_EMBED_FILE_H__

#include <MCore.h>

#include <list>

namespace minged
{
    class EmbedFile : public MFile
    {
    public:	
	EmbedFile(const char* name, const char* file, long size);

	void	open(const char* path, const char* mode);
	int	close();
	size_t	read(void* dest, size_t size, size_t count);
	size_t	write(const void* str, size_t size, size_t count) {}
	int	print(const char* format, ...) {}
	int	print(const char* format, va_list args) {}
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

    class EmbedFileOpenHook : public MFileOpenHook
    {
    public:
	EmbedFileOpenHook();

	void AddEmbeddedFile(const char* name, const char* file, long size);
	
	MFile* open(const char* path, const char* mode);
	
    private:
	
	typedef std::list<EmbedFile> fileList;
	typedef fileList::iterator    fileListIter;
	fileList m_Files;
	
    
	MFileOpenHook* m_DefaultFileOpenHook;
    };
};


#endif/*__MINGED_EMBED_FILE_H__*/
