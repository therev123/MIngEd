#include "pluginChecks.h"
#include "MWindow.h"
#include "MEngine.h"

#include <MPlugin/MPlugin.h>

MWindow MWindow::s_instance;

int pluginCheck(lua_State* L)
{
  MPlugin test;
  test.load(lua_tostring(L, -1));
  lua_pushboolean(L, test.isLoaded());

  lua_pushstring(L, test.isLoaded() ? "No Error" : dlerror());

  return 2;
}
