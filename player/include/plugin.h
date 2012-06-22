#ifndef __MINGED_PLUGIN_H__
#define __MINGED_PLUGIN_H__

#include <MEngine.h>
#include <MPlugin/MPlugin.h>

typedef void*(*pFunc)();

class Plugin : public MPlugin
{
 public:
  void Update();
  void Draw();

  pFunc GetFunction(const char* name);
 private:
  pFunc m_UpdateFunc;
  pFunc m_DrawFunc;
};

#endif/*__MINGED_PLUGIN_H__*/
