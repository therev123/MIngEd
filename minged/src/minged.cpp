#include "minged.h"

// minged includes
#include "editor.h"
#include "util.h"

#ifdef MINGED_USE_PTHREAD
#  undef MINGED_USE_PTHREAD
#endif

// system includes
#include <stdio.h>
#ifdef MINGED_USE_PTHREAD
#include <pthread.h>
#endif/*MINGED_USE_PTHREAD*/
#include <time.h>
#ifdef WIN32
# include <windows.h>
# include <time.h>
#else
# include <unistd.h>
#endif

minged::Editor* instance;
#ifdef MINGED_USE_PTHREAD
pthread_t thread;
#endif/*MINGED_USE_PTHREAD*/

#define FRAMERATE 60
#define TICKTIME (1000/FRAMERATE)

void* RunPlugin(void* data)
{
  minged::Editor* editor = (minged::Editor*)data;

  uint32 prevtick = minged::util::GetTimeMS();
  while(true)
  {
      uint32 ms = minged::util::GetTimeMS();
    uint32 dt = ms - prevtick;
    editor->Update(dt);
    uint32 postms = minged::util::GetTimeMS();
    if(postms - ms < TICKTIME)
	minged::util::Sleep(TICKTIME - (postms - ms));
    prevtick = ms;    
  }
}

uint32 tick;

MPLUGIN_START_IMPLEMENT(minged)
{
    instance = new minged::Editor;

#ifdef MINGED_USE_PTHREAD
  pthread_create(&thread, NULL, RunPlugin, instance);
#endif/*MINGED_USE_PTHREAD*/
  tick = minged::util::GetTimeMS();
}

void UpdatePlugin()
{
    uint32 ms = minged::util::GetTimeMS();
  uint32 dt = ms - tick;
  instance->Update(dt);
  tick = ms;
}

void Render()
{
  instance->Render();
}

MPLUGIN_END_IMPLEMENT(minged)
{
#ifdef MINGED_USE_PTHREAD
  pthread_exit(&thread);
#endif/*MINGED_USE_PTHREAD*/
  delete instance;
}
