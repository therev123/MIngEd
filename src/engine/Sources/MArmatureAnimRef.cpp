/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MEngine
// MArmatureAnimRef.cpp
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


MArmatureAnimRef::MArmatureAnimRef(MArmatureAnim * armatureAnim, const char * filename):
	m_armatureAnim(armatureAnim){
    M_PROFILE_SCOPE(MArmatureAnimRef::MArmatureAnimRef);
	m_filename = filename;
}

MArmatureAnimRef::~MArmatureAnimRef(void){
    M_PROFILE_SCOPE(MArmatureAnimRef::~MArmatureAnimRef);
	clear();
}

MArmatureAnimRef * MArmatureAnimRef::getNew(MArmatureAnim * armatureAnim, const char * filename){
    M_PROFILE_SCOPE(MArmatureAnimRef::getNew);
	return new MArmatureAnimRef(armatureAnim, filename);
}

void MArmatureAnimRef::clear(void)
{
    M_PROFILE_SCOPE(MArmatureAnimRef::clear);
	if(m_armatureAnim)
	{
		m_armatureAnim->destroy();
		m_armatureAnim = NULL;
	}
}

void MArmatureAnimRef::destroy(void){
    M_PROFILE_SCOPE(MArmatureAnimRef::destroy);
	delete this;
}

void MArmatureAnimRef::update(void)
{
    M_PROFILE_SCOPE(MArmatureAnimRef::update);
	MEngine * engine = MEngine::getInstance();

	if(! m_armatureAnim)
		m_armatureAnim = MArmatureAnim::getNew();

	engine->getArmatureAnimLoader()->loadData(getFilename(), m_armatureAnim);	
}
