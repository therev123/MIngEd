/*
  NGL - C++ cross-platform framework for OpenGL based applications
  Copyright (C) 2000-2003 NGL Team

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "nui.h"
#include "nglApplication.h"
#include "nglVideoMode.h"
#include "nglWindow.h"
#include "nuiMainMenu.h"

#include <string.h>
#include <wchar.h>

const nglChar* gpWindowErrorTable[] =
{
/*  0 */ _T("No error"),
/*  1 */ _T("System window creation failed"),
/*  2 */ _T("Full screen video mode unavailable"),
/*  3 */ _T("Properties out of range"),
         NULL
};

/*
 * PC-102/105 mapping (XFree86)
 */

#define NGL_KEYMAP_SIZE 0x80
#define NGL_KEYMAP_MASK 0x7F

uint gpKeymapXFree86[NGL_KEYMAP_SIZE] = {
/*  0 */ 0, 0, 0, 0, 0, 0, 0, 0,
/*  8 */ 0, NK_ESC, NK_1, NK_2, NK_3, NK_4, NK_5, NK_6,
/* 10 */ NK_7, NK_8, NK_9, NK_0, NK_MINUS, NK_EQUAL, NK_BACKSPACE, NK_TAB,
/* 18 */ NK_Q, NK_W, NK_E, NK_R, NK_T, NK_Y, NK_U, NK_I,
/* 20 */ NK_O, NK_P, NK_LBRACKET, NK_RBRACKET, NK_ENTER, NK_LCTRL, NK_A, NK_S,
/* 28 */ NK_D, NK_F, NK_G, NK_H, NK_J, NK_K, NK_L, NK_SEMICOLON,
/* 30 */ NK_APOSTROPHE, NK_GRAVE, NK_LSHIFT, NK_NUMBERSIGN, NK_Z, NK_X, NK_C, NK_V,
/* 38 */ NK_B, NK_N, NK_M, NK_COMMA, NK_PERIOD, NK_SLASH, NK_RSHIFT, NK_PAD_ASTERISK,
/* 40 */ NK_LALT, NK_SPACE, NK_CAPSLOCK, NK_F1, NK_F2, NK_F3, NK_F4, NK_F5,
/* 48 */ NK_F6, NK_F7, NK_F8, NK_F9, NK_F10, NK_PAD_LOCK, NK_SCRLOCK, NK_PAD_7,
/* 50 */ NK_PAD_8, NK_PAD_9, NK_PAD_MINUS, NK_PAD_4, NK_PAD_5, NK_PAD_6, NK_PAD_PLUS, NK_PAD_1,
/* 58 */ NK_PAD_2, NK_PAD_3, NK_PAD_0, NK_PAD_PERIOD, 0, 0, NK_BACKSLASH, NK_F11,
/* 60 */ NK_F12, NK_HOME, NK_UP, NK_PAGEUP, NK_LEFT, 0, NK_RIGHT, NK_END,
/* 68 */ NK_DOWN, NK_PAGEDOWN, NK_INSERT, NK_DELETE, NK_PAD_ENTER, NK_RCTRL, NK_PAUSE, NK_SYSREQ,
/* 70 */ NK_PAD_SLASH, NK_RALT, 0, NK_LMETA, NK_RMETA, NK_MENU, 0, 0,
/* 78 */ 0, 0, 0, 0, 0, 0, 0, 0
};

#define STUB() printf("Not implemented: %s:%s\n", __FILE__, __LINE__);

/*
 * X events settings
 */

#define NGL_CLICK_DELAY 250

/*
 * Constructors
 */

nglWindow::nglWindow (uint Width, uint Height, bool IsFullScreen)
{
  Register();
  nglContextInfo context; // Get default context
  nglWindowInfo info(Width, Height, IsFullScreen);

  InternalInit (context, info, NULL);
}

nglWindow::nglWindow (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared)
{
  Register();
  InternalInit (rContext, rInfo, pShared);
}


void nglWindow::InternalInit (const nglContextInfo& rContext, const nglWindowInfo& rInfo, const nglContext* pShared)
{
  STUB();
}

nglWindow::~nglWindow()
{
  if (!mWindow)
    return;
  STUB();

  App->DelWindow (this);


  Unregister();
}


/*
 * Geometry
 */

uint nglWindow::GetWidth() const
{
  return mWidth;
}

uint nglWindow::GetHeight() const
{
  return mHeight;
}

void nglWindow::GetSize (uint& rWidth, uint& rHeight) const
{
  rWidth = mWidth;
  rHeight = mHeight;
}

bool nglWindow::SetSize (uint Width, uint Height)
{
  if (!mWindow) return false;
  STUB();

  return true;
}

void nglWindow::GetPosition (int& rXPos, int& rYPos) const
{
  STUB();
}

bool nglWindow::SetPosition (int XPos, int YPos)
{
  if (!mWindow) return false;
  STUB();

  return true;
}

bool nglWindow::GetResolution (float& rHorizontal, float& rVertical) const
{
  STUB();
  return false;
}


/*
 * Appearance
 */

nglWindow::StateInfo nglWindow::GetState() const
{
  STUB();
  return eVisible;
}

void nglWindow::SetState (StateChange State)
{
  STUB();
}


nglString nglWindow::GetTitle() const
{
  STUB();
  return _T("ngl");
}

void nglWindow::SetTitle (const nglString& rTitle)
{
  STUB();
}


nuiMouseCursor nglWindow::GetCursor() const
{
  return mCursor;
}

bool nglWindow::SetCursor (nuiMouseCursor Cursor)
{
  if (!mWindow) return false;

  if (Cursor == mCursor)
    return true;
  STUB();

  return true;
}


/*
 * Events control
 */

nglWindow::EventMask nglWindow::GetEventMask() const
{
  return mEventMask;
}

void nglWindow::SetEventMask(EventMask Events)
{
  mEventMask = Events;
  // FIXME (update mWindow event field, spares bandwidth on indirect contexts)
  STUB();
}


/*
 * Extension
 */

const nglWindow::OSInfo* nglWindow::GetOSInfo() const
{
  return &mOSInfo;
}


/*
 * Mouse
 */

nglMouseInfo::Mode nglWindow::GetMouseMode() const
{
  return mMouseMode;
}

void nglWindow::SetMouseMode(nglMouseInfo::Mode Mode)
{
  mMouseMode = Mode; // FIXME
}


void nglWindow::GetMouse (nglMouseInfo &rInfo, bool Local) const
{
  STUB();
}


/*
 * Keyboard
 */

bool nglWindow::GetKeyRepeat() const
{
  return mKeyRepeat;
}

void nglWindow::SetKeyRepeat(bool AllowRepeat)
{
  STUB();
}


bool nglWindow::IsKeyDown(nglKeyCode Key) const
{
  if (Key == NK_META)
    return IsKeyDown(NK_LMETA) || IsKeyDown(NK_RMETA);
  else if (Key == NK_CTRL)
    return IsKeyDown(NK_LCTRL) || IsKeyDown(NK_RCTRL);
  else if (Key == NK_SHIFT)
    return IsKeyDown(NK_LSHIFT) || IsKeyDown(NK_RSHIFT);
  else if (Key == NK_ALT)
    return IsKeyDown(NK_LALT) || IsKeyDown(NK_RALT);

  if ((Key > 0) && (Key <= NGL_KEY_MAX))
    return mpKeyState[Key-1];
  else
    return false;
}


/*
 * Drag and drop
 */
void nglWindow::OnDragEnter() {}
void nglWindow::OnDragLeave() {}
nglDropEffect nglWindow::OnCanDrop(nglDragAndDrop* pDragObject, int X, int Y, nglMouseInfo::Flags Button) { return eDropEffectNone; }
void nglWindow::OnDropped (nglDragAndDrop* pDragObject, int X,int Y, nglMouseInfo::Flags Button) {}
bool nglWindow::Drag(nglDragAndDrop* pDragObject) { return false; }

void nglWindow::OnDragRequestData(nglDragAndDrop* pDragObject, const nglString& rMimeType)
{
}
void nglWindow::OnDragStop(bool canceled)
{
}




/*
 * Rendering
 */

void nglWindow::BeginSession()
{
  STUB();
}

bool nglWindow::MakeCurrent() const
{
  return nglContext::MakeCurrent(mWindow);
}


void nglWindow::EndSession()
{
  STUB();
}

void nglWindow::Invalidate()
{
  STUB();
}


/*
 * Internals
 */

Window nglWindow::GetHandle()
{
  return mWindow;
}

void nglWindow::OnXEvent (const XEvent& rXEvent)
{
  STUB();
}

void nglWindow::DoMouseMove (const XMotionEvent& rEvent)
{
  STUB();
}

void nglWindow::DoMouseClick (const XButtonEvent& rEvent)
{
  STUB();
}

bool nglWindow::DoKeyPress (const XKeyEvent& rEvent)
{
  STUB();
  return true;
}

void nglWindow::SetDeco (uint Deco)  // 0 or MWM_DECOR_ALL
{
  STUB();
}

void nglWindow::Grab(bool DoGrab)
{
  STUB();
}

nglMouseInfo::Flags nglWindow::GetXButtons(uint Mask) const
{
  nglMouseInfo::Flags flags = nglMouseInfo::ButtonNone;

  if (Mask & Button1Mask) flags |= nglMouseInfo::ButtonLeft;
  if (Mask & Button2Mask) flags |= nglMouseInfo::ButtonMiddle;
  if (Mask & Button3Mask) flags |= nglMouseInfo::ButtonRight;
  if (Mask & Button4Mask) flags |= nglMouseInfo::ButtonWheelUp;
  if (Mask & Button5Mask) flags |= nglMouseInfo::ButtonWheelDown;

  return flags;
}

Cursor nglWindow::GetXCursor(nuiMouseCursor _Cursor)
{
  STUB();
  // FIXME
  return (Cursor)NULL;
}

void nglWindow::EnterModalState()
{
    NGL_LOG("deb", 0, "Entering modal state");
	App->EnterModalState();
}

void nglWindow::ExitModalState()
{
    NGL_LOG("deb", 0, "Exiting modal state");
	App->ExitModalState();
}

void nglWindow::StartTextInput(int32 X, int32 Y, int32 W, int32 H)
{
  STUB();
  //#FIXME
}

void nglWindow::EndTextInput()
{
  STUB();
  //#FIXME
}

bool nglWindow::IsEnteringText() const
{
  STUB();
  //#FIXME
  return false;
}

