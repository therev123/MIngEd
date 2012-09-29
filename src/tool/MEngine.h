#ifndef __MENGINE_H__
#define __MENGINE_H__

// placeholder to allow minimal dependencies with MEngine
#define M_ENGINE_EXPORT


#include <string>
#include <stdio.h>
using std::string;

inline void getGlobalFilename(char* out, const char* dir, const char* file) { sprintf(out, "%s/%s", dir, file); }

#endif/*__MENGINE_H__*/
