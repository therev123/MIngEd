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
    if(dt < TICKTIME)
      Sleep(TICKTIME - dt);
    prevtick = ms;    
  }
}

void StartPlugin()
{
  pthread_create(&thread, NULL, RunPlugin, &instance);
}

void EndPlugin()
{
  pthread_exit(&thread);
}
