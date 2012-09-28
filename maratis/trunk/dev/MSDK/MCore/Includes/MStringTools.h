/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MEngine
// stringTool.h
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
// Copyright (c) 2003-2011 Anael Seghezzi <www.maratis3d.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================


#ifndef _M_STRING_TOOLS_H
#define _M_STRING_TOOLS_H

M_CORE_EXPORT void getLocalFilename(char * out, const char * workingDirectory, const char * filename);
M_CORE_EXPORT void getGlobalFilename(char * out, const char * workingDirectory, const char * filename);
M_CORE_EXPORT void getRepertory(char * out, const char * filename);
M_CORE_EXPORT char * readTextFile(const char * filename);

// string conversion, using std::string because it's a bit easier
#include <string>
#include <sstream>

// some kind of semi-reasonable default?
template<typename T>
std::string M_CORE_EXPORT ConvertToString(T val)
{ std::stringstream ss; ss << val; return ss.str(); }

template<typename T>
T M_CORE_EXPORT ConvertFromString(std::string val)
{
    T rtn; val >> rtn; return rtn;
}

// basic type specialisation
// c-string
template<> inline std::string M_CORE_EXPORT ConvertToString<const char*>(const char* val)
{ return std::string(val); }

template<> inline const char* M_CORE_EXPORT ConvertFromString<const char*>(std::string val)
{ return val.c_str(); }

// std::string
template<> inline std::string M_CORE_EXPORT ConvertToString<std::string>(std::string val)
{ return val; }

template<> inline std::string M_CORE_EXPORT ConvertFromString<std::string>(std::string val)
{ return val; }

// int
template<> inline int M_CORE_EXPORT ConvertFromString<int>(std::string val)
{ return atoi(val.c_str()); }

// float
template<> inline float M_CORE_EXPORT ConvertFromString<float>(std::string val)
{ return atof(val.c_str()); }

// bool
template<> inline bool M_CORE_EXPORT ConvertFromString<bool>(std::string val)
{ return atoi(val.c_str()) != 0; }

#endif
