#include "util.h"

bool isDirectory(const char* path)
{
  DIR* dir = opendir(path);
  if(dir == NULL)
    return false;
  closedir(dir);
  return true;
}
