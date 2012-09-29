#include "../Includes/MEngine.h"

#include "../Includes/MProfiler.h"

MProfileScope::MProfileScope(const char* name)
{
    MEngine* engine = MEngine::getInstance();
    if(MProfilerContext* profiler = engine->getProfilerContext())
    {
	profiler->pushScope(name);
    }
}

MProfileScope::~MProfileScope()
{
    MEngine* engine = MEngine::getInstance();
    if(MProfilerContext* profiler = engine->getProfilerContext())
    {
	profiler->popScope();
    }
}

