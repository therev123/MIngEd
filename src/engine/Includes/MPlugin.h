/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis
// MPlugin.h
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


#ifndef _M_PLUGIN_H
#define _M_PLUGIN_H

#ifdef MPLUGIN_DYNAMIC
#  ifdef WIN32
#    include <windows.h>
#    define MPLUGIN_EXPORT __declspec(dllexport)
#  else
// automagically exported
#    define MPLUGIN_EXPORT
#    include <dlfcn.h>
#  endif /*WIN32*/
#endif/*MPLUGIN_DYNAMIC*/

typedef void * (*FunctionPtr)();

class M_ENGINE_EXPORT MPlugin
{
public :

	MPlugin(void);
	~MPlugin(void);

protected :

	string m_filename;

#ifdef MPLUGIN_DYNAMIC	
#    ifdef WIN32
	HMODULE m_library;
#    else
	void * m_library;
#    endif
#else
        FunctionPtr Start;
        FunctionPtr End;

#endif
	bool m_loaded;
public:
	
	void load(const char * filename);
	inline const char * getFilename(void){ return m_filename.c_str(); }
	inline bool isLoaded(void) { return m_loaded; }
	void callFunction(const char* fn, bool addSuffix = true);
};

// Some nasty macros to help making plugins for both systems that can dynamically
// load libraries (ie. sensible systems) and for annoying systems like iOS
// where we can't have shared objects.
//
// to use, in the plugin somewhere do as follows:
//------------AwesomePlugin.h---------------------------------------------------
// #include <MPlugin.h>
// 
// MPLUGIN_DECLARE(MyAwesomePlugin);
//------------AwesomePlugin.cpp-------------------------------------------------
// #include "AwesomePlugin.h"
//
// MPLUGIN_START_IMPLEMENT(MyAwesomePlugin)
// {
//   // this is where you do your code
// }
//
// MPLUGIN_END_IMPLEMENT(MyAwesomePlugin)
// {
//   // cleanup here
// }
//
// Everything else should be taken care of

#ifdef MPLUGIN_DYNAMIC
#    define MPLUGIN_DECLARE(name)		\
  extern "C" {					\
  MPLUGIN_EXPORT void StartPlugin();		\
  MPLUGIN_EXPORT void EndPlugin();		\
  }

#    define MPLUGIN_START_IMPLEMENT(name)	\
  void StartPlugin()

#    define MPLUGIN_END_IMPLEMENT(name)		\
  void EndPlugin()
#else
#    define MPLUGIN_DECLARE(name)			\
  void StartPlugin##name();				\
  void EndPlugin##name();				\
  class Plugin##name##AutoStart				\
  {							\
  public:						\
    Plugin##name##AutoStart();				\
  }							\
  extern Plugin##name##AutoStart s_##name##AutoStarter;

#   define MPLUGIN_START_IMPLEMENT(name)				\
    void AddPluginFunctions(const char* pluginName, FunctionPtr start, FunctionPtr end); \
  Plugin##name##AutoStart s_##name##AutoStarter;			\
  Plugin##name##AutoStart::Plugin##name##AutoStart()			\
  {									\
    AddPluginFunctions(#name, &StartPlugin##name, &EndPlugin##name);	\
  }									\
  void StartPlugin##name()

#   define MPLUGIN_END_IMPLEMENT(name)		\
  void EndPlugin##name()
#endif/*MPLUGIN_DYNAMIC*/

class MIPluginLoadHook
{
public:
    MIPluginLoadHook();
    ~MIPluginLoadHook();

    virtual void Load(MPlugin* plugin) {}
    virtual void Unload(MPlugin* plugin) {}
};

#endif
