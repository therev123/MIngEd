#include "minged.h"

// minged includes
#include "editor.h"

// system includes
#include <stdio.h>
#include <pthread.h>
#include <time.h>
#include <unistd.h>

minged::Editor instance;
pthread_t thread;

#define MStoNS(_ms) (_ms * 1000000)
#define NStoMS(_ns) (_ns / 1000000)

uint32 GetTimeMS()
{
  struct timespec t;
  clock_gettime(CLOCK_MONOTONIC, &t);
  return (t.tv_sec * 1000) + NStoMS(t.tv_nsec);
}

void Sleep(uint32 ms)
{
  struct timespec t;
  t.tv_nsec = MStoNS(ms);
  t.tv_sec = 0;
  nanosleep(&t, NULL);
}

#define FRAMERATE 60
#define TICKTIME (1000/FRAMERATE)

void* RunPlugin(void* data)
{
  minged::Editor* editor = (minged::Editor*)data;

  uint32 prevtick = GetTimeMS();
  while(true)
  {
    uint32 ms = GetTimeMS();
    uint32 dt = ms - prevtick;
    editor->Update(dt);
    uint32 postms = GetTimeMS();
    if(postms - ms < TICKTIME)
      Sleep(TICKTIME - (postms - ms));
    prevtick = ms;    
  }
}

uint32 tick;
void StartPlugin()
{
#ifdef MINGED_USE_PTHREAD
  pthread_create(&thread, NULL, RunPlugin, &instance);
#endif/*MINGED_USE_PTHREAD*/
  tick = GetTimeMS();
}

void UpdatePlugin()
{
  uint32 ms = GetTimeMS();
  uint32 dt = ms - tick;
  instance.Update(dt);
  tick = ms;
}

void Render()
{
  instance.Render();
}

void EndPlugin()
{
#ifdef MINGED_USE_PTHREAD
  pthread_exit(&thread);
#endif/*MINGED_USE_PTHREAD*/
}
