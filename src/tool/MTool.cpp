extern "C"
{
#include <lualib.h>
#include <lauxlib.h>
}

#include "pluginfuncs.h"
#include "osfuncs.h"
#include "npkfuncs.h"
#include "filefuncs.h"

static const luaL_Reg mtool_functions[] = {
  { "load_plugin", mtool_loadPlugin },
  { "plugin_ext",  mtool_pluginExt },
  { "plugin_dir",  mtool_pluginDir },
  { "plugin_dir_sys",  mtool_sysPluginDir },
  { NULL, NULL }
};

static const luaL_Reg os_functions[] = {
  { "mkdir", os_mkdir },
  { "cp", os_cp },
  { "cp_of_type", os_cp_of_type },
  { "rm", os_rm },
  { NULL, NULL }
};

static const luaL_Reg npk_functions[] = {
  { "open", npk_open },
  { "add", npk_add },
  { "add_dir", npk_add_dir },
  { "close", npk_close },
  { NULL, NULL}
};

static const luaL_Reg file_functions[] = {
    { "embed", file_embed },
    { NULL, NULL }
};

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

  luaL_register(L, "tool", mtool_functions);
  luaL_register(L, "os", os_functions);
  luaL_register(L, "npk", npk_functions);
  luaL_register(L, "file", file_functions);

  if(luaL_loadfile(L, argv[1]) != 0)
  {
      printf("ERROR: \n%s\n", lua_tostring(L, -1));
      return 1;
  }
  
  if(lua_pcall(L, 0, LUA_MULTRET, 0) != 0)
  {
      printf("ERROR: \n%s\n", lua_tostring(L, -1));
      lua_close(L);
      return 1;
  }
  
  lua_getglobal(L, "main");
  lua_newtable(L);
  for(int i = 0; i < nArgs; ++i)
  {
    lua_pushstring(L, Args[i]);
    lua_rawseti(L, -2, i+1);
  }

  if(lua_pcall(L, 1, LUA_MULTRET, 0) != 0)
  {
      printf("ERROR: \n%s\n", lua_tostring(L, -1));
      lua_close(L);
      return 1;
  }

  lua_close(L);
  return 0;
}
