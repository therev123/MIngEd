/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MGui
// MGuiEvent.h
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
// Copyright (c) 2003-2011 Anael Seghezzi <www.maratis3d.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================


#ifndef _M_GUI_EVENTS_H
#define _M_GUI_EVENTS_H


enum MGUI_EVENT_TYPE
{
	MGUI_EVENT_MOUSE_MOVE,			// Mouse moved on the GUI
	MGUI_EVENT_MOUSE_BUTTON_DOWN,	// Mouse button pressed on the GUI
	MGUI_EVENT_MOUSE_BUTTON_UP,		// Mouse button released on the GUI
	MGUI_EVENT_DRAW,				// Gui object drawing
	MGUI_EVENT_RESIZE,				// Gui object resize
	MGUI_EVENT_ON_CHANGE,			// On change Gui object
	MGUI_EVENT_SEND_VARIABLE		// On send variable
};

// MWinEvent structure
struct M_GUI_EXPORT MGuiEvent
{
	MGUI_EVENT_TYPE type;
    int data[2];
};

#endif
