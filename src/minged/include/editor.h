#ifndef __MINGED_EDITOR_H__
#define __MINGED_EDITOR_H__

#include "minged.h"

namespace minged
{
    enum btnState
    {
	eUp,
	eDown,
	ePressed,  // first frame
	eReleased, // first frame
    };

    class Editor
    {
    public:
	Editor();
	~Editor();
	
	void Update(uint32 dt);
	
	void Render();
	
	
    private:
	void CheckToToggle();
	void UpdateOpen(uint32 dt);
	void Init();

	bool m_Initialised;
        bool m_IsOpen;
        btnState m_ToggleButton;

	MPackage m_Data;
    };
};

#endif/*__MINGED_EDITOR_H__*/
