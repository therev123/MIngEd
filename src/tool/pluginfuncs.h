#ifndef __MTOOL_PLUGIN_FUNCS_H__
#define __MTOOL_PLUGIN_FUNCS_H__

extern "C"
{
#include <lualib.h>
}

// lua functions
int mtool_loadPlugin(lua_State* L);

int mtool_pluginExt(lua_State* L);

int mtool_pluginDir(lua_State* L);

#endif/*__MTOOL_PLUGIN_FUNCS_H__*/
