/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Maratis
// MScript.cpp
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


#include "MScript.h"

#include <list>
#include <string>

char g_currentDirectory[256] = "";
std::list<std::string> g_loadPath;

#ifdef WIN32
#define snprintf sprintf_s
#endif

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Some frequently used macros
/////////////////////////////////////////////////////////////////////////////////////////////////////////

#define GET_OBJECT_SUBCLASS_BEGIN(type_, var_, type_enum)	\
	MObject3d * object;	\
	lua_Integer id = lua_tointeger(L, 1);	\
	if((object = getObject3d(id)))	\
	{	\
		if(object->getType() == type_enum)	\
		{	\
			type_ * var_ = (type_*)object;

#define GET_OBJECT_SUBCLASS_END()	\
		}	\
	}

/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Global Functions
/////////////////////////////////////////////////////////////////////////////////////////////////////////

bool isFunctionOk(lua_State * L, const char * name, unsigned int nbArgs)
{
    M_PROFILE_SCOPE(lua-c:isFunctionOk);
	int nbArguments = lua_gettop(L);
	if(nbArguments < (int)nbArgs)
	{
		printf("ERROR script : \"%s\" need at least %d parameter(s)\n", name, nbArgs);
		return false;
	}
	return true;
}

void pushFloatArray(lua_State* L, float * values, unsigned int nbValues)
{
    M_PROFILE_SCOPE(lua-c:pushFloatArray);
	lua_newtable(L);

	for(unsigned int i=0; i<nbValues; i++)
	{
		lua_pushinteger(L, i+1);
		lua_pushnumber(L, values[i]);
		lua_rawset(L, -3);
	}
}

MObject3d * getObject3d(LUA_INTEGER object)
{
    M_PROFILE_SCOPE(lua-c:getObject3d);
	if(object == 0)
		return NULL;
	
	return (MObject3d*)object;
}

bool getVector2(lua_State * L, int index, MVector2 * vector)
{
    M_PROFILE_SCOPE(lua-c:getVector2);
	if(lua_istable(L, index) && (lua_objlen(L, index) >= 2))
	{
		lua_pushnil(L);

		lua_next(L, index);
		vector->x = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_next(L, index);
		vector->y = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);

		return true;
	}

	return false;
}

bool getVector3(lua_State * L, int index, MVector3 * vector)
{
    M_PROFILE_SCOPE(lua-c:getVector3);
	if(lua_istable(L, index) && (lua_objlen(L, index) >= 3))
	{
		lua_pushnil(L);

		lua_next(L, index);
		vector->x = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_next(L, index);
		vector->y = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);

		lua_next(L, index);
		vector->z = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);

		return true;
	}

	return false;
}

bool getVector4(lua_State * L, int index, MVector4 * vector)
{
    M_PROFILE_SCOPE(lua-c:getVector4);
	if(lua_istable(L, index) && (lua_objlen(L, index) >= 4))
	{
		lua_pushnil(L);
		
		lua_next(L, index);
		vector->x = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
		
		lua_next(L, index);
		vector->y = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
		
		lua_next(L, index);
		vector->z = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
		
		lua_next(L, index);
		vector->w = (float)lua_tonumber(L, -1);
		lua_pop(L, 1);
		
		return true;
	}
	
	return false;
}

int getScene(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getScene);
	MLevel * level = MEngine::getInstance()->getLevel();
	
	if(! isFunctionOk(L, "getScene", 1))
		return 0;
	
	const char * name = lua_tostring(L, 1);
	if(name)
	{
		unsigned int sceneIndex;
		if(level->getSceneIndexByName(name, &sceneIndex))
		{
			lua_pushinteger(L, (lua_Integer)sceneIndex);
			return 1;
		}
	}
	
	printf("ERROR script : scene \"%s\" doesn't exit\n", name);
	return 0;
}

int getObject(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getObject);
	MLevel * level = MEngine::getInstance()->getLevel();
	MScene * scene = level->getCurrentScene();
	

	if(! isFunctionOk(L, "getObject", 1))
		return 0;

	int nbArguments = lua_gettop(L);
	if(nbArguments == 2)
	{
		unsigned int sceneId = lua_tointeger(L, 1);
		scene = level->getSceneByIndex(sceneId);
	}
	
	const char * name = lua_tostring(L, nbArguments);
	if(name)
	{
		MObject3d * object = scene->getObjectByName(name);
		if(object)
		{
			lua_pushinteger(L, (lua_Integer)object);
			return 1;
		}
	}

	printf("ERROR script : object \"%s\" doesn't exit\n", name);
	return 0;
}

int getClone(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getClone);
	MLevel * level = MEngine::getInstance()->getLevel();
	MScene * scene = level->getCurrentScene();
	
	
	if(! isFunctionOk(L, "getClone", 1))
		return 0;
	
	int nbArguments = lua_gettop(L);
	if(nbArguments == 2)
	{
		unsigned int sceneId = lua_tointeger(L, 1);
		scene = level->getSceneByIndex(sceneId);
	}
	
	MObject3d * object;
	lua_Integer id = lua_tointeger(L, nbArguments);
	
	if((object = getObject3d(id)))
	{
		MObject3d * cloneObj = NULL;
		
		switch(object->getType())
		{
			case M_OBJECT3D_CAMERA:
				cloneObj = scene->addNewCamera(*(MOCamera*)object);
				break;
			case M_OBJECT3D_LIGHT:
				cloneObj = scene->addNewLight(*(MOLight*)object);
				break;
			case M_OBJECT3D_ENTITY:
				cloneObj = scene->addNewEntity(*(MOEntity*)object);
				scene->prepareCollisionObject((MOEntity*)cloneObj);
				break;
			case M_OBJECT3D_SOUND:
				cloneObj = scene->addNewSound(*(MOSound*)object);
				break;
			case M_OBJECT3D_TEXT:
				cloneObj = scene->addNewText(*(MOText*)object);
				break;
		}
		
		lua_pushinteger(L, (lua_Integer)cloneObj);
		return 1;
	}
	
	return 0;
}

int rotate(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:rotate);
	if(! isFunctionOk(L, "rotate", 3))
		return 0;

	int nbArguments = lua_gettop(L);

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		MVector3 axis;
		if(getVector3(L, 2, &axis))
		{
			// get angle
			float angle = (float)lua_tonumber(L, 3);

			// is local ?
			bool local = false;
			if(nbArguments > 3)
			{
				const char * mode = lua_tostring(L, 4);
				if(strcmp(mode, "local") == 0)
					local = true;
			}
			
			// rotate
			if(local)
				object->addAxisAngleRotation(axis, angle);
			else
				object->addAxisAngleRotation(object->getInverseRotatedVector(axis), angle);
		}
	}

	return 0;
}

int translate(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:translate);
	if(! isFunctionOk(L, "translate", 2))
		return 0;

	int nbArguments = lua_gettop(L);

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		MVector3 axis;
		if(getVector3(L, 2, &axis))
		{
			// is local ?
			bool local = false;
			if(nbArguments > 2)
			{
				const char * mode = lua_tostring(L, 3);
				if(strcmp(mode, "local") == 0)
					local = true;
			}

			// rotate
			if(local)
			{
				axis = object->getRotatedVector(axis);
				object->setPosition(object->getPosition() + axis);
			}
			else
				object->setPosition(object->getPosition() + axis);
		}
	}

	return 0;
}

int getPosition(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getPosition);
	if(! isFunctionOk(L, "getPosition", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		pushFloatArray(L, object->getPosition(), 3);
		return 1;
	}

	return 0;
}

int getRotation(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getRotation);
	if(! isFunctionOk(L, "getRotation", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		pushFloatArray(L, object->getEulerRotation(), 3);
		return 1;
	}

	return 0;
}

int getScale(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getScale);
	if(! isFunctionOk(L, "getScale", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		pushFloatArray(L, object->getScale(), 3);
		return 1;
	}

	return 0;
}

int setPosition(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setPosition);
	if(! isFunctionOk(L, "setPosition", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		MVector3 position;
		if(getVector3(L, 2, &position))
		{
			object->setPosition(position);
		}
	}

	return 0;
}

int setRotation(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setRotation);
	if(! isFunctionOk(L, "setRotation", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		MVector3 rotation;
		if(getVector3(L, 2, &rotation))
		{
			object->setEulerRotation(rotation);
		}
	}

	return 0;
}

int setScale(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setScale);
	if(! isFunctionOk(L, "setScale", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		MVector3 scale;
		if(getVector3(L, 2, &scale))
		{
			object->setScale(scale);
		}
	}

	return 0;
}

int isVisible(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:isVisible);
	if(! isFunctionOk(L, "isVisible", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		int vis = object->isVisible();
		lua_pushboolean(L, vis);
		return 1;
	}

	return 0;
}

int activate(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:activate);
	if(! isFunctionOk(L, "activate", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		object->setActive(true);
		return 0;
	}

	return 0;
}

int deactivate(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:deactivate);
	if(! isFunctionOk(L, "deactivate", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		object->setActive(false);
		return 0;
	}

	return 0;
}

int changeAnimation(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:changeAnimation);
	if(! isFunctionOk(L, "changeAnimation", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_ENTITY)
		{
			MOEntity * entity = (MOEntity*)object;

			unsigned int anim = (unsigned int)lua_tointeger(L, 2);
			entity->changeAnimation(anim);
			return 0;
		}
	}

	return 0;
}

int isAnimationOver(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:isAnimationOver);
	if(! isFunctionOk(L, "isAnimationOver", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_ENTITY)
		{
			MOEntity * entity = (MOEntity*)object;
			int isAnimOver = entity->isAnimationOver();
			lua_pushboolean(L, isAnimOver);
			return 1;
		}
	}

	return 0;
}

int getCurrentAnimation(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getCurrentAnimation);
	if(! isFunctionOk(L, "getCurrentAnimation", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_ENTITY)
		{
			MOEntity * entity = (MOEntity*)object;
			int anim = (int)entity->getAnimationId();
			lua_pushinteger(L, anim);
			return 1;
		}
	}

	return 0;
}

int setAnimationSpeed(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setAnimationSpeed);
	if(! isFunctionOk(L, "setAnimationSpeed", 2))
		return 0;

	GET_OBJECT_SUBCLASS_BEGIN(MOEntity, entity, M_OBJECT3D_ENTITY)
			entity->setAnimationSpeed((float)lua_tonumber(L, 2));
			return 0;
	GET_OBJECT_SUBCLASS_END()

	return 0;
}

int getAnimationSpeed(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getAnimationSpeed);
	if(! isFunctionOk(L, "getAnimationSpeed", 1))
		return 0;

	GET_OBJECT_SUBCLASS_BEGIN(MOEntity, entity, M_OBJECT3D_ENTITY)
			lua_pushnumber(L, (lua_Number)entity->getAnimationSpeed());
			return 1;
	GET_OBJECT_SUBCLASS_END()

	return 0;
}

int setCurrentFrame(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setCurrentFrame);
	if(! isFunctionOk(L, "setCurrentFrame", 2))
		return 0;

	GET_OBJECT_SUBCLASS_BEGIN(MOEntity, entity, M_OBJECT3D_ENTITY)
			entity->setCurrentFrame((float)lua_tonumber(L, 2));
			return 0;
	GET_OBJECT_SUBCLASS_END()

	return 0;
}

int getCurrentFrame(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getCurrentFrame);
	if(! isFunctionOk(L, "getCurrentFrame", 1))
		return 0;

	GET_OBJECT_SUBCLASS_BEGIN(MOEntity, entity, M_OBJECT3D_ENTITY)
			lua_pushnumber(L, (lua_Number)entity->getCurrentFrame());
			return 1;
	GET_OBJECT_SUBCLASS_END()

	return 0;
}

int getGravity(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getGravity);
	MLevel * level = MEngine::getInstance()->getLevel();
	MScene * scene = level->getCurrentScene();
	
	int nbArguments = lua_gettop(L);
	if(nbArguments == 1)
	{
		unsigned int sceneId = lua_tointeger(L, 1);
		scene = level->getSceneByIndex(sceneId);
	}
	
	pushFloatArray(L, *scene->getGravity(), 3);

	return 1;
}

int setGravity(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setGravity);
	MLevel * level = MEngine::getInstance()->getLevel();
	MScene * scene = level->getCurrentScene();
	
	if(! isFunctionOk(L, "setGravity", 1))
		return 0;

	int nbArguments = lua_gettop(L);
	if(nbArguments == 2)
	{
		unsigned int sceneId = lua_tointeger(L, 1);
		scene = level->getSceneByIndex(sceneId);
	}
	
	MVector3 gravity;
	if(getVector3(L, nbArguments, &gravity))
		scene->setGravity(gravity);

	return 0;
}

int changeCurrentCamera(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:changeCurrentCamera);
	MLevel * level = MEngine::getInstance()->getLevel();
	MScene * scene = level->getCurrentScene();
	
	if(! isFunctionOk(L, "changeCurrentCamera", 1))
		return 0;

	int nbArguments = lua_gettop(L);
	if(nbArguments == 2)
	{
		unsigned int sceneId = lua_tointeger(L, 1);
		scene = level->getSceneByIndex(sceneId);
	}
	
	lua_Integer id = lua_tointeger(L, nbArguments);
	MObject3d * object = getObject3d(id);
	if(object)
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			unsigned int i;
			unsigned int size = scene->getCamerasNumber();
			for(i=0; i<size; i++)
			{
				if(object == (MObject3d *)scene->getCameraByIndex(i))
				{
					scene->setCurrentCamera(i);
					break;
				}
			}
		}
	}

	return 0;
}

int addCentralForce(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:addCentralForce);
	int nbArguments = lua_gettop(L);
	if(! isFunctionOk(L, "addCentralForce", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_ENTITY)
		{
			MVector3 force;
			if(getVector3(L, 2, &force))
			{
				// is local ?
				bool local = false;
				if(nbArguments > 2)
				{
					const char * mode = lua_tostring(L, 3);
					if(strcmp(mode, "local") == 0)
						local = true;
				}

				MOEntity * entity = (MOEntity*)object;
				MPhysicsProperties * phyProps = entity->getPhysicsProperties();
				if(phyProps)
				{
					if(local)
						force = object->getRotatedVector(force);

					MPhysicsContext * physics = MEngine::getInstance()->getPhysicsContext();
					physics->addCentralForce(phyProps->getCollisionObjectId(), force);
				}
			}
		}
	}

	return 0;
}

int clearForces(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:clearForces);
	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_ENTITY)
		{
			MOEntity * entity = (MOEntity*)object;
			MPhysicsProperties * phyProps = entity->getPhysicsProperties();
			if(phyProps)
			{
				MPhysicsContext * physics = MEngine::getInstance()->getPhysicsContext();
				physics->clearForces(phyProps->getCollisionObjectId());
			}
		}
	}

	return 0;
}

int addTorque(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:addTorque);
	int nbArguments = lua_gettop(L);
	if(! isFunctionOk(L, "addTorque", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_ENTITY)
		{
			MVector3 torque;
			if(getVector3(L, 2, &torque))
			{
				// is local ?
				bool local = false;
				if(nbArguments > 2)
				{
					const char * mode = lua_tostring(L, 3);
					if(strcmp(mode, "local") == 0)
						local = true;
				}

				MOEntity * entity = (MOEntity*)object;
				MPhysicsProperties * phyProps = entity->getPhysicsProperties();
				if(phyProps)
				{
					if(local)
						torque = object->getRotatedVector(torque);

					MPhysicsContext * physics = MEngine::getInstance()->getPhysicsContext();
					physics->addTorque(phyProps->getCollisionObjectId(), torque);
				}
			}
		}
	}

	return 0;
}

int getLinearDamping(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getLinearDamping);
	if(! isFunctionOk(L, "getLinearDamping", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);
	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_ENTITY)
		{
			MOEntity * entity = (MOEntity*)object;
			MPhysicsProperties * phyProps = entity->getPhysicsProperties();
			if(phyProps)
			{
				float linearDamping = phyProps->getLinearDamping();
				lua_pushnumber(L, (lua_Number)linearDamping);
				return 1;
			}
		}
	}

	return 0;
}

int setLinearDamping(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setLinearDamping);
	MPhysicsContext * physics = MEngine::getInstance()->getPhysicsContext();

	if(! isFunctionOk(L, "setLinearDamping", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);
	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_ENTITY)
		{
			MOEntity * entity = (MOEntity*)object;
			MPhysicsProperties * phyProps = entity->getPhysicsProperties();
			if(phyProps)
			{
				float linearDamping = (float)lua_tonumber(L, 2);
				phyProps->setLinearDamping(linearDamping);
				physics->setObjectDamping(
					phyProps->getCollisionObjectId(),
					phyProps->getLinearDamping(),
					phyProps->getAngularDamping()
					);
				return 0;
			}
		}
	}

	return 0;
}

int getAngularDamping(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getAngularDamping);
	if(! isFunctionOk(L, "getAngularDamping", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);
	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_ENTITY)
		{
			MOEntity * entity = (MOEntity*)object;
			MPhysicsProperties * phyProps = entity->getPhysicsProperties();
			if(phyProps)
			{
				float angularDamping = phyProps->getAngularDamping();
				lua_pushnumber(L, (lua_Number)angularDamping);
				return 1;
			}
		}
	}

	return 0;
}

int setAngularDamping(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setAngularDamping);
	MPhysicsContext * physics = MEngine::getInstance()->getPhysicsContext();

	if(! isFunctionOk(L, "setAngularDamping", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);
	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_ENTITY)
		{
			MOEntity * entity = (MOEntity*)object;
			MPhysicsProperties * phyProps = entity->getPhysicsProperties();
			if(phyProps)
			{
				float angularDamping = (float)lua_tonumber(L, 2);
				phyProps->setAngularDamping(angularDamping);
				physics->setObjectDamping(
					phyProps->getCollisionObjectId(),
					phyProps->getLinearDamping(),
					phyProps->getAngularDamping()
					);
				return 0;
			}
		}
	}

	return 0;
}

int getNumCollisions(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getNumCollisions);
	if(! isFunctionOk(L, "getNumCollisions", 1))
		return 0;
	
	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);
	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_ENTITY)
		{
			MOEntity * entity = (MOEntity*)object;
			MPhysicsProperties * phyProps = entity->getPhysicsProperties();
			if(phyProps)
			{
				MPhysicsContext * physics = MEngine::getInstance()->getPhysicsContext();
				int collision = physics->isObjectInCollision(phyProps->getCollisionObjectId());
				lua_pushinteger(L, collision);
				return 1;
			}
		}
	}
	
	return 0;
}

int isCollisionTest(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:isCollisionTest);
	if(! isFunctionOk(L, "isCollisionTest", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);
	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_ENTITY)
		{
			MOEntity * entity = (MOEntity*)object;
			MPhysicsProperties * phyProps = entity->getPhysicsProperties();
			if(phyProps)
			{
				MPhysicsContext * physics = MEngine::getInstance()->getPhysicsContext();
				int collision = MIN(1, physics->isObjectInCollision(phyProps->getCollisionObjectId()));
				lua_pushboolean(L, collision);
				return 1;
			}
		}
	}

	return 0;
}

int isCollisionBetween(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:isCollisionBetween);
	if(! isFunctionOk(L, "isCollisionBetween", 2))
		return 0;

	MObject3d * object1;
	MObject3d * object2;

	lua_Integer id1 = lua_tointeger(L, 1);
	lua_Integer id2 = lua_tointeger(L, 2);
	if((object1 = getObject3d(id1)) && (object2 = getObject3d(id2)))
	{
		if((object1->getType() == M_OBJECT3D_ENTITY) && (object2->getType() == M_OBJECT3D_ENTITY))
		{
			MOEntity * entity1 = (MOEntity*)object1;
			MOEntity * entity2 = (MOEntity*)object2;

			MPhysicsProperties * phyProps1 = entity1->getPhysicsProperties();
			MPhysicsProperties * phyProps2 = entity2->getPhysicsProperties();
			if(phyProps1 && phyProps2)
			{
				MPhysicsContext * physics = MEngine::getInstance()->getPhysicsContext();
				int collision = physics->isObjectsCollision(
					phyProps1->getCollisionObjectId(),
					phyProps2->getCollisionObjectId()
					);
				lua_pushboolean(L, collision);
				return 1;
			}
		}
	}

	return 0;
}

int rayHit(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:rayHit);
	if(! isFunctionOk(L, "rayHit", 2))
		return 0;
	
	int nbArguments = lua_gettop(L);
	
	MVector3 start, end;
	if(getVector3(L, 1, &start) && getVector3(L, 2, &end))
	{
		MPhysicsContext * physics = MEngine::getInstance()->getPhysicsContext();
		
		unsigned int collisionObjId;
		MVector3 point;
		
		// ray test
		if(physics->isRayHit(start, end, &collisionObjId, &point))
		{
			if(nbArguments == 3)
			{
				MObject3d * object;
				lua_Integer id = lua_tointeger(L, 1);
				if((object = getObject3d(id)))
				{
					if(object->getType() == M_OBJECT3D_ENTITY)
					{
						MOEntity * entity = (MOEntity*)object;
						MPhysicsProperties * phyProps = entity->getPhysicsProperties();
						if(phyProps)
						{
							if(phyProps->getCollisionObjectId() == collisionObjId)
							{
								pushFloatArray(L, point, 3);
								return 1;
							}
						}
					}
				}
			}
			else
			{
				pushFloatArray(L, point, 3);
				return 1;
			}
		}
	}
	
	return 0;
}

int getRayHitObject(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getRayHitObject);
	if(! isFunctionOk(L, "rayHit", 2))
		return 0;
	
	int nbArguments = lua_gettop(L);
	
	MVector3 start, end;
	if(getVector3(L, 1, &start) && getVector3(L, 2, &end))
	{
		MPhysicsContext * physics = MEngine::getInstance()->getPhysicsContext();
		
		unsigned int collisionObjId;
		MVector3 point;
		
		// ray test
		if(physics->isRayHit(start, end, &collisionObjId, &point))
		{
		    
		}
	}
	return 0;
}

int isKeyPressed(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:isKeyPressed);
	MInputContext * input = MEngine::getInstance()->getInputContext();

	if(! isFunctionOk(L, "isKeyPressed", 1))
		return 0;

	const char * name = lua_tostring(L, 1);
	if(name)
	{
		int keyPressed = input->isKeyPressed(name);
		lua_pushboolean(L, keyPressed);
		return 1;
	}

	return 0;
}

int onKeyDown(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:onKeyDown);
	MInputContext * input = MEngine::getInstance()->getInputContext();

	if(! isFunctionOk(L, "onKeyDown", 1))
		return 0;

	const char * name = lua_tostring(L, 1);
	if(name)
	{
		int keyDown = input->onKeyDown(name);
		lua_pushboolean(L, keyDown);
		return 1;
	}

	return 0;
}

int onKeyUp(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:onKeyUp);
	MInputContext * input = MEngine::getInstance()->getInputContext();

	if(! isFunctionOk(L, "onKeyUp", 1))
		return 0;

	const char * name = lua_tostring(L, 1);
	if(name)
	{
		int keyUp = input->onKeyUp(name);
		lua_pushboolean(L, keyUp);
		return 1;
	}

	return 0;
}

int getAxis(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getAxis);
	MInputContext * input = MEngine::getInstance()->getInputContext();

	if(! isFunctionOk(L, "getAxis", 1))
		return 0;

	const char * name = lua_tostring(L, 1);
	if(name)
	{
		float axis = input->getAxis(name);
		lua_pushnumber(L, axis);
		return 1;
	}

	return 0;
}

int getProperty(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getProperty);
	MInputContext * input = MEngine::getInstance()->getInputContext();

	if(! isFunctionOk(L, "getProperty", 1))
		return 0;

	const char * name = lua_tostring(L, 1);
	if(name)
	{
		int prop = input->getProperty(name);
		lua_pushinteger(L, prop);
		return 1;
	}

	return 0;
}

int getVectorProperty(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getVectorProperty);
    MInputContext * input = MEngine::getInstance()->getInputContext();
    
    if (!isFunctionOk(L, "getVectorProperty", 1))
        return 0;
    
    const char* name = lua_tostring(L, 1);
    if (name)
    {
        pushFloatArray(L, input->getVectorProperty(name), 3);
        return 1;
    }
    
    return 0;
}

int getTouchPosition(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getTouchPosition);
    MInputContext * input = MEngine::getInstance()->getInputContext();
    
    if (!isFunctionOk(L, "getTouchPosition", 1))
        return 0;
    
    unsigned int touchID = (unsigned int)lua_tointeger(L, 1);
    pushFloatArray(L, input->getTouchPosition(touchID), 2);
    
    return 1;
}

int getLastTouchPosition(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getLastTouchPosition);
    MInputContext * input = MEngine::getInstance()->getInputContext();
    
    if (!isFunctionOk(L, "getLastTouchPosition", 1))
        return 0;
    
    unsigned int touchID = (unsigned int)lua_tointeger(L, 1);
    pushFloatArray(L, input->getLastTouchPosition(touchID), 2);
    
    return 1;
}

int getTouchPhase(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getTouchPhase);
    MInputContext * input = MEngine::getInstance()->getInputContext();
    
    if (!isFunctionOk(L, "getTouchPhase", 1))
        return 0;
    
    unsigned int touchID = (unsigned int)lua_tointeger(L, 1);
    lua_pushinteger(L, input->getTouchPhase(touchID));
    
    return 1;
}

int playSound(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:playSound);
	if(! isFunctionOk(L, "playSound", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_SOUND)
		{
			MOSound * sound = (MOSound*)object;
			sound->play();
		}
	}

	return 0;
}

int pauseSound(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:pauseSound);
	if(! isFunctionOk(L, "pauseSound", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_SOUND)
		{
			MOSound * sound = (MOSound*)object;
			sound->pause();
		}
	}

	return 0;
}

int stopSound(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:stopSound);
	if(! isFunctionOk(L, "stopSound", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_SOUND)
		{
			MOSound * sound = (MOSound*)object;
			sound->stop();
		}
	}

	return 0;
}

int changeScene(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:changeScene);
	MEngine * engine = MEngine::getInstance();
	MLevel * level = engine->getLevel();

	if(! isFunctionOk(L, "changeScene", 1))
		return 0;

	lua_Integer id = lua_tointeger(L, 1);
	level->changeCurrentScene(id);
	
	return 0;
}

int getCurrentSceneId(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getCurrentSceneId);
	MEngine * engine = MEngine::getInstance();
	MLevel * level = engine->getLevel();

	lua_pushinteger(L, (int)level->getCurrentSceneId());
	return 1;
}

int getScenesNumber(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getScenesNumber);
	MEngine * engine = MEngine::getInstance();
	MLevel * level = engine->getLevel();

	lua_pushinteger(L, level->getScenesNumber());
	return 1;
}

int loadLevel(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:loadLevel);
	MEngine * engine = MEngine::getInstance();

	if(! isFunctionOk(L, "loadLevel", 1))
		return 0;

	const char * filename = lua_tostring(L, 1);
	if(filename)
		engine->requestLoadLevel(filename);

	return 0;
}

int quit(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:quit);
	MEngine * engine = MEngine::getInstance();
	engine->setActive(false);
	return 0;
}

int doesLevelExist(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:doesLevelExist);
	MEngine * engine = MEngine::getInstance();

	if(! isFunctionOk(L, "doesLevelExist", 1))
		return 0;

	const char * filename = lua_tostring(L, 1);

	lua_pushnumber(L, (lua_Number)engine->doesLevelExist(filename));
	return 1;
}

int getLightColor(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getLightColor);
	if(! isFunctionOk(L, "getLightColor", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_LIGHT)
		{
			MOLight * light = (MOLight*)object;
			pushFloatArray(L, *light->getColor(), 3);
			return 1;
		}
	}

	return 0;
}

int getLightRadius(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getLightRadius);
	if(! isFunctionOk(L, "getLightRadius", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_LIGHT)
		{
			MOLight * light = (MOLight*)object;
			lua_pushnumber(L, (lua_Number)light->getRadius());
			return 1;
		}
	}

	return 0;
}

int getLightIntensity(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getLightIntensity);
	if(! isFunctionOk(L, "getLightIntensity", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_LIGHT)
		{
			MOLight * light = (MOLight*)object;
			lua_pushnumber(L, (lua_Number)light->getIntensity());
			return 1;
		}
	}

	return 0;
}

int setLightColor(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setLightColor);
	if(! isFunctionOk(L, "setLightColor", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		MVector3 color;
		if(object->getType() == M_OBJECT3D_LIGHT && getVector3(L, 2, &color))
		{
			MOLight * light = (MOLight*)object;
			light->setColor(color);
			return 0;
		}
	}

	return 0;
}

int setLightRadius(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setLightRadius);
	if(! isFunctionOk(L, "setLightRadius", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_LIGHT)
		{
			float radius = (float)lua_tonumber(L, 2);

			MOLight * light = (MOLight*)object;
			light->setRadius(radius);
			return 0;
		}
	}

	return 0;
}

int setLightIntensity(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setLightIntensity);
	if(! isFunctionOk(L, "setLightIntensity", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_LIGHT)
		{
			float intensity = (float)lua_tonumber(L, 2);

			MOLight * light = (MOLight*)object;
			light->setIntensity(intensity);
			return 0;
		}
	}

	return 0;
}

int getSoundGain(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getSoundGain);
	if(! isFunctionOk(L, "getSoundGain", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_SOUND)
		{
			MOSound * sound = (MOSound*)object;
			lua_pushnumber(L, (lua_Number)sound->getGain());
			return 1;
		}
	}

	return 0;
}

int setSoundGain(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setSoundGain);
	if(! isFunctionOk(L, "setSoundGain", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_SOUND)
		{
			float gain = (float)lua_tonumber(L, 2);
			MOSound * sound = (MOSound*)object;
			sound->setGain(gain);
			return 0;
		}
	}

	return 0;
}

int setCameraClearColor(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setCameraClearColor);
	if(! isFunctionOk(L, "setCameraClearColor", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		MVector3 color;
		if(object->getType() == M_OBJECT3D_CAMERA && getVector3(L, 2, &color))
		{
			MOCamera * camera = (MOCamera*)object;
			camera->setClearColor(color);
			return 0;
		}
	}

	return 0;
}

int getCameraClearColor(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getCameraClearColor);
	if(! isFunctionOk(L, "getCameraClearColor", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			MOCamera * camera = (MOCamera*)object;
			pushFloatArray(L, *camera->getClearColor(), 3);
			return 1;
		}
	}

	return 0;
}

int setCameraNear(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setCameraNear);
	if(! isFunctionOk(L, "setCameraNear", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			float clipNear = (float)lua_tonumber(L, 2);
			MOCamera * camera = (MOCamera*)object;
			camera->setClippingNear(clipNear);
			return 0;
		}
	}

	return 0;
}

int getCameraNear(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getCameraNear);
	if(! isFunctionOk(L, "getCameraNear", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			MOCamera * camera = (MOCamera*)object;
			lua_pushnumber(L, (lua_Number)camera->getClippingNear());
			return 1;
		}
	}

	return 0;
}

int setCameraFar(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setCameraFar);
	if(! isFunctionOk(L, "setCameraFar", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			float clipFar = (float)lua_tonumber(L, 2);
			MOCamera * camera = (MOCamera*)object;
			camera->setClippingFar(clipFar);
			return 0;
		}
	}

	return 0;
}

int getCameraFar(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getCameraFar);
	if(! isFunctionOk(L, "getCameraFar", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			MOCamera * camera = (MOCamera*)object;
			lua_pushnumber(L, (lua_Number)camera->getClippingFar());
			return 1;
		}
	}

	return 0;
}

int setCameraFov(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setCameraFov);
	if(! isFunctionOk(L, "setCameraFov", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			float fov = (float)lua_tonumber(L, 2);
			MOCamera * camera = (MOCamera*)object;
			camera->setFov(fov);
			return 0;
		}
	}

	return 0;
}

int getCameraFov(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getCameraFov);
	if(! isFunctionOk(L, "getCameraFov", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			MOCamera * camera = (MOCamera*)object;
			lua_pushnumber(L, (lua_Number)camera->getFov());
			return 1;
		}
	}

	return 0;
}

int setCameraFogDistance(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setCameraFogDistance);
	if(! isFunctionOk(L, "setCameraFogDistance", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			float fogDist = (float)lua_tonumber(L, 2);
			MOCamera * camera = (MOCamera*)object;
			camera->setFogDistance(fogDist);
			return 0;
		}
	}

	return 0;
}

int getCameraFogDistance(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getCameraFogDistance);
	if(! isFunctionOk(L, "getCameraFogDistance", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			MOCamera * camera = (MOCamera*)object;
			lua_pushnumber(L, (lua_Number)camera->getFogDistance());
			return 1;
		}
	}

	return 0;
}

int enableCameraOrtho(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:enableCameraOrtho);
	if(! isFunctionOk(L, "enableCameraOrtho", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			bool ortho = lua_toboolean(L, 2) == 1;
			MOCamera * camera = (MOCamera*)object;
			camera->enableOrtho(ortho);
			return 0;
		}
	}

	return 0;
}

int isCameraOrtho(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:isCameraOrtho);
	if(! isFunctionOk(L, "isCameraOrtho", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			MOCamera * camera = (MOCamera*)object;
			lua_pushboolean(L, camera->isOrtho());
			return 1;
		}
	}

	return 0;
}

int enableCameraFog(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:enableCameraFog);
	if(! isFunctionOk(L, "enableCameraFog", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			bool fog = lua_toboolean(L, 2) == 1;
			MOCamera * camera = (MOCamera*)object;
			camera->enableFog(fog);
			return 0;
		}
	}

	return 0;
}

int isCameraFogEnabled(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:isCameraFogEnabled);
	if(! isFunctionOk(L, "isCameraFogEnabled", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			MOCamera * camera = (MOCamera*)object;
			lua_pushboolean(L, camera->hasFog());
			return 1;
		}
	}

	return 0;
}

int enableCameraLayer(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:enableCameraLayer);
	if(! isFunctionOk(L, "enableCameraLayer", 2))
		return 0;
	
	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);
	
	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			unsigned int sceneId = (unsigned int)lua_tointeger(L, 2);
			MOCamera * camera = (MOCamera*)object;
			camera->setSceneLayer(sceneId+1);
			return 0;
		}
	}
	
	return 0;
}

int disableCameraLayer(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:disableCameraLayer);
	if(! isFunctionOk(L, "disableCameraLayer", 1))
		return 0;
	
	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);
	
	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			MOCamera * camera = (MOCamera*)object;
			camera->setSceneLayer(0);
			return 0;
		}
	}
	
	return 0;
}

int enableRenderToTexture(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:enableRenderToTexture);
	MEngine * engine = MEngine::getInstance();
	MSystemContext * system = engine->getSystemContext();
	MRenderingContext * render = engine->getRenderingContext();
	MLevel * level = engine->getLevel();
	
	if(! isFunctionOk(L, "enableRenderToTexture", 4))
		return 0;
	
	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);
	
	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			const char * name = lua_tostring(L, 2);
			if(name)
			{
				MOCamera * camera = (MOCamera *)object;
				
				unsigned int width = (unsigned int)lua_tointeger(L, 3);
				unsigned int height = (unsigned int)lua_tointeger(L, 4);
				
				char globalFilename[256];
				getGlobalFilename(globalFilename, system->getWorkingDirectory(), name);
				
				MTextureRef * colorTexture = level->loadTexture(globalFilename, 0, 0);
				MTextureRef * depthTexture = level->loadTexture(object->getName(), 0, 0);
				
				colorTexture->clear();
				depthTexture->clear();
				
				camera->setRenderColorTexture(colorTexture);
				camera->setRenderDepthTexture(depthTexture);
				
				unsigned int m_colorTextureId, m_depthTextureId;
				
				render->createTexture(&m_colorTextureId);
				render->bindTexture(m_colorTextureId);
				render->setTextureFilterMode(M_TEX_FILTER_LINEAR, M_TEX_FILTER_LINEAR);
				render->setTextureUWrapMode(M_WRAP_CLAMP);
				render->setTextureVWrapMode(M_WRAP_CLAMP);
				render->texImage(0, width, height, M_UBYTE, M_RGBA, 0);
				
				render->createTexture(&m_depthTextureId);
				render->bindTexture(m_depthTextureId);
				render->setTextureFilterMode(M_TEX_FILTER_LINEAR, M_TEX_FILTER_LINEAR);
				render->setTextureUWrapMode(M_WRAP_CLAMP);
				render->setTextureVWrapMode(M_WRAP_CLAMP);
				render->texImage(0, width, height, M_UINT, M_DEPTH, 0);
				render->bindTexture(0);
				
				colorTexture->setTextureId(m_colorTextureId);
				colorTexture->setWidth(width);
				colorTexture->setHeight(height);
				depthTexture->setTextureId(m_depthTextureId);
				depthTexture->setWidth(width);
				depthTexture->setHeight(height);
			}
			
			return 0;
		}
	}
	
	return 0;
}

int disableRenderToTexture(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:disableRenderToTexture);
	if(! isFunctionOk(L, "disableRenderToTexture", 1))
		return 0;
	
	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);
	
	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_CAMERA)
		{
			MOCamera * camera = (MOCamera *)object;
				
			MTextureRef * depthTexture = camera->getRenderDepthTexture();
			if(depthTexture)
				depthTexture->clear();
				
			camera->setRenderColorTexture(NULL);
			camera->setRenderDepthTexture(NULL);
			
			return 0;
		}
	}
	
	return 0;
}

int getScreenSize(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getScreenSize);
    if(! isFunctionOk(L, "getScreenSize", 0))
	return 0;

    unsigned int w, h;
    MEngine::getInstance()->getSystemContext()->getScreenSize(&w, &h);
    lua_pushinteger(L, (lua_Integer)w);
    lua_pushinteger(L, (lua_Integer)h);
    return 2;
}

int getBehaviorVariable(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getBehaviorVariable);
	if(! isFunctionOk(L, "getBehaviorVariable", 3))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		unsigned int bId = (unsigned int)lua_tointeger(L, 2);
		const char * varName = lua_tostring(L, 3);

		if(varName)
		{
			unsigned bSize = object->getBehaviorsNumber();
			if(bId < bSize)
			{
				MBehavior * behavior = object->getBehavior(bId);
				unsigned int v;
				unsigned int vSize = behavior->getVariablesNumber();
				for(v=0; v<vSize; v++)
				{
					MVariable variable = behavior->getVariable(v);
					if(strcmp(variable.getName(), varName) == 0)
					{
						switch(variable.getType())
						{
						case M_VARIABLE_BOOL:
							{
								lua_pushboolean(L, *(bool *)variable.getPointer());
								return 1;
							}
						case M_VARIABLE_INT:
						case M_VARIABLE_UINT:
							{
								lua_pushinteger(L, *(int *)variable.getPointer());
								return 1;
							}
						case M_VARIABLE_FLOAT:
							{
								lua_pushnumber(L, *(float *)variable.getPointer());
								return 1;
							}
						case M_VARIABLE_STRING:
							{
								lua_pushstring(L, ((MString *)variable.getPointer())->getData());
								return 1;
							}
						case M_VARIABLE_VEC2:
							{
								pushFloatArray(L, *(MVector2 *)variable.getPointer(), 2);
								return 1;
							}
						case M_VARIABLE_VEC3:
							{
								pushFloatArray(L, *(MVector3 *)variable.getPointer(), 3);
								return 1;
							}
						case M_VARIABLE_VEC4:
							{
								pushFloatArray(L, *(MVector4 *)variable.getPointer(), 4);
								return 1;
							}
						}
					}
				}
			}
		}
	}

	return 0;
}

int setBehaviorVariable(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setBehaviorVariable);
	if(! isFunctionOk(L, "setBehaviorVariable", 4))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		unsigned int bId = (unsigned int)lua_tointeger(L, 2);
		const char * varName = lua_tostring(L, 3);

		if(varName)
		{
			unsigned bSize = object->getBehaviorsNumber();
			if(bId < bSize)
			{
				MBehavior * behavior = object->getBehavior(bId);
				unsigned int v;
				unsigned int vSize = behavior->getVariablesNumber();
				for(v=0; v<vSize; v++)
				{
					MVariable variable = behavior->getVariable(v);
					if(strcmp(variable.getName(), varName) == 0)
					{
						switch(variable.getType())
						{
						case M_VARIABLE_BOOL:
							{
								bool val = lua_toboolean(L, 4) == 1;
								*(bool *)variable.getPointer() = val;
								return 0;
							}
						case M_VARIABLE_INT:
						case M_VARIABLE_UINT:
							{
								int val = lua_tointeger(L, 4);
								*(int *)variable.getPointer() = val;
								return 0;
							}
						case M_VARIABLE_FLOAT:
							{
								float val = (float)lua_tonumber(L, 4);
								*(float *)variable.getPointer() = val;
								return 0;
							}
						case M_VARIABLE_STRING:
							{
								const char * str = lua_tostring(L, 4);
								if(str)
									((MString *)variable.getPointer())->set(str);
								return 0;
							}
						case M_VARIABLE_VEC2:
							{
								MVector2 vec;
								if(getVector2(L, 2, &vec))
									*(MVector2 *)variable.getPointer() = vec;
								return 0;
							}
						case M_VARIABLE_VEC3:
							{
								MVector3 vec;
								if(getVector3(L, 2, &vec))
									*(MVector3 *)variable.getPointer() = vec;
								return 0;
							}
						case M_VARIABLE_VEC4:
							{
								MVector4 vec;
								if(getVector4(L, 2, &vec))
									*(MVector4 *)variable.getPointer() = vec;
								return 0;
							}
						}
					}
				}
			}
		}
	}

	return 0;
}

int centerCursor(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:centerCursor);
	MEngine * engine = MEngine::getInstance();
	MSystemContext * system = engine->getSystemContext();
	MInputContext * input = engine->getInputContext();

	unsigned int width = 0;
	unsigned int height = 0;
	system->getScreenSize(&width, &height);
	int x = width/2;
	int y = height/2;

	system->setCursorPosition(x, y);
	input->setAxis("MOUSE_X", (float)(x / (float)width));
	input->setAxis("MOUSE_Y", (float)(y / (float)height));

	return 0;
}

int hideCursor(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:hideCursor);
	MEngine * engine = MEngine::getInstance();
	MSystemContext * system = engine->getSystemContext();
	
	system->hideCursor();
	
	return 0;
}

int showCursor(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:showCursor);
	MEngine * engine = MEngine::getInstance();
	MSystemContext * system = engine->getSystemContext();
	
	system->showCursor();
	
	return 0;
}

int getText(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getText);
	if(! isFunctionOk(L, "getText", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_TEXT)
		{
			const char * text = ((MOText *)object)->getText();
			if(text){
				lua_pushstring(L, text);
				return 1;
			}
		}
	}

	return 0;
}

int setText(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setText);
	if(! isFunctionOk(L, "setText", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		const char * text = lua_tostring(L, 2);
		if((text) && (object->getType() == M_OBJECT3D_TEXT))
		{
			((MOText *)object)->setText(text);
		}
	}

	return 0;
}

int getTextColor(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:getTextColor);
	MScript * script = (MScript *)MEngine::getInstance()->getScriptContext();

	if(! isFunctionOk(L, "getTextColor", 1))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_TEXT)
		{
			MVector4 * color = ((MOText *)object)->getColor();
			script->pushFloatArray(*color, 4);
			return 1;
		}
	}

	return 0;
}

int setTextColor(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:setTextColor);
	MScript * script = (MScript *)MEngine::getInstance()->getScriptContext();

	if(! isFunctionOk(L, "setTextColor", 2))
		return 0;

	MObject3d * object;
	lua_Integer id = lua_tointeger(L, 1);

	if((object = getObject3d(id)))
	{
		if(object->getType() == M_OBJECT3D_TEXT)
		{
			MVector4 color;
			script->getFloatArray(1, color, 4);
			((MOText *)object)->setColor(color);
		}
	}

	return 0;
}

char* getFile(const char* filename)
{
    M_PROFILE_SCOPE(lua-c:getFile);
    char* rtn = NULL;
    for(std::list<std::string>::iterator iPath = g_loadPath.begin(); iPath != g_loadPath.end(); ++iPath)
    {
	// make global filename
	char globalFilename[256];
	getGlobalFilename(globalFilename, iPath->c_str(), filename);

	if(isFileExist(globalFilename))
	    rtn = readTextFile(globalFilename);

	if(rtn)
	{
	    // do file and update current directory
	    getRepertory(g_currentDirectory, globalFilename);
	    g_loadPath.push_back(g_currentDirectory);
	    return rtn;
	}
    }
}

int doFile(lua_State * L)
{
    M_PROFILE_SCOPE(lua-c:doFile);
	if(! isFunctionOk(L, "doFile", 1))
	    return 0;
	    
	const char * filename = lua_tostring(L, 1);
	
	char * text = getFile(filename);

	if(!text) return 0;

	if(luaL_dostring(L, text) != 0)
	    printf("ERROR lua script: \n %s\n", lua_tostring(L, -1));
	else
	    g_loadPath.pop_back();

	SAFE_FREE(text);

	return 0;
}

// similar to dofile
// should have lookups, so you don't end up calling twice (not that it matters much)
// will first look for {param}/__init__.lua and then, failing that, {param}.lua
int require(lua_State* L)
{
    M_PROFILE_SCOPE(lua-c:require);
    if(!isFunctionOk(L, "require", 1))
	return 0;

    const char* filename = lua_tostring(L, 1);
    char init[256];
    snprintf(init, 256, "%s/__init__.lua", filename);
    char file[256];
    snprintf(file, 256, "%s.lua", filename);
    char* text = getFile(init);
    if(text == NULL)
	text = getFile(file);

    if(!text || strlen(text) == 0) return 0;

    if(luaL_dostring(L, text) != 0)
	printf("ERROR lua script: \n %s\n", lua_tostring(L, -1));
    else
	g_loadPath.pop_back();
    SAFE_FREE(text);

    return 0;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////
// Init
/////////////////////////////////////////////////////////////////////////////////////////////////////////

MScript::MScript(void):
m_state(NULL),
m_isRunning(false)
{
}

MScript::~MScript(void)
{
    M_PROFILE_SCOPE(MScript::~MScript);
	clear();
}

void MScript::init(void)
{
    M_PROFILE_SCOPE(MScript::init);
	// create context
	m_state = lua_open();
	g_loadPath.clear();
	g_loadPath.push_back("");

	luaopen_base(m_state);
	luaopen_table(m_state);
	luaopen_string(m_state);
	luaopen_math(m_state);

	// load custom functions
	lua_register(m_state, "getScene",	 getScene);
	lua_register(m_state, "getObject",	 getObject);
	lua_register(m_state, "getClone",	 getClone);
	lua_register(m_state, "rotate",		 rotate);
	lua_register(m_state, "translate",	 translate);
	lua_register(m_state, "getPosition", getPosition);
	lua_register(m_state, "getRotation", getRotation);
	lua_register(m_state, "getScale",	 getScale);
	lua_register(m_state, "setPosition", setPosition);
	lua_register(m_state, "setRotation", setRotation);
	lua_register(m_state, "setScale",	 setScale);
	lua_register(m_state, "isVisible",	 isVisible);
	lua_register(m_state, "activate",	 activate);
	lua_register(m_state, "deactivate", deactivate);

	// behavior
	lua_register(m_state, "getBehaviorVariable", getBehaviorVariable);
	lua_register(m_state, "setBehaviorVariable", setBehaviorVariable);

	// animation
	lua_register(m_state, "getCurrentAnimation",	getCurrentAnimation);
	lua_register(m_state, "changeAnimation",		changeAnimation);
	lua_register(m_state, "isAnimationOver",		isAnimationOver);
	lua_register(m_state, "getAnimationSpeed",		getAnimationSpeed);
	lua_register(m_state, "setAnimationSpeed",		setAnimationSpeed);
	lua_register(m_state, "getCurrentFrame",		getCurrentFrame);
	lua_register(m_state, "setCurrentFrame",		setCurrentFrame);

	// physics
	lua_register(m_state, "setGravity",			setGravity);
	lua_register(m_state, "getGravity",			getGravity);
	lua_register(m_state, "addCentralForce",	addCentralForce);
	lua_register(m_state, "addTorque",			addTorque);
	lua_register(m_state, "getLinearDamping",	getLinearDamping);
	lua_register(m_state, "setLinearDamping",	setLinearDamping);
	lua_register(m_state, "getAngularDamping",	getAngularDamping);
	lua_register(m_state, "setAngularDamping",	setAngularDamping);
	lua_register(m_state, "isCollisionTest",	isCollisionTest);
	lua_register(m_state, "isCollisionBetween", isCollisionBetween);
	lua_register(m_state, "clearForces",		clearForces);
	lua_register(m_state, "getNumCollisions",	getNumCollisions);
	lua_register(m_state, "rayHit",				rayHit);

	// input
	lua_register(m_state, "isKeyPressed", isKeyPressed);
	lua_register(m_state, "onKeyDown",	  onKeyDown);
	lua_register(m_state, "onKeyUp",	  onKeyUp);
	lua_register(m_state, "getAxis",	  getAxis);
	lua_register(m_state, "getProperty",  getProperty);
    
    // mobile-input
    lua_register(m_state, "getTouchPosition", getTouchPosition);
    lua_register(m_state, "getLastTouchPosition", getLastTouchPosition);
    lua_register(m_state, "getTouchPhase", getTouchPhase);

	// sound
	lua_register(m_state, "playSound",	  playSound);
	lua_register(m_state, "pauseSound",	  pauseSound);
	lua_register(m_state, "stopSound",	  stopSound);
	lua_register(m_state, "getSoundGain", getSoundGain);
	lua_register(m_state, "setSoundGain", setSoundGain);
	
	// scene/level
	lua_register(m_state, "changeScene",			changeScene);
	lua_register(m_state, "getCurrentSceneId",		getCurrentSceneId);
	lua_register(m_state, "getScenesNumber",		getScenesNumber);
	lua_register(m_state, "doesLevelExist",			doesLevelExist);
	lua_register(m_state, "loadLevel",				loadLevel);

	// light
	lua_register(m_state, "getLightColor",	   getLightColor);
	lua_register(m_state, "getLightRadius",	   getLightRadius);
	lua_register(m_state, "getLightIntensity", getLightIntensity);
	lua_register(m_state, "setLightColor",	   setLightColor);
	lua_register(m_state, "setLightRadius",	   setLightRadius);
	lua_register(m_state, "setLightIntensity", setLightIntensity);

	// camera
	lua_register(m_state, "changeCurrentCamera",    changeCurrentCamera);
	lua_register(m_state, "getCameraClearColor",    getCameraClearColor);
	lua_register(m_state, "getCameraFov",		    getCameraFov);
	lua_register(m_state, "getCameraNear",		    getCameraNear);
	lua_register(m_state, "getCameraFar",		    getCameraFar);
	lua_register(m_state, "getCameraFogDistance",   getCameraFogDistance);
	lua_register(m_state, "isCameraOrtho",		    isCameraOrtho);
	lua_register(m_state, "isCameraFogEnabled",	    isCameraFogEnabled);
	lua_register(m_state, "setCameraClearColor",    setCameraClearColor);
	lua_register(m_state, "setCameraFov",		    setCameraFov);
	lua_register(m_state, "setCameraNear",	        setCameraNear);
	lua_register(m_state, "setCameraFar",	        setCameraFar);
	lua_register(m_state, "setCameraFogDistance",   setCameraFogDistance);
	lua_register(m_state, "enableCameraOrtho",      enableCameraOrtho);
	lua_register(m_state, "enableCameraFog",	    enableCameraFog);
	lua_register(m_state, "enableCameraLayer",      enableCameraLayer);
	lua_register(m_state, "disableCameraLayer",	    disableCameraLayer);
	lua_register(m_state, "enableRenderToTexture",  enableRenderToTexture);
	lua_register(m_state, "disableRenderToTexture", disableRenderToTexture);
	lua_register(m_state, "getScreenSize",          getScreenSize);

	// text
	lua_register(m_state, "getText", getText);
	lua_register(m_state, "setText", setText);
	lua_register(m_state, "getTextColor", getTextColor);
	lua_register(m_state, "setTextColor", setTextColor);

	// cursor
	lua_register(m_state, "centerCursor", centerCursor);
	lua_register(m_state, "hideCursor", hideCursor);
	lua_register(m_state, "showCursor", showCursor);

	// dofile
	lua_register(m_state, "dofile", doFile);
	lua_register(m_state, "require", require);

	// quit
	lua_register(m_state, "quit", quit);
	
	
	// register custom functions
	map<string, int (*)(void)>::iterator
		mit (m_functions.begin()),
		mend(m_functions.end());

	for(;mit!=mend;++mit)
		lua_register(m_state, mit->first.c_str(), function);
}

void MScript::clear(void)
{
    M_PROFILE_SCOPE(MScript::clear);
	if(m_state)
	{
		lua_close(m_state);
		m_state = NULL;
	}
	m_isRunning = false;
}

int MScript::function(lua_State * L)
{
    M_PROFILE_SCOPE(MScript::function)
	MScript * script = (MScript *)MEngine::getInstance()->getScriptContext();

	lua_Debug ar;
	lua_getstack(L, 0, &ar);
	lua_getinfo(L, "n", &ar);

	map<string, int (*)(void)>::iterator iter = script->m_functions.find(ar.name);
	if(iter != script->m_functions.end())
		return iter->second();

	return 0;
}

void MScript::runScript(const char * filename)
{
    M_PROFILE_SCOPE(MScript::runScript);
        clear();
	init();

	if(! filename)
	{
		m_isRunning = false;
		return;
	}

	if(strlen(filename) == 0)
	{
		m_isRunning = false;
		return;
	}

	// current directory
	getRepertory(g_currentDirectory, filename);
	g_loadPath.push_back(g_currentDirectory);
	
	// read file
	char * text = readTextFile(filename);
	if(! text)
	{
		printf("ERROR lua script : unable to read file %s\n", filename);
		m_isRunning = false;
		return;
	}
	
	// do string
	if(luaL_dostring(m_state, text) != 0)
	{
		printf("ERROR lua script :\n %s\n", lua_tostring(m_state, -1));
		m_isRunning = false;
		SAFE_FREE(text);
		return;
	}
	
	// finish
	SAFE_FREE(text);
	m_isRunning = true;
}

bool MScript::addScript(const char* filename)
{
    M_PROFILE_SCOPE(MScript::addScript);
    if(!filename || !strlen(filename))
	return false;
    // save current directory
    char current[256];
    strcpy(current, g_currentDirectory);
    
    // make global filename
    char globalFilename[256];
    getGlobalFilename(globalFilename, g_currentDirectory, filename);
    
    // do file and update current directory
    getRepertory(g_currentDirectory, globalFilename);

    g_loadPath.push_back(g_currentDirectory);
    char * text = readTextFile(globalFilename);
    if(!text)
    {
	g_loadPath.pop_back();
	return false;
    }
    bool rtn = luaL_dostring(m_state, text) == 0;

    if(!rtn)
	printf("ERROR lua script: \n %s\n", lua_tostring(m_state, -1));
    
    // set back to current
    g_loadPath.pop_back();
    strcpy(g_currentDirectory, current);
    m_isRunning = m_isRunning || rtn;
    SAFE_FREE(text);
    return rtn;
}

bool MScript::startCallFunction(const char* name)
{
    M_PROFILE_SCOPE(MScript::startCallFunction)
	if(m_isRunning)
	{
		lua_getglobal(m_state, name);
		if(!lua_isfunction(m_state, -1))
		{
			lua_pop(m_state, 1);
			return false;
		}
		return true;
	}
	return false;
}

bool MScript::endCallFunction(int numArgs)
{
    M_PROFILE_SCOPE(MScript::endCallFunction);
	if(lua_pcall(m_state, numArgs, 0, 0) != 0)
	{
		printf("ERROR lua script :\n %s\n", lua_tostring(m_state, -1));
		m_isRunning = false;
		return false;
	}
	return true;
}

void MScript::callFunction(const char * name)
{
    M_PROFILE_SCOPE(MScript::callFunction);
	if(startCallFunction(name))
		endCallFunction();
}

void MScript::addFunction(const char * name, int (*_function)(void)){
    M_PROFILE_SCOPE(MScript::addFunction);
	m_functions[name] = _function;
	if(m_isRunning)
	    lua_register(m_state, name, function);
}

unsigned int MScript::getArgsNumber(void){
    M_PROFILE_SCOPE(MScript::getArgsNumber);
	return lua_gettop(m_state);
}

void MScript::getIntArray(unsigned int arg, int * values, unsigned int valuesNumber)
{
    M_PROFILE_SCOPE(MScript::getIntArray);
	arg++;
	if(lua_istable(m_state, arg) && (lua_objlen(m_state, arg) >= valuesNumber))
	{
		lua_pushnil(m_state);
		for(unsigned int i=0; i<valuesNumber; i++)
		{
			lua_next(m_state, arg);
			values[i] = (int)lua_tointeger(m_state, -1);
			lua_pop(m_state, 1);
		}
	}
}

void MScript::getFloatArray(unsigned int arg, float * values, unsigned int valuesNumber)
{
    M_PROFILE_SCOPE(MScript::getFloatArray);
	arg++;
	if(lua_istable(m_state, arg) && (lua_objlen(m_state, arg) >= valuesNumber))
	{
		lua_pushnil(m_state);
		for(unsigned int i=0; i<valuesNumber; i++)
		{
			lua_next(m_state, arg);
			values[i] = (float)lua_tonumber(m_state, -1);
			lua_pop(m_state, 1);
		}
	}
}

const char * MScript::getString(unsigned int arg){
    M_PROFILE_SCOPE(MScript::getString);
	return lua_tostring(m_state, arg+1);
}

int MScript::getInteger(unsigned int arg){
    M_PROFILE_SCOPE(MScript::getInteger);
	return (int)lua_tointeger(m_state, arg+1);
}

float MScript::getFloat(unsigned int arg){
    M_PROFILE_SCOPE(MScript::getFloat);
	return (float)lua_tonumber(m_state, arg+1);
}

void* MScript::getPointer(unsigned int arg){
    M_PROFILE_SCOPE(MScript::getPointer);
	return (void*)lua_tointeger(m_state, arg+1);
}

void MScript::pushIntArray(const int * values, unsigned int valuesNumber)
{
    M_PROFILE_SCOPE(MScript::pushIntArray);
	lua_newtable(m_state);
	for(unsigned int i=0; i<valuesNumber; i++)
	{
		lua_pushinteger(m_state, (lua_Integer)i+1);
		lua_pushnumber(m_state, (lua_Integer)values[i]);
		lua_rawset(m_state, -3);
	}
}

void MScript::pushFloatArray(const float * values, unsigned int valuesNumber)
{
    M_PROFILE_SCOPE(MScript::pushFloatArray);
	lua_newtable(m_state);
	for(unsigned int i=0; i<valuesNumber; i++)
	{
		lua_pushinteger(m_state, (lua_Integer)i+1);
		lua_pushnumber(m_state, (lua_Number)values[i]);
		lua_rawset(m_state, -3);
	}
}

void MScript::pushBoolean(bool value){
    M_PROFILE_SCOPE(MScript::pushBoolean);
	lua_pushboolean(m_state, (int)value);
}

void MScript::pushString(const char * string){
    M_PROFILE_SCOPE(MScript::pushString);
	lua_pushstring(m_state, string);
}

void MScript::pushInteger(int value){
    M_PROFILE_SCOPE(MScript::pushInteger);
	lua_pushinteger(m_state, (lua_Integer)value);
}

void MScript::pushFloat(float value){
    M_PROFILE_SCOPE(MScript::pushFloat);
	lua_pushnumber(m_state, (lua_Number)value);
}

void MScript::pushPointer(void* value){
    M_PROFILE_SCOPE(MScript::pushPointer);
	lua_pushinteger(m_state, (lua_Integer)value);
}
