/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis
// MPlugin.cpp
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


#include <MEngine.h>
#include "MPlugin.h"
#include <MWindow.h>
#include <MFileTools.h>

#include <stdio.h>

#include <set>
std::set<MIPluginLoadHook*> s_Hooks;

#ifndef MPLUGIN_DYNAMIC
#include <map>
typedef struct _plugin
{
  FunctionPtr Start;
  FunctionPtr End;
  _plugin() : Start(0), End(0) {}
  _plugin(FunctionPtr s, FunctionPtr e) : Start(s), End(e) {}
} plugin;

typedef std::map<const char*, plugin> pluginmap;
typedef pluginmap::iterator pluginiter;
pluginmap g_plugindefs;

void AddPluginFunctions(const char* pluginName, FunctionPtr start, FunctionPtr end)
{
    plugin p(start, end);
    g_plugindefs[pluginName] = p;
}
#endif


MPlugin::MPlugin(void)
{
#ifdef MPLUGIN_DYNAMIC
	m_library = NULL;
#else
	Start= 0;
	End = 0;
#endif
	m_loaded = false;
}

MPlugin::~MPlugin(void)
{
#ifdef MPLUGIN_DYNAMIC
    if(m_library)
    {

	for(std::set<MIPluginLoadHook*>::iterator iHook = s_Hooks.begin();
	    iHook != s_Hooks.end(); iHook++)
	    (*iHook)->Unload(this);

#    ifdef WIN32
	
	FunctionPtr function = reinterpret_cast<FunctionPtr>(GetProcAddress(m_library, "EndPlugin"));
	if(function)
	    function();
	FreeLibrary(m_library);
	
#    else
	
	FunctionPtr function = (FunctionPtr)dlsym(m_library, "EndPlugin");
	if(function)
	    function();
	dlclose(m_library);
	
#    endif
    }
#else		
    if(End)
	End();
#endif
}

void MPlugin::load(const char * filename)
{
#ifdef MPLUGIN_DYNAMIC


	char file[256];
	MWindow* window = MWindow::getInstance();

	char dirs[3][256];
	snprintf(dirs[0], 256, window->getWorkingDirectory());
	snprintf(dirs[1], 256, "%s/Plugins", window->getUserDirectory());
	snprintf(dirs[2], 256, "%s/Plugins", window->getSystemDirectory());
	
	for(int i = 0; i < 3; ++i)
	{
#    ifdef WIN32
	  getGlobalFilename(file, dirs[i], (string(filename) + ".dll").c_str());
#    elseif __APPLE__
	  getGlobalFilename(file, dirs[i], (string(filename) + ".dylib").c_str());
#    else // just assume a linux based os
	  getGlobalFilename(file, dirs[i], (string(filename) + ".so").c_str());
#    endif

	  if(isFileExist(file))
	     break;
	}

#    ifdef WIN32
    
    m_library = LoadLibrary(file);
    if(! m_library);
    return;
    
    FunctionPtr function = reinterpret_cast<FunctionPtr>(GetProcAddress(m_library, "StartPlugin"));
    if(! function)
	return;

    m_filename = filename;
    function();	
    m_loaded = true;

#    else
    
    m_library = dlopen(file, RTLD_LAZY);
    if(! m_library)
    {
	printf("%s\n", dlerror());
	return;
    }

    FunctionPtr function = (FunctionPtr)dlsym(m_library, "StartPlugin");
    if(! function)
    {
	printf(dlerror());
	return;
    }
    
    m_filename = filename;
    function();
    m_loaded = true;
    
#    endif
#else
    // still need to cut up the filename... or something
    pluginiter iplug = g_plugindefs.find(filename);
    if(iplug != g_plugindefs.end())
    {
	Start = iplug->second.Start;
	End = iplug->second.End;
    }
    if(Start)
    {
        Start();
	m_loaded = true;
    }
#endif

    for(std::set<MIPluginLoadHook*>::iterator iHook = s_Hooks.begin();
	iHook != s_Hooks.end(); iHook++)
	(*iHook)->Load(this);

}

void MPlugin::callFunction(const char* fn, bool addSuffix)
{
    char fnName[0xff];
#ifndef MPLUGIN_DYNAMIC
    if(addSuffix)
	snprintf(fnName, 0xff, "%s%s", fn, m_filename.c_str());
    else
#endif
	snprintf(fnName, 0xff, "%s", fn);
    
#ifdef MPLUGIN_DYNAMIC
# ifdef WIN32
    FunctionPtr function = reinterpret_cast<FunctionPtr>(GetProcAddress(m_library, fnName));
# else
    FunctionPtr function = (FunctionPtr)dlsym(m_library, fnName);
# endif
    if(function != 0)
	function();
#else
    // currently no automatic mapping of statically linked plugins
#endif
}

MIPluginLoadHook::MIPluginLoadHook()
{
    s_Hooks.insert(this);
}

MIPluginLoadHook::~MIPluginLoadHook()
{
    s_Hooks.erase(this);
}
