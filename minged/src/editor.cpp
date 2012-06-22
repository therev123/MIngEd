#include "editor.h"

#include <stdio.h>

#include <MEngine.h>

namespace minged
{
  Editor::Editor()
  {
    m_IsOpen= true;
  }

  void Editor::Update(uint32 dt)
  {
    CheckToToggle();

    if(m_IsOpen)
      UpdateOpen(dt);
  }

  void Editor::CheckToToggle()
  {
    // if just pressed key, toggle m_IsOpen
    MEngine* engine = MEngine::getInstance();
    if(MInputContext* input = engine->getInputContext())
    {
      if(input->onKeyDown("F1"))
      {
	m_IsOpen = !m_IsOpen;
      }
    }
  }

  void Editor::UpdateOpen(uint32 dt)
  {
  }

  void Editor::Render()
  {
    if(m_IsOpen)
    {
    }
  }
};
