#include "editor.h"

#include "atlas.h"
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

	    MRenderingContext* render = engine->getRenderingContext();
	    MSystemContext* system = engine->getSystemContext();

	    static MVector3 testQuad[4] = 
	    {
		MVector3(0,0,1),
		MVector3(256,0,1),
		MVector3(256,256,1),
		MVector3(0,256,1)
	    };

	    unsigned int w, h;
	    system->getScreenSize(&w, &h);

	    render->setViewport(0, 0, w, h);
	    render->setOrthoView(0, w, 0, h, 0.1, 100);
	    render->enableVertexArray();
	    render->setVertexPointer(M_FLOAT, 3, testQuad);
	    render->setDepthMask(false);
	    render->drawArray(M_PRIMITIVE_TRIANGLE_FAN, 0, 4);
	}
    }
};
