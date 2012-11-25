#ifndef __MTOOL_UTIL_H__
#define __MTOOL_UTIL_H__

#include <stdio.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>

#ifdef WIN32
	#include <direct.h>
	#define mkdir _mkdir
	#define rmdir _rmdir
    #define snprintf sprintf_s
#else
	#define mkdir(file) mkdir(file, 0777)
        #include <unistd.h>
#endif

bool isDirectory(const char* path);

#endif/*__MTOOL_UTIL_H__*/
