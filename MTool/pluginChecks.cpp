#include "pluginChecks.h"
#include "MWindow.h"
#include "MEngine.h"

#include <MPlugin/MPlugin.h>

#ifdef WIN32
#  include <ShlObj.h>
#  include <stdio.h>
#  define snprintf sprintf_s
#endif

MWindow MWindow::s_instance;

int pluginCheck(lua_State* L)
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

int pluginExt(lua_State* L)
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

int pluginDir(lua_State* L)
{
	char dir[255];
#ifdef WIN32
	char appdata[MAX_PATH];
	SHGetFolderPath(NULL, CSIDL_APPDATA, NULL, 0, appdata);
	sprintf_s(dir, "%s/Maratis/Plugins", appdata);
#elif __APPLE__
	snprintf(dir, "%s/.Maratis/Plugins", getenv("HOME"));
#else
	snprintf(dir, "%s/.Maratis/Plugins", getenv("HOME"));
#endif
	lua_pushstring(L, dir);
	return 1;
}

