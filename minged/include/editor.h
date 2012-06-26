#ifndef __MINGED_EDITOR_H__
#define __MINGED_EDITOR_H__

#include "minged.h"

namespace minged
{
  class Editor
  {
  public:
    Editor();

    void Update(uint32 dt);

    void Render();
    
  protected:
    void CheckToToggle();
    void UpdateOpen(uint32 dt);
    
  private:
    bool m_IsOpen;
  };
};

#endif/*__MINGED_EDITOR_H__*/
