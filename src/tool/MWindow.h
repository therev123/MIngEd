#ifndef __MWINDOW_H__
#define __MWINDOW_H__

#ifdef WIN32
#  include <direct.h>
#else
#  include <unistd.h>
#endif

// placeholder to allow minimal dependencies with MEngine
class MWindow
{
  static MWindow s_instance;
 public:
  static inline MWindow* getInstance() { return &s_instance; }
  inline const char* getWorkingDirectory() { return getcwd(NULL, 0); }
  inline const char* getUserDirectory() { return getcwd(NULL, 0); }
  inline const char* getSystemDirectory() { return getcwd(NULL, 0); }
};


#endif/*__MWINDOW_H__*/
