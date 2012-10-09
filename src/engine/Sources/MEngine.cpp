/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MEngine
// MEngine.cpp
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
#include "../Includes/MEmbedFile.h"
#include "MPlugin.h"

#include "MCore.h"
#include "MConfigFile.h"

MEngine::MEngine(void):
m_isActive(true),
m_level(NULL),
m_game(NULL),
m_renderer(NULL),
m_soundContext(NULL),
m_renderingContext(NULL),
m_physicsContext(NULL),
m_scriptContext(NULL),
m_inputContext(NULL),
m_systemContext(NULL),
m_packageManager(NULL),
m_requestedLevelToLoad(NULL)
{}

MEngine::~MEngine(void)
{
    M_PROFILE_SCOPE(MEngine::MEngine);

    for(std::list<MPlugin*>::iterator iPlugin = m_Plugins.begin();
	iPlugin != m_Plugins.end(); iPlugin++)
	delete *iPlugin;
    m_Plugins.clear();

	SAFE_FREE(m_requestedLevelToLoad);
}

MEngine * MEngine::getInstance(void)
{
    // we don't want to profile this, because it
    // just leads to calling this again...
    // returning a static instance should be
    // quick enough to not bother us
    // M_PROFILE_SCOPE(MEngine::getInstance);
	static MEngine m_instance;
	return &m_instance;
}

void MEngine::setRenderingContext(MRenderingContext * renderingContext)
{
    M_PROFILE_SCOPE(MEngine::setRenderingContext);
	m_renderingContext = renderingContext;
}

void MEngine::setSoundContext(MSoundContext * soundContext)
{
    M_PROFILE_SCOPE(MEngine::setSoundContext)
	m_soundContext = soundContext;
}

void MEngine::setPhysicsContext(MPhysicsContext * physicsContext)
{
    M_PROFILE_SCOPE(MEngine::setPhysicsContext);
	m_physicsContext = physicsContext;
}

void MEngine::setScriptContext(MScriptContext * scriptContext)
{
    M_PROFILE_SCOPE(MEngine::setScriptContext);
	m_scriptContext = scriptContext;
}

void MEngine::setInputContext(MInputContext * inputContext)
{
    M_PROFILE_SCOPE(MEngine::setInputContext);
	m_inputContext = inputContext;
}

void MEngine::setSystemContext(MSystemContext * systemContext)
{
    M_PROFILE_SCOPE(MEngine::setSystemContext);
	m_systemContext = systemContext;
}

void MEngine::setProfilerContext(MProfilerContext * profilerContext)
{
    // we probably don't care about profiling this either
    // M_PROFILE_SCOPE(MEngine::setProfilerContext);
	m_profilerContext = profilerContext; 
}

void MEngine::setPackageManager(MPackageManager * packageManager)
{
    M_PROFILE_SCOPE(MEngine::setPackageManager);
	m_packageManager = packageManager;
}

void MEngine::addFileLoader(const char* ext, MIFileLoader* loader)
{
    M_PROFILE_SCOPE(MEngine::addFileLoader);
    m_fileLoaders[ext] = loader;
}

void* MEngine::loadFile(const char* file, void* data)
{
    M_PROFILE_SCOPE(MEngine::loadFile);
    std::map<std::string, MIFileLoader*>::iterator iFL;
    for(iFL = m_fileLoaders.begin(); iFL != m_fileLoaders.end(); iFL++)
	if(strstr(file, iFL->first.c_str()) != 0)
	    return iFL->second->load(file, data);
    return NULL;
}

void MEngine::setLevel(MLevel * level)
{
    M_PROFILE_SCOPE(MEngine::setLevel);
	m_level = level;
}

void MEngine::updateRequests(void)
{
    M_PROFILE_SCOPE(MEngine::updateRequests);
	// see if we should load requested scene or level
	loadLevelIfRequested();
	if(m_level)
		m_level->changeCurrentSceneIfRequested();
}

bool MEngine::loadLevel(const char * filename)
{
    M_PROFILE_SCOPE(MEngine::loadLevel);
	if(! filename)
		return false;

	if(! m_level)
		return false;

	char globalFilename[256];
	getGlobalFilename(globalFilename, m_systemContext->getWorkingDirectory(), filename);

	// Check if there is a level under the given name
	if(! isFileExist(globalFilename))
		return false;

	if(m_game)
	{
		if(m_game->isRunning())
		{
			m_game->onEndScene();
			m_game->onEndLevel();
		}
	}

	if(m_levelLoader.loadData(globalFilename, m_level))
	{
		if(m_game){
			if(m_game->isRunning())
			{
				m_game->onBeginLevel();
				m_game->onBeginScene();
			}
		}
		return true;
	}

	return false;
}

void MEngine::requestLoadLevel(const char * filename)
{
    M_PROFILE_SCOPE(MEngine::requestLoadLevel);
	SAFE_FREE(m_requestedLevelToLoad);

	unsigned int len = strlen(filename);
	m_requestedLevelToLoad = (char*)malloc(len + 1);
	strcpy(m_requestedLevelToLoad, filename);
}

bool MEngine::doesLevelExist(const char * filename)
{
    M_PROFILE_SCOPE(MEngine::doesLevelExist);
	if(! filename)
		return false;

	char globalFilename[256];
	getGlobalFilename(globalFilename, m_systemContext->getWorkingDirectory(), filename);
	return isFileExist(globalFilename);
}

void MEngine::setGame(MGame * game)
{
    M_PROFILE_SCOPE(MEngine::setGame);
	m_game = game;
}

void MEngine::setRenderer(MRenderer * renderer)
{
    M_PROFILE_SCOPE(MEngine::setRenderer);

    static int test = 0;
    test++;
    printf("setRenderer called %d time(s)\n", test);
	m_renderer = renderer;
}

void MEngine::loadLevelIfRequested()
{
    M_PROFILE_SCOPE(MEngine::loadLevelIfRequested);
	if(m_requestedLevelToLoad == NULL)
		return;

	loadLevel(m_requestedLevelToLoad);
	SAFE_FREE(m_requestedLevelToLoad);
}

void MEngine::loadPlugin(const char* name)
{
    M_PROFILE_SCOPE(MEngine::loadPlugin);
    MPlugin* plugin = new MPlugin;
    plugin->load(name);
    if(plugin->isLoaded())
	m_Plugins.push_back(plugin);
    else
	delete plugin;
}
