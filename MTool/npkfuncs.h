#ifndef __NPK_FUNCS_H__
#define __NPK_FUNCS_H__

extern "C"
{
#include <lualib.h>
}

int npk_open(lua_State* L);
int npk_add(lua_State* L);
int npk_add_dir(lua_State* L);
int npk_close(lua_State* L);


#endif/*__NPK_FUNCS_H__*/
