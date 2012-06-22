#include "plugin.h"

void Plugin::Draw()
{
  if(m_library)
  {
    if(!m_DrawFunc)
      m_DrawFunc = GetFunction("Render");

    if(m_DrawFunc)
      m_DrawFunc();
  }
}
void Plugin::Update()
{
  if(m_library)
  {
    if(!m_UpdateFunc)
      m_UpdateFunc = GetFunction("UpdatePlugin");

    if(m_UpdateFunc)
      m_UpdateFunc();
  }
}

  pFunc Plugin::GetFunction(const char* name)
  {
#ifdef WIN32
    return reinterpret_cast<pFunc>(GetProcAddress(m_library, name);
#else
      return (pFunc)dlsym(m_library, name);
#endif/*WIN32*/

  }
