/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MEngine
// MFontRef.cpp
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


MFontRef::MFontRef(MFont * font, const char * filename):
	m_font(font){
    M_PROFILE_SCOPE(MFontRef::MFontRef);
	m_filename.set(filename);
}

MFontRef::~MFontRef(void){
    M_PROFILE_SCOPE(MFontRef::~MFontRef);
	clear();
}

MFontRef * MFontRef::getNew(MFont * font, const char * filename)
{
    M_PROFILE_SCOPE(MFontRef::getNew);
	return new MFontRef(font, filename);
}

void MFontRef::clear(void)
{
    M_PROFILE_SCOPE(MFontRef::clear);
	if(m_font)
	{
		m_font->destroy();
		m_font = NULL;
	}
}

void MFontRef::destroy(void){
    M_PROFILE_SCOPE(MFontRef::destroy);
	delete this;
}

void MFontRef::update(void)
{
    M_PROFILE_SCOPE(MFontRef::update);
	MEngine * engine = MEngine::getInstance();

	if(! m_font)
		m_font = MFont::getNew();

	engine->getFontLoader()->loadData(getFilename(), m_font);
}
