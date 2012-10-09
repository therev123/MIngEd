/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MEngine
// MMesh.cpp
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


// MDisplay
MDisplay::MDisplay(M_PRIMITIVE_TYPES primitiveType, unsigned int begin, unsigned int size):
m_primitiveType(primitiveType),
m_begin(begin),
m_size(size),
m_cullMode(M_CULL_BACK),
m_material(NULL),
m_visibility(true)
{}

MDisplay::MDisplay(const MDisplay & display):
m_primitiveType(display.m_primitiveType),
m_begin(display.m_begin),
m_size(display.m_size),
m_cullMode(display.m_cullMode),
m_material(display.m_material),
m_visibility(display.m_visibility)
{}

MDisplay::~MDisplay(void)
{}


// MSubMesh
MSubMesh::MSubMesh(void):

m_morphingData(NULL),
m_skinData(NULL),

m_indicesSize(0),
m_verticesSize(0),
m_normalsSize(0),
m_tangentsSize(0),
m_texCoordsSize(0),
m_colorsSize(0),

m_indicesType(M_UINT),
m_indices(NULL),

m_vertices(NULL),
m_normals(NULL),
m_tangents(NULL),
m_texCoords(NULL),
m_colors(NULL),

m_displaysNumber(0),
m_displays(NULL)
{}

MSubMesh::~MSubMesh(void)
{
    M_PROFILE_SCOPE(MSubMesh::~MSubMesh);
	clearIndices();
	clearVertices();
	clearNormals();
	clearTangents();
	clearTexCoords();
	clearColors();

	clearDisplays();

	clearMorphingData();
	clearSkinData();
}

void MSubMesh::clearMapChannelOffset(void)
{
    M_PROFILE_SCOPE(MSubMesh::clearMapChannelOffset);
	m_mapChannelsOffset.clear();
}

void MSubMesh::setMapChannelOffset(unsigned int channel, unsigned int offset)
{
    M_PROFILE_SCOPE(MSubMesh::setMapchannelOffset);
	m_mapChannelsOffset[channel] = offset;
}

bool MSubMesh::isMapChannelExist(unsigned int channel)
{
    M_PROFILE_SCOPE(MSubMesh::isMapChannelExist);
	map<unsigned int, unsigned int>::iterator iter = m_mapChannelsOffset.find(channel);
	return (iter != m_mapChannelsOffset.end());
}

unsigned int MSubMesh::getMapChannelOffset(unsigned int channel)
{
    M_PROFILE_SCOPE(MSubMesh::getMapChannelOffset);
	map<unsigned int, unsigned int>::iterator iter = m_mapChannelsOffset.find(channel);
	if(iter != m_mapChannelsOffset.end())
		return iter->second;
	return 0;
}

void MSubMesh::clearMorphingData(void)
{
    M_PROFILE_SCOPE(MSubMesh::clearMorphingData);
	SAFE_DELETE(m_morphingData);
}

MMorphingData * MSubMesh::createMorphingData(void)
{
    M_PROFILE_SCOPE(MSubMesh::createMorphingData);
	MMorphingData * morphingData = new MMorphingData();
	clearMorphingData();
	m_morphingData = morphingData;
	return morphingData;
}

void MSubMesh::clearSkinData(void)
{
    M_PROFILE_SCOPE(MSubMesh::clearSkinData);
	SAFE_DELETE(m_skinData);
}

MSkinData * MSubMesh::createSkinData(void)
{
    M_PROFILE_SCOPE(MSubMesh::createSkinData);
	MSkinData * skinData = new MSkinData();
	clearSkinData();
	m_skinData = skinData;
	return skinData;
}

void MSubMesh::clearIndices(void)
{
    M_PROFILE_SCOPE(MSubMesh::clearIndices);
	m_indicesSize = 0;
	SAFE_FREE(m_indices);
}

void MSubMesh::clearVertices(void)
{
    M_PROFILE_SCOPE(MSubMesh::clearVertices);
	m_verticesSize = 0;
	SAFE_DELETE_ARRAY(m_vertices);
}

void MSubMesh::clearNormals(void)
{
    M_PROFILE_SCOPE(MSubMesh::clearNormals);
	m_normalsSize = 0;
	SAFE_DELETE_ARRAY(m_normals);
}

void MSubMesh::clearTangents(void)
{
    M_PROFILE_SCOPE(MSubMesh::clearTangents);
	m_tangentsSize = 0;
	SAFE_DELETE_ARRAY(m_tangents);
}

void MSubMesh::clearTexCoords(void)
{
    M_PROFILE_SCOPE(MSubMesh::clearTexCoords);
	m_texCoordsSize = 0;
	SAFE_DELETE_ARRAY(m_texCoords);
}

void MSubMesh::clearColors(void)
{
    M_PROFILE_SCOPE(MSubMesh::clearColors);
	m_colorsSize = 0;
	SAFE_DELETE_ARRAY(m_colors);
}

void * MSubMesh::allocIndices(unsigned int size, M_TYPES type)
{
    M_PROFILE_SCOPE(MSubMesh::allocIndices);
	clearIndices();
	if(size == 0)
		return NULL;

	m_indicesSize = size;

	switch(type)
	{
	default:
	case M_UINT:
		m_indicesType = M_UINT;
		m_indices = malloc(sizeof(unsigned int)*size);
		break;
	case M_USHORT:
		m_indicesType = M_USHORT;
		m_indices = malloc(sizeof(unsigned short)*size);
		break;
	}
	
	return m_indices;
}

MVector3 * MSubMesh::allocVertices(unsigned int size)
{
    M_PROFILE_SCOPE(MSubMesh::allocVertices);
	clearVertices();
	if(size == 0)
		return NULL;

	m_verticesSize = size;
	m_vertices = new MVector3[size];

	return m_vertices;
}

MVector3 * MSubMesh::allocNormals(unsigned int size)
{
    M_PROFILE_SCOPE(MSubMesh::allocNormals);
	clearNormals();
	if(size == 0)
		return NULL;

	m_normalsSize = size;
	m_normals = new MVector3[size];

	return m_normals;
}

MVector3 * MSubMesh::allocTangents(unsigned int size)
{
    M_PROFILE_SCOPE(MSubMesh::allocTangents);
	clearTangents();
	if(size == 0)
		return NULL;
	
	m_tangentsSize = size;
	m_tangents = new MVector3[size];
	
	return m_tangents;
}

MVector2 * MSubMesh::allocTexCoords(unsigned int size)
{
    M_PROFILE_SCOPE(MSubMesh::allocTexCoords);
	clearTexCoords();
	if(size == 0)
		return NULL;

	m_texCoordsSize = size;
	m_texCoords = new MVector2[size];

	return m_texCoords;
}

MColor * MSubMesh::allocColors(unsigned int size)
{
    M_PROFILE_SCOPE(MSubMesh::allocColors);
	clearColors();
	if(size == 0)
		return NULL;

	m_colorsSize = size;
	m_colors = new MColor[size];

	return m_colors;
}

void MSubMesh::allocDisplays(unsigned int size)
{
    M_PROFILE_SCOPE(MSubMesh::allocDisplays);
	clearDisplays();
	if(size == 0)
		return;
	m_displays = (MDisplay **) malloc(sizeof(void *)*size);
}

void MSubMesh::clearDisplays(void)
{
    M_PROFILE_SCOPE(MSubMesh::clearDisplays);
	unsigned int i;
	for(i=0; i<m_displaysNumber; i++)
		SAFE_DELETE(m_displays[i]);

	m_displaysNumber = 0;
	SAFE_FREE(m_displays);
}

MDisplay * MSubMesh::addNewDisplay(M_PRIMITIVE_TYPES primitiveType, unsigned int begin, unsigned int size)
{
    M_PROFILE_SCOPE(MSubMesh::addNewDisplay);
	MDisplay * display = new MDisplay(primitiveType, begin, size);
	m_displays[m_displaysNumber] = display;
	m_displaysNumber++;
	return display;
}

bool MSubMesh::hasTransparency(void)
{
    M_PROFILE_SCOPE(MSubMesh::hasTransparency);
	unsigned int i;
	unsigned int displayNumber = getDisplaysNumber();
	for(i=0; i<displayNumber; i++)
	{
		MDisplay * display = getDisplay(i);

		if((! display->isVisible()) || (! display->getMaterial()))
			continue;

		if(display->getMaterial()->getBlendMode() != M_BLENDING_NONE)
			return true;
	}

	return false;
}


// MMesh
MMesh::MMesh(void):

m_animsRangesNumber(0),
m_animsRanges(NULL),

m_armatureAnimRef(NULL),
m_texturesAnimRef(NULL),
m_materialsAnimRef(NULL),

m_armature(NULL),

m_texturesNumber(0),
m_textures(NULL),

m_materialsNumber(0),
m_materials(NULL),

m_subMeshsNumber(0),
m_subMeshs(NULL)
{}

MMesh::~MMesh(void)
{
    M_PROFILE_SCOPE(MMesh::MMesh);
	clear();
}

MMesh * MMesh::getNew(void)
{
    M_PROFILE_SCOPE(MMesh::getNew);
	return new MMesh();
}

void MMesh::destroy(void)
{
    M_PROFILE_SCOPE(MMesh::destroy);
	delete this;
}

void MMesh::clear(void)
{
    M_PROFILE_SCOPE(MMesh::clear);
	clearArmature();
	clearTextures();
	clearMaterials();
	clearSubMeshs();
	clearAnimsRanges();
	
	m_armatureAnimRef = NULL;
	m_texturesAnimRef = NULL;
	m_materialsAnimRef = NULL;
}

void MMesh::clearAnimsRanges(void)
{
    M_PROFILE_SCOPE(MMesh::clearAnimsRanges);
	m_animsRangesNumber = 0;
	SAFE_DELETE_ARRAY(m_animsRanges);
}

MAnimRange * MMesh::allocAnimsRanges(unsigned int size)
{
    M_PROFILE_SCOPE(MMesh::allocAnimsRanges);
	clearAnimsRanges();
	if(size == 0)
		return NULL;

	m_animsRangesNumber = size;
	m_animsRanges = new MAnimRange[m_animsRangesNumber];
	return m_animsRanges;
}

MArmatureAnim * MMesh::getArmatureAnim(void)
{
    M_PROFILE_SCOPE(MMesh::getArmatureAnim);
	if(! m_armatureAnimRef)
		return NULL;

	return m_armatureAnimRef->getArmatureAnim();
}

MTexturesAnim * MMesh::getTexturesAnim(void)
{
    M_PROFILE_SCOPE(MMesh::getTexturesAnim);
	if(! m_texturesAnimRef)
		return NULL;

	return m_texturesAnimRef->getTexturesAnim();
}

MMaterialsAnim * MMesh::getMaterialsAnim(void)
{
    M_PROFILE_SCOPE(MMesh::getMaterialsAnim);
	if(! m_materialsAnimRef)
		return NULL;

	return m_materialsAnimRef->getMaterialsAnim();
}

void MMesh::clearArmature(void)
{
    M_PROFILE_SCOPE(MMesh::clearArmature);
	SAFE_DELETE(m_armature);
}

MArmature * MMesh::createArmature(void)
{
    M_PROFILE_SCOPE(MMesh::createArmature);
	clearArmature();
	m_armature = new MArmature();
	return m_armature;
}

void MMesh::allocTextures(unsigned int size)
{
    M_PROFILE_SCOPE(MMesh::allocTextures);
	clearTextures();
	if(size == 0)
		return;
	m_textures = (MTexture **) malloc(sizeof(void *)*size);
}

void MMesh::clearTextures(void)
{
    M_PROFILE_SCOPE(MMesh::clearTextures);
	unsigned int i;
	for(i=0; i<m_texturesNumber; i++)
		SAFE_DELETE(m_textures[i]);

	m_texturesNumber = 0;
	SAFE_FREE(m_textures);
}

MTexture * MMesh::addNewTexture(MTextureRef * texRef)
{
    M_PROFILE_SCOPE(MMesh::addNewTextures);
	MTexture * texture = new MTexture(texRef);
	m_textures[m_texturesNumber] = texture;
	m_texturesNumber++;
	return texture;
}

void MMesh::allocMaterials(unsigned int size)
{
    M_PROFILE_SCOPE(MMesh::allocMaterials);
	clearMaterials();
	if(size == 0)
		return;
	m_materials = (MMaterial **) malloc(sizeof(void *)*size);
}

void MMesh::clearMaterials(void)
{
    M_PROFILE_SCOPE(MMesh::clearMaterials);
	unsigned int i;
	for(i=0; i<m_materialsNumber; i++)
		SAFE_DELETE(m_materials[i]);

	m_materialsNumber = 0;
	SAFE_FREE(m_materials);
}

MMaterial * MMesh::addNewMaterial(void)
{
    M_PROFILE_SCOPE(MMesh::addNewMaterial);
	MMaterial * material = new MMaterial();
	m_materials[m_materialsNumber] = material;
	m_materialsNumber++;
	return material;
}

MSubMesh * MMesh::allocSubMeshs(unsigned int size)
{
    M_PROFILE_SCOPE(MMesh::allocSubMeshs);
	clearSubMeshs();
	if(size == 0)
		return NULL;

	m_subMeshsNumber = size;
	m_subMeshs = new MSubMesh[size];
	return m_subMeshs;
}

void MMesh::clearSubMeshs(void)
{
    M_PROFILE_SCOPE(MMesh::clearSubMeshs);
	m_subMeshsNumber = 0;
	SAFE_DELETE_ARRAY(m_subMeshs);
}

void MMesh::updateBoundingBox(void)
{
    M_PROFILE_SCOPE(MMesh::updateBoundingBox);
	if(m_subMeshsNumber > 0)
	{
		MVector3 * min = m_boundingBox.getMin();
		MVector3 * max = m_boundingBox.getMax();

		MBox3d * box = m_subMeshs[0].getBoundingBox();
		(*min) = *box->getMin();
		(*max) = *box->getMax();

		unsigned int i;
		for(i=1; i<m_subMeshsNumber; i++)
		{
			box = m_subMeshs[i].getBoundingBox();

			MVector3 * subMin = box->getMin();
			MVector3 * subMax = box->getMax();

			min->x = MIN(min->x, subMin->x);
			min->y = MIN(min->y, subMin->y);
			min->z = MIN(min->z, subMin->z);

			max->x = MAX(max->x, subMax->x);
			max->y = MAX(max->y, subMax->y);
			max->z = MAX(max->z, subMax->z);
		}
	}
}
