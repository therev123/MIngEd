#include "editor.h"

#include "atlas.h"
#include "renderer.h"
#include "util.h"

#include "minged_npk.h"

#include <stdio.h>

#include <MEngine.h>
#include <MGui.h>
#include <MKeyboard.h>
#include <MFile.h>

#define PACKAGE_NAME "minged.npk"

namespace minged
{
    Editor::Editor()
    {
	m_Initialised = false;
	m_IsOpen= false;
	m_ToggleButton = eUp;
    }
    
    Editor::~Editor()
    {
	MEngine* engine = MEngine::getInstance();
	if(MScriptContext* script = engine->getScriptContext())
	    script->callFunction("mingedCleanup");

	delete m_FileManager;
    }

    void Editor::Init()
    {
	MEngine* engine = MEngine::getInstance();

	m_FileManager = new EmbedFileOpenHook;
	m_FileManager->AddEmbeddedFile(PACKAGE_NAME, minged_npk, minged_npkSize());
	
	char filename[255];
	getGlobalFilename(filename, engine->getSystemContext()->getWorkingDirectory(), PACKAGE_NAME);
	engine->getPackageManager()->loadPackage(filename);

	MScriptContext* script = engine->getScriptContext();
	
	// load the script
	script->addScript("minged/minged.lua");

	// register all minged functions
	Atlas::RegisterScript(script);
	Renderer::RegisterScript(script);

	script->callFunction("mingedInit");

	m_Initialised = true;
    }

    void Editor::Update(uint32 dt)
    {	
	if(!m_Initialised)
	    Init();
	CheckToToggle();
	
	if(m_IsOpen)
	    UpdateOpen(dt);
    }
    
    void Editor::CheckToToggle()
    {
	// if just pressed key, toggle m_IsOpen
	MEngine* engine = MEngine::getInstance();
	if(MInputContext* input = engine->getInputContext())
	{
	    if(input->isKeyPressed("F1"))
	    {
		if(m_ToggleButton == ePressed || m_ToggleButton == eUp)
		    m_ToggleButton = eUp;
		else
		    m_ToggleButton = ePressed;
	    }
	    else
	    {
		if(m_ToggleButton == eReleased || m_ToggleButton == eDown)
		    m_ToggleButton = eDown;
		else
		    m_ToggleButton = eReleased;
	    }
	}

	if(m_ToggleButton == ePressed)
	    m_IsOpen = !m_IsOpen;
    }
    
    void Editor::UpdateOpen(uint32 dt)
    {
	MEngine* engine = MEngine::getInstance();
	if(MScriptContext* script = engine->getScriptContext())
	    script->callFunction("mingedUpdate");

    }
    
    void Editor::Render()
    {
	if(m_IsOpen)
	{
	    MEngine* engine = MEngine::getInstance();
	    if(MScriptContext* script = engine->getScriptContext())
		script->callFunction("mingedRender");

	    Renderer::Flush();
	}
    }
};
