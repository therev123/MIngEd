#ifndef __MTOOL_PLUGIN_CHECKS_H__
#define __MTOOL_PLUGIN_CHECKS_H__

extern "C"
{
#include <lualib.h>
}

// lua functions
int pluginCheck(lua_State* L);

int pluginExt(lua_State* L);

int pluginDir(lua_State* L);

#endif/*__MTOOL_PLUGIN_CHECKS_H__*/
