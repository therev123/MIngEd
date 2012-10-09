/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MEngine
// MMaterialAnim.cpp
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


MMaterialAnim::MMaterialAnim(void):
m_opacityKeysNumber(0),
m_shininessKeysNumber(0),
m_customValueKeysNumber(0),
m_diffuseKeysNumber(0),
m_specularKeysNumber(0),
m_emitKeysNumber(0),
m_customColorKeysNumber(0),
m_opacityKeys(NULL),
m_shininessKeys(NULL),
m_customValueKeys(NULL),
m_diffuseKeys(NULL),
m_specularKeys(NULL),
m_emitKeys(NULL),
m_customColorKeys(NULL)
{}

MMaterialAnim::~MMaterialAnim(void)
{
    M_PROFILE_SCOPE(MMaterialAnim::~MMaterialAnim);
	clearOpacityKeys();
	clearShininessKeys();
	clearCustomValueKeys();
	clearDiffuseKeys();
	clearSpecularKeys();
	clearEmitKeys();
	clearCustomColorKeys();
}

MKey * MMaterialAnim::allocOpacityKeys(unsigned int size)
{
    M_PROFILE_SCOPE(MMaterialAnim::allocOpacityKeys);
	clearOpacityKeys();
	if(size == 0)
		return NULL;

	m_opacityKeysNumber = size;
	m_opacityKeys = new MKey[size];
	return m_opacityKeys;
}

MKey * MMaterialAnim::allocShininessKeys(unsigned int size)
{
    M_PROFILE_SCOPE(MMaterialAnim::allocShininessKeys);
	clearShininessKeys();
	if(size == 0)
		return NULL;

	m_shininessKeysNumber = size;
	m_shininessKeys = new MKey[size];
	return m_shininessKeys;
}

MKey * MMaterialAnim::allocCustomValueKeys(unsigned int size)
{
    M_PROFILE_SCOPE(MMaterialAnim::allocCustomValueKeys);
	clearCustomValueKeys();
	if(size == 0)
		return NULL;

	m_customValueKeysNumber = size;
	m_customValueKeys = new MKey[size];
	return m_customValueKeys;
}

MKey * MMaterialAnim::allocDiffuseKeys(unsigned int size)
{
    M_PROFILE_SCOPE(MMaterialAnim::allocDiffuseKeys);
	clearDiffuseKeys();
	if(size == 0)
		return NULL;

	m_diffuseKeysNumber = size;
	m_diffuseKeys = new MKey[size];
	return m_diffuseKeys;
}

MKey * MMaterialAnim::allocSpecularKeys(unsigned int size)
{
    M_PROFILE_SCOPE(MMaterialAnim::allocSpecularKeys);
	clearSpecularKeys();
	if(size == 0)
		return NULL;

	m_specularKeysNumber = size;
	m_specularKeys = new MKey[size];
	return m_specularKeys;
}

MKey * MMaterialAnim::allocEmitKeys(unsigned int size)
{
    M_PROFILE_SCOPE(MMaterialAnim::allocEmitKeys);
	clearEmitKeys();
	if(size == 0)
		return NULL;

	m_emitKeysNumber = size;
	m_emitKeys = new MKey[size];
	return m_emitKeys;
}

MKey * MMaterialAnim::allocCustomColorKeys(unsigned int size)
{
    M_PROFILE_SCOPE(MMaterialAnim::allocCustomColorKeys);
	clearCustomColorKeys();
	if(size == 0)
		return NULL;

	m_customColorKeysNumber = size;
	m_customColorKeys = new MKey[size];
	return m_customColorKeys;
}

void MMaterialAnim::clearOpacityKeys(void)
{
    M_PROFILE_SCOPE(MMaterialAnim::clearOpacityKeys);
	SAFE_DELETE_ARRAY(m_opacityKeys);
	m_opacityKeysNumber = 0;
}

void MMaterialAnim::clearShininessKeys(void)
{
    M_PROFILE_SCOPE(MMaterialAnim::clearShininessKeys);
	SAFE_DELETE_ARRAY(m_shininessKeys);
	m_shininessKeysNumber = 0;
}

void MMaterialAnim::clearCustomValueKeys(void)
{
    M_PROFILE_SCOPE(MMaterialAnim::clearCustomValueKeys)
	SAFE_DELETE_ARRAY(m_customValueKeys);
	m_customValueKeysNumber = 0;
}

void MMaterialAnim::clearDiffuseKeys(void)
{
    M_PROFILE_SCOPE(MMaterialAnim::clearDiffuseKeys);
	SAFE_DELETE_ARRAY(m_diffuseKeys);
	m_diffuseKeysNumber = 0;
}

void MMaterialAnim::clearSpecularKeys(void)
{
    M_PROFILE_SCOPE(MMaterialAnim::clearSpecularKeys);
	SAFE_DELETE_ARRAY(m_specularKeys);
	m_specularKeysNumber = 0;
}

void MMaterialAnim::clearEmitKeys(void)
{
    M_PROFILE_SCOPE(MMaterialAnim::clearEmitKeys);
	SAFE_DELETE_ARRAY(m_emitKeys);
	m_emitKeysNumber = 0;
}

void MMaterialAnim::clearCustomColorKeys(void)
{
    M_PROFILE_SCOPE(MMaterialAnim::clearCustomColorKeys);
	SAFE_DELETE_ARRAY(m_customColorKeys);
	m_customColorKeysNumber = 0;
}
