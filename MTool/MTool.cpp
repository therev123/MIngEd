extern "C"
{
#include <lualib.h>
#include <lauxlib.h>
}

void PrintHelp(const char* run)
{
  printf("usage: %s script...\n", run);
}

int main(int argc, char** argv)
{
  if(argc == 1)
  {
      PrintHelp(argv[0]);
      return 0;
  }

  lua_State* L = lua_open();
  luaL_openlibs(L);
  luaL_loadfile(L, argv[1]);
  lua_pcall(L, 0, LUA_MULTRET, 0);
  lua_close(L);
}
