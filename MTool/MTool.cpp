extern "C"
{
#include <lualib.h>
#include <lauxlib.h>
}

#include "pluginChecks.h"

void PrintHelp(const char* run)
{
  printf("usage: %s script arguments...\n", run);
}

int main(int argc, char** argv)
{
  if(argc == 1)
  {
      PrintHelp(argv[0]);
      return 0;
  }
  char* str = (char*)"none";
  
  char** Args;
  
  int nArgs = argc - 2;
  if(nArgs > 0)
    Args = &argv[2];
  else
    Args = &str;

  lua_State* L = lua_open();
  luaL_openlibs(L);

  luaL_loadfile(L, argv[1]);
  
  lua_register(L, "load_plugin", pluginCheck);
  lua_register(L, "plugin_ext", pluginExt);
  lua_register(L, "plugin_dir", pluginDir);

  lua_pcall(L, 0, LUA_MULTRET, 0);
  
  lua_getglobal(L, "main");
  lua_newtable(L);
  for(int i = 0; i < nArgs; ++i)
  {
    lua_pushstring(L, Args[i]);
    lua_rawseti(L, -2, i+1);
  }

  lua_pcall(L, 1, LUA_MULTRET, 0);

  lua_close(L);
}
