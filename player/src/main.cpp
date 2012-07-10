/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MaratisPlayer
// main.cpp
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
//  Maratis, Copyright (c) 2003-2011 Anael Seghezzi <www.maratis3d.com>
//
//  This program is free software; you can redistribute it and/or
//  modify it under the terms of the GNU General Public License
//  as published by the Free Software Foundation; either version 2
//  of the License, or (at your option) any later version.
//
//  This program is distributed in the hope that it will be useful,
//  but WITHOUT ANY WARRANTY; without even the implied warranty of
//  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//  GNU General Public License for more details.
//
//  You should have received a copy of the GNU General Public License
//  along with this program; if not, write to the Free Software Foundation,
//  Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
//
//========================================================================
//jan 2012 - Philipp Geyer <philipp@geyer.co.uk> - embedded project/package manager


#include <MEngine.h>
#include <MWindow.h>

#include <MGameWinEvents.h>
#include "MaratisPlayer.h"

#include "plugin.h"

// window events
void windowEvents(MWinEvent * windowEvents)
{
	MEngine * engine = MEngine::getInstance();

	// game
	MGame * game = engine->getGame();
	if(game)
	{
		if(game->isRunning())
        {
			gameWinEvents(windowEvents);
		}
	}

	if(windowEvents->type == MWIN_EVENT_KEY_DOWN && windowEvents->data[0] == MKEY_ESCAPE)
    {
		MWindow::getInstance()->setActive(false);
	}

	if(windowEvents->type == MWIN_EVENT_WINDOW_CLOSE)
    {
		MWindow::getInstance()->setActive(false);
	}
}

// update
void update(void)
{
	MaratisPlayer::getInstance()->logicLoop();
}

// draw
void draw(void)
{
	MaratisPlayer::getInstance()->graphicLoop();
	MWindow::getInstance()->swapBuffer();
}

#ifndef PRELOAD_MAX
#define PRELOAD_MAX 5
#endif 

typedef struct CommandParameters
{
  CommandParameters()
    : width(1024), height(768), fullscreen(0), project(0)
  {
    preloads = new Plugin*[PRELOAD_MAX];
    for(int i=0; i<PRELOAD_MAX; ++i)
      preloads[i] = 0;
  }
  ~CommandParameters()
  {
    for(int i=0; i<PRELOAD_MAX; ++i)
      if(preloads[i]) { delete preloads[i]; preloads[i] = 0; }
    delete [] preloads;
  }
  unsigned int width;
  unsigned int height;
  unsigned int fullscreen;
  Plugin** preloads;
  char* project;
} CommandParameters;

void AddPreload(CommandParameters &params, char* libname)
{
  // first check whether we have a slot available
  int slot = 0;
  while(params.preloads[slot] != 0 && slot < PRELOAD_MAX) slot++;
  if(slot == PRELOAD_MAX && params.preloads[slot] != 0)
  {
    printf("Unable to load preload: %s: Not enough preload slots\n", libname);
    return;
  }

  Plugin* plugin = new Plugin();
  plugin->load(libname);
  if(strlen(plugin->getFilename()) == 0)
  {
      printf("Unable to load preload: %s\n", libname);
    delete plugin;
    return;
  }

  params.preloads[slot] = plugin;
}

void ParseParams(CommandParameters &params, int argc, char** argv)
{
  if(argc < 2)
    return;
  for(int i = 1;i < argc; ++i)
    {
      if(strstr(argv[i], "--width=") == argv[i])
      {
	params.width = atoi(&argv[i][8]);
      }
      else if(strcmp(argv[i], "-w") == 0)
      {
	params.width = atoi(argv[i+1]);
	i++;
      }
      else if(strstr(argv[i], "--height=") == argv[i])
      {
	params.height = atoi(&argv[i][9]);
      }
      else if(strcmp(argv[i], "-h") == 0)
      {
	params.height = atoi(argv[i+1]);
	i++;
      }
      else if( strcmp(argv[i], "--fullscreen") == 0 ||
	       strcmp(argv[i], "-f") == 0 )
      {
	params.fullscreen = 1;
      }
      else if( strstr(argv[i], "--preload=") == argv[i] )
      {
	AddPreload(params, &argv[i][10]);
      }
      else if( strcmp(argv[i], "-P") == 0)
      {
	AddPreload(params, argv[i+1]);
	i++;
      }
      else if( strstr(argv[i], "--project=") == argv[i] )
      {
	params.project = &argv[i][10];
      }
      else // just assume it's a project path
      {
	params.project = argv[i];
      }
    }
}


// main
int main(int argc, char **argv)
{
	setlocale(LC_NUMERIC, "C");
	
	CommandParameters params;
	ParseParams(params, argc, argv);
	
	// get engine (first time call onstructor)
	MEngine * engine = MEngine::getInstance();

	// get window (first time call onstructor)
	MWindow * window = MWindow::getInstance();

	// create window
	window->create("Maratis", params.width, params.height, 32, params.fullscreen > 0);
	if(false)
		window->hideCursor();

	// set current directory
	char rep[256];
	getRepertory(rep, argv[0]);
	window->setCurrentDirectory(rep);
	
	// get Maratis (first time call onstructor)
	MaratisPlayer * maratis = MaratisPlayer::getInstance();

	// window pointer event
	window->setPointerEvent(windowEvents);

	// load project
	bool projectFound = false;
	if(params.project != 0)
    {
		char filename[256];
		getGlobalFilename(filename, window->getCurrentDirectory(), params.project);
		if(maratis->loadProject(filename))
		{
			engine->getGame()->begin();
			projectFound = true;
		}
	}
	
	if(! projectFound)
	{
		// if we've found, and written the embedded sections, then use that data
		if((strstr(s_embedded_level_name, "[EMBEDDED") != s_embedded_level_name) &&
		   (strstr(s_embedded_renderer, "[EMBEDDED") != s_embedded_renderer))
		{
			MProject embeddedProj;
			embeddedProj.renderer = s_embedded_renderer;
			
			char levelName[256];
			getGlobalFilename(levelName, window->getCurrentDirectory(), s_embedded_level_name);
			char projName[256];
			getGlobalFilename(projName, window->getCurrentDirectory(), s_embedded_game_name);
			
			embeddedProj.startLevel = levelName;
			maratis->loadProject(&embeddedProj, projName);
			engine->getGame()->begin();
			projectFound = true;
		}
		else
		{
			// look for an mproj in the current directory
			std::vector<std::string> files;
			readDirectory(".", &files);
			
			for(int i = 0; i < files.size(); ++i)
			{
				if(strstr(files[i].c_str(), ".mproj"))
				{
					char filename[256];
					getGlobalFilename(filename, window->getCurrentDirectory(), files[i].c_str());
					
					if(maratis->loadProject(filename))
					{
						engine->getGame()->begin();
						projectFound = true;
						break;
					}
				}
			}
		}
	}

	
	// time
	unsigned int frequency = 60;
	unsigned long previousFrame = 0;
	unsigned long startTick = window->getSystemTick();
	
	
	// on events
	while(window->isActive())
	{
		// on events
		if(window->onEvents())
		{
			// compute target tick
			unsigned long currentTick = window->getSystemTick();

			unsigned long tick = currentTick - startTick;
			unsigned long frame = (unsigned long)(tick * (frequency * 0.001f));

			// update elapsed time
			unsigned int i;
			unsigned int steps = (unsigned int)(frame - previousFrame);

			if(window->getFocus())
			{
				// don't wait too much
				if(steps >= (frequency/2))
				{
					update();
					draw();
					previousFrame += steps;
					continue;
				}

				for(i=0; i<PRELOAD_MAX; ++i)
				  if(params.preloads[i])
				    params.preloads[i]->Update();
				
				// update
				for(i=0; i<steps; i++)
				{
					update();
					previousFrame++;
				}

				// draw
				if(steps > 0){
					draw();
				}

				for(i=0; i<PRELOAD_MAX; ++i)
				  if(params.preloads[i])
				    params.preloads[i]->Draw();
			}
			else
			{
				previousFrame = frame;
				window->swapBuffer();
			}
		}
	}

	maratis->clear();
	return 0;
}
