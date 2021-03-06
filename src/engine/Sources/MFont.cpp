/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MEngine
// MFont.cpp
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


MCharacter::MCharacter(void):
	m_xAdvance(0),
	m_offset(0, 0),
	m_pos(0, 0),
	m_scale(0, 0)
{}


MFont::MFont(void):
m_textureId(0),
m_fontSize(0),
m_textureWidth(0),
m_textureHeight(0)
{}

MFont::~MFont(void)
{
    M_PROFILE_SCOPE(MFont::~MFont);
	MRenderingContext * render = MEngine::getInstance()->getRenderingContext();
	render->deleteTexture(&m_textureId);
}

MFont * MFont::getNew(void)
{
    M_PROFILE_SCOPE(MFont::getNew);
	return new MFont();
}

void MFont::destroy(void)
{
    M_PROFILE_SCOPE(MFont::destroy);
	delete this;
}

void MFont::setCharacter(unsigned int charCode, const MCharacter & character)
{
    M_PROFILE_SCOPE(MFont::setCharacter);
	m_characters[charCode] = character;
}

MCharacter * MFont::getCharacter(unsigned int charCode)
{
    M_PROFILE_SCOPE(MFont::getCharacter);
	map<unsigned int, MCharacter>::iterator iter = m_characters.find(charCode);
	if(iter != m_characters.end())
		return &iter->second;

	return NULL;
}

unsigned int MFont::getCharactersNumber(void)
{
    M_PROFILE_SCOPE(MFont::getCharactersNumber);
	return (unsigned int)m_characters.size();
}

MCharacter * MFont::getCharacterByIndex(unsigned int id)
{
    M_PROFILE_SCOPE(MFont::getCharacterByIndex);
	map<unsigned int, MCharacter>::iterator iter(m_characters.begin());

	for(unsigned int i=0; i<id; i++)
		iter++;

	if(iter != m_characters.end())
		return &iter->second;

	return NULL;
}
