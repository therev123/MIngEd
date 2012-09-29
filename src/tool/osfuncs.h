#ifndef __MTOOL_OS_FUNCS_H__
#define __MTOOL_OS_FUNCS_H__

extern "C"
{
#include <lualib.h>
}

// lua functions
int os_mkdir(lua_State* L);

int os_cp(lua_State* L);

int os_cp_of_type(lua_State* L);

int os_rm(lua_State* L);

#endif/*__MTOOL_OS_FUNCS_H__*/
