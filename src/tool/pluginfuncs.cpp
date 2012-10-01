#include "pluginfuncs.h"
#include "MWindow.h"
#include "MEngine.h"

#include <MPlugin.h>

#ifdef WIN32
#  include <ShlObj.h>
#  include <stdio.h>
#  define snprintf sprintf_s
#else
#  include <stdlib.h>
#endif

MWindow MWindow::s_instance;

int mtool_loadPlugin(lua_State* L)
{
  MPlugin test;
  test.load(lua_tostring(L, -1));
  lua_pushboolean(L, test.isLoaded());

#ifdef WIN32
  char msg[255];
  snprintf(msg, 255, "Plugin failed with error 0x%x", GetLastError());
  lua_pushstring(L, test.isLoaded() ? "No Error" : msg);
#else
  lua_pushstring(L, test.isLoaded() ? "No Error" : dlerror());
#endif

  return 2;
}

int mtool_pluginExt(lua_State* L)
{
#ifdef WIN32
	lua_pushstring(L, "dll");
#elif __APPLE__
	lua_pushstring(L, "dylib");
#else
	lua_pushstring(L, "so");
#endif
	return 1;
}

int mtool_pluginDir(lua_State* L)
{
	char dir[255];
#ifdef WIN32
	char appdata[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appdata);
	sprintf_s(dir, "%s/Maratis/Plugins", appdata);
#elif __APPLE__
	snprintf(dir, 255, "%s/.Maratis/Plugins", getenv("HOME"));
#else
	snprintf(dir, 255, "%s/.Maratis/Plugins", getenv("HOME"));
#endif
	lua_pushstring(L, dir);
	return 1;
}

int mtool_sysPluginDir(lua_State* L)
{
    static char dir[255];
    snprintf(dir, 255, "%s/Plugins", getenv("MSDKDIR"));
    lua_pushstring(L, dir);
    return 1;
}

