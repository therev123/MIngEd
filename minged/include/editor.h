#ifndef __MINGED_EDITOR_H__
#define __MINGED_EDITOR_H__

#include "minged.h"

namespace minged
{
  class Editor
  {
  public:
    void Update(uint32 dt);
    
  protected:
    void CheckToToggle();
    void UpdateOpen(uint32 dt);
    
  private:
    bool m_IsOpen;
  };
};

#endif/*__MINGED_EDITOR_H__*/
