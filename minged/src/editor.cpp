#include "editor.h"

#include <stdio.h>

namespace minged
{
  void Editor::Update(uint32 ms)
  {
    CheckToToggle();

    if(m_IsOpen)
      UpdateOpen(dt);
  }

  void Editor::CheckToToggle()
  {
    // if just pressed key, toggle m_IsOpen
  }

  void Editor::UpdateOpen(uint32 dt)
  {
  }
};
