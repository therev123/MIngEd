#ifndef __MINGED_EDITOR_H__
#define __MINGED_EDITOR_H__

#include "minged.h"
#include <Rocket/Core.h>
#include "Inventory.h"

namespace minged
{
  class Editor
  {
  public:
    Editor();
    ~Editor();

    void Update(uint32 dt);

    void Render();
    
  protected:
    void CheckToToggle();
    void UpdateOpen(uint32 dt);
    
  private:
    bool m_IsOpen;

	Rocket::Core::Context* m_Rocket;
	Inventory* m_Inv1;
	Inventory* m_Inv2;
  };
};

#endif/*__MINGED_EDITOR_H__*/
