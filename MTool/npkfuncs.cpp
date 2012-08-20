#include "npkfuncs.h"
#include "npk.h"
#include "npk_dev.h"

#include <stdio.h>

#ifndef MAX_PKG
#  define MAX_PKG 10
#endif

typedef struct _npkg
{
  NPK_PACKAGE pkg;
  const char* path;
  int id;

  static _npkg* _pkgs[MAX_PKG];
  static int _n;

  _npkg()
  {
    if(_n == 0)
      for(int i=0; i<MAX_PKG; ++i)
	_pkgs[i]= 0;

    id = -1;
    for(int i=0; i<MAX_PKG; ++i)
      if(_pkgs[i] == 0)
      { id = i; _pkgs[i] = this; break; }
  }
  ~_npkg()
  {
    if(id >= 0)
      _pkgs[id] = 0;
  }
} npkg;

_npkg* _npkg::_pkgs[MAX_PKG];
int _npkg::_n = 0;

int npk_new(lua_State* L)
{
  npkg* pkg = new npkg;
  pkg->path = lua_tostring(L, -1);
  int key[4] = {0, 0, 0, 0};

  FILE* fp = fopen(pkg->path, "r");
  if(fp == 0)
    npk_package_alloc(&pkg->pkg, key);
  else
    pkg->pkg = npk_package_open(pkg->path, key);
  lua_pushnumber(L, pkg->id);
  return 1;
}

int npk_add(lua_State* L)
{
  return 0;
}
