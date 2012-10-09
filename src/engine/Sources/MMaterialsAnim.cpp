/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MEngine
// MMaterialsAnim.cpp
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


#include "../Includes/MEngine.h"


MMaterialsAnim::MMaterialsAnim(void):
m_materialsAnimNumber(0),
m_materialsAnim(NULL)
{}

MMaterialsAnim::~MMaterialsAnim(void)
{
    M_PROFILE_SCOPE(MMaterialsAnim::~MMaterialsAnim);
	clearMaterialsAnim();
}

MMaterialsAnim * MMaterialsAnim::getNew(void)
{
    M_PROFILE_SCOPE(MMaterialsAnim::getNew);
	return new MMaterialsAnim();
}

void MMaterialsAnim::destroy(void)
{
    M_PROFILE_SCOPE(MMaterialsAnim::destroy);
	delete this;
}

MMaterialAnim * MMaterialsAnim::allocMaterialsAnim(unsigned int size)
{
    M_PROFILE_SCOPE(MMaterialsAnim::allocMaterialsAnim);
	clearMaterialsAnim();
	if(size == 0)
		return NULL;

	m_materialsAnimNumber = size;
	m_materialsAnim = new MMaterialAnim[size];
	return m_materialsAnim;
}

void MMaterialsAnim::clearMaterialsAnim(void)
{
    M_PROFILE_SCOPE(MMaterialsAnim::clearMaterialsAnim);
	SAFE_DELETE_ARRAY(m_materialsAnim);
	m_materialsAnimNumber = 0;
}
