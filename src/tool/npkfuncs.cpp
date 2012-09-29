#include "npkfuncs.h"

#include "util.h"

#include "npk.h"
#include "npk_dev.h"

#include <stdio.h>

#ifndef MAX_PKG
#  define MAX_PKG 10
#endif

typedef struct _npkg
{
    NPK_PACKAGE pkg;
    char path[255];
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
	    _n++;
	}
    ~_npkg()
	{
	    if(id >= 0)
		_pkgs[id] = 0;
	    _n--;
	}
} npkg;

_npkg* _npkg::_pkgs[MAX_PKG];
int _npkg::_n = 0;

int npk_open(lua_State* L)
{
    npkg* pkg = new npkg;
    snprintf(pkg->path, 255, lua_tostring(L, -1));
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
    npkg* pkg  = _npkg::_pkgs[lua_tointeger(L, -3)];
    const char* file = lua_tostring(L, -2);
    const char* ent = lua_tostring(L, -1);

    NPK_ENTITY entity = 0;
    if(pkg && file && ent)
	if(!isDirectory(file))
	    npk_package_add_file(pkg->pkg, file, ent, &entity);
    return 0;
}

void adddir(npkg* pkg, const char* _dir, const char* root)
{
    DIR* dir = opendir(_dir);
    NPK_ENTITY entity;
    struct dirent* ent;
    if(dir == NULL)
	return;
    while((ent = readdir(dir)) != NULL)
    {
	if(strcmp(ent->d_name, ".") == 0 ||
	   strcmp(ent->d_name, "..") == 0)
	    continue;
	char src[0xff];
	snprintf(src, 0xff, "%s/%s", _dir, ent->d_name);
	const char* dest = src;
	if(strstr(dest, root) == dest)
	    dest = dest + strlen(root);
	while(*dest == '/') dest++;
	if(isDirectory(src))
	    adddir(pkg, src, root);
	else
	    npk_package_add_file(pkg->pkg, src, dest, &entity);
    }
}

int npk_add_dir(lua_State* L)
{
    npkg* pkg  = _npkg::_pkgs[lua_tointeger(L, -2)];
    const char* dir = lua_tostring(L, -1);

    if(isDirectory(dir) && pkg && dir)
	adddir(pkg, dir, dir); 

    return 0;
}

int npk_close(lua_State* L)
{
    npkg* pkg  = _npkg::_pkgs[lua_tointeger(L, -3)];
    if(!pkg)
	return 0;

    npk_package_save(pkg->pkg, pkg->path, true);
    npk_package_close(pkg->pkg);
    delete pkg;
    return 0;
}
