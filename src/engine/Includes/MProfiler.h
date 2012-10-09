#ifndef __MARATIS_PROFILER_H__
#define __MARATIS_PROFILER_H__

class M_ENGINE_EXPORT MProfilerContext
{
public:
    virtual void pushScope(const char* name) = 0;
    virtual void popScope() = 0;

    virtual void init() = 0;
    virtual void update() = 0;
    virtual void cleanup() = 0;
};

class M_ENGINE_EXPORT MProfileScope
{
public:
    MProfileScope(const char* name);
    ~MProfileScope();
};

#ifdef M_SHOULD_PROFILE
#   define M_PROFILE_SCOPE(name) \
    MProfileScope((#name));
#else
#   define M_PROFILE_SCOPE(name) {}
#endif

#endif /*__MARATIS_PROFILER_H__*/
