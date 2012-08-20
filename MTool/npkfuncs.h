#ifndef __NPK_FUNCS_H__
#define __NPK_FUNCS_H__

extern "C"
{
#include <lualib.h>
}

int npk_new(lua_State* L);
int npk_add(lua_State* L);


#endif/*__NPK_FUNCS_H__*/
