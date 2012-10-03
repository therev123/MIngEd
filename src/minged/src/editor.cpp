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
#include "MEmbedFile.h"

#define PACKAGE_NAME "minged.npk"

namespace minged
{
    class EditorPluginHook : public MIPluginLoadHook
    {
	virtual void Load(MPlugin* plugin) { plugin->callFunction("StartEditor"); }
	virtual void Unload(MPlugin* plugin) { plugin->callFunction("EndEditor"); }
    };
    EditorPluginHook s_Hook;

    Editor::Editor()
    {
	m_Initialised = false;
	m_IsOpen= false;
	m_ToggleButton = eUp;
	m_Data = 0;
	Init();
    }
    
    Editor::~Editor()
    {
	MEngine* engine = MEngine::getInstance();
	if(MScriptContext* script = engine->getScriptContext())
	    script->callFunction("mingedCleanup");
	if(m_Data && engine->getPackageManager())
	    engine->getPackageManager()->unloadPackage(m_Data);
	m_Data = 0;
    }

    void Editor::Init()
    {
	MEngine* engine = MEngine::getInstance();

	engine->getEmbedFileManager()->AddEmbeddedFile(PACKAGE_NAME, minged_npk, minged_npkSize());
	
	char filename[255];
	getGlobalFilename(filename, engine->getSystemContext()->getWorkingDirectory(), PACKAGE_NAME);
	m_Data = engine->getPackageManager()->loadPackage(filename);

	MScriptContext* script = engine->getScriptContext();
	
	// load the script
	script->addScript("minged/minged.lua");

	// register all minged functions
	Atlas::RegisterScript(script);
	Renderer::RegisterScript(script);

	script->callFunction("mingedInit");

	// Load all system plugins
	char pluginDir[0xff];
	getGlobalFilename(pluginDir,
			  engine->getSystemContext()->getSystemDirectory(),
			  "Plugins");
	std::vector<std::string> files;
	readDirectory(pluginDir, &files);
	for(std::vector<std::string>::iterator iFile = files.begin();
	    iFile != files.end(); iFile++)
	{
	    char file[0xff];
	    snprintf(file, 0xff, iFile->c_str());
	    for(int i=iFile->size() - 1; i >= 0; --i)
		if(file[i] == '.') file[i] = 0;
	    engine->loadPlugin(file);
	}

	m_Initialised = true;
    }

    void Editor::Update(uint32 dt)
    {	
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
