#include "osfuncs.h"
#include "util.h"

bool copy(const char* source, const char* target)
{
  char destination[0xff];
  if(isDirectory(target))
  {
    if(strstr(source, "/") == 0)
      snprintf(destination, 0xff, "%s/%s", target, source);
    else
    {
      const char* pSrc = source + strlen(source);
      while(*pSrc != '/') pSrc--;
      snprintf(destination, 0xff, "%s%s", target, pSrc);
    }
  }
  else
    snprintf(destination, 0xff, target);

  struct stat stats;
  stat(source, &stats);

  FILE* src = fopen(source, "r");
  if(src == 0)
  {
      printf("Error reading from %s\n", source);
      return false;
  }
  
  FILE* dest = fopen(destination, "w");
  if(dest == 0)
  {
      printf("Error writing to %s\n", destination);
      fclose(src);
      return false;
  }
  
  char buf[1024];
  int n;
  
  while((n = fread(buf, 1, 1024, src)) > 0)
      fwrite(buf, 1, n, dest);

  fclose(src);
  fclose(dest);

  // preserve permissions
  chmod(destination, stats.st_mode);
  
  return true;
}

bool copydir(const char* source, const char* target, const char* type = 0)
{
  DIR* dir = opendir(source);
  struct dirent* ent;
  if(dir == NULL)
    return false;
  while((ent = readdir(dir)) != NULL)
  {
    if(strcmp(ent->d_name, ".") == 0 ||
       strcmp(ent->d_name, "..") == 0)
      continue;
    char dest[0xff];
    char src[0xff];
    snprintf(dest, 0xff, "%s/%s", target, ent->d_name);
    snprintf(src, 0xff, "%s/%s", source, ent->d_name);
    if(isDirectory(src) && type == 0)
    {
      mkdir(dest);
      copydir(src, dest, type);
    }
    else if(type == 0 || strstr(src, type) == src + strlen(src) - strlen(type))
      copy(src, dest);
  }
}

int os_mkdir(lua_State* L)
{
  mkdir(lua_tostring(L, -1));
  
  return 0;
}

int os_cp(lua_State* L)
{
  const char* source = lua_tostring(L, -2);
  const char* target = lua_tostring(L, -1);

  if(isDirectory(source))
    copydir(source, target);
  else
    copy(source, target);
  return 0;
}


int os_cp_of_type(lua_State* L)
{
  const char* source = lua_tostring(L, -3);
  const char* target = lua_tostring(L, -2);
  const char* type   = lua_tostring(L, -1);

  if(isDirectory(source))
    copydir(source, target, type);
  else
    copy(source, target);
  return 0;
}

void _rmdir(const char* path)
{
    DIR* dir = opendir(path);
    dirent* ent = NULL;
    while(ent = readdir(dir))
	if(isDirectory(ent->d_name))
	    rmdir(ent->d_name);
	else
	    remove(ent->d_name);
    closedir(dir);
}

int os_rm(lua_State* L)
{
    const char* path = lua_tostring(L, -1);
    if(isDirectory(path))
	_rmdir(path);
    else
	remove(path);
}
