/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MEngine
// MObject3dAnim.cpp
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


MObject3dAnim::MObject3dAnim(void):
m_positionKeysNumber(0),
m_scaleKeysNumber(0),
m_rotationKeysNumber(0),
m_positionKeys(NULL),
m_scaleKeys(NULL),
m_rotationKeys(NULL)
{}

MObject3dAnim::~MObject3dAnim(void)
{
    M_PROFILE_SCOPE(MObject3dAnim::~MObject3dAnim);
	clearPositionKeys();
	clearScaleKeys();
	clearRotationKeys();
}

MKey * MObject3dAnim::allocPositionKeys(unsigned int size)
{
    M_PROFILE_SCOPE(MObject3dAnim::allocPositionKeys);
	clearPositionKeys();
	if(size == 0)
		return NULL;

	m_positionKeysNumber = size;
	m_positionKeys = new MKey[size];
	return m_positionKeys;
}

MKey * MObject3dAnim::allocScaleKeys(unsigned int size)
{
    M_PROFILE_SCOPE(MObject3dAnim::allocScaleKeys);
	clearScaleKeys();
	if(size == 0)
		return NULL;

	m_scaleKeysNumber = size;
	m_scaleKeys = new MKey[size];
	return m_scaleKeys;
}

MKey * MObject3dAnim::allocRotationKeys(unsigned int size)
{
    M_PROFILE_SCOPE(MObject3dAnim::allocRotationKeys);
	clearRotationKeys();
	if(size == 0)
		return NULL;

	m_rotationKeysNumber = size;
	m_rotationKeys = new MKey[size];
	return m_rotationKeys;
}

void MObject3dAnim::clearPositionKeys(void)
{
    M_PROFILE_SCOPE(MObject3dAnim::clearPositionKeys);
	SAFE_DELETE_ARRAY(m_positionKeys);
	m_positionKeysNumber = 0;
}

void MObject3dAnim::clearScaleKeys(void)
{
    M_PROFILE_SCOPE(MObject3dAnim::clearScaleKeys)
	SAFE_DELETE_ARRAY(m_scaleKeys);
	m_scaleKeysNumber = 0;
}

void MObject3dAnim::clearRotationKeys(void)
{
    M_PROFILE_SCOPE(MObject3dAnim::clearRotationKeys);
	SAFE_DELETE_ARRAY(m_rotationKeys);
	m_rotationKeysNumber = 0;
}
