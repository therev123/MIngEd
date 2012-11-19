#ifndef __M_GRAPH_H__
#define __M_GRAPH_H__

class M_UTIL_EXPORT MGraph
{
public:
    virtual float getValue(float ref) = 0;

    static MGraph* getGraphNamed(const char* name);
};

#endif/*__M_GRAPH_H__*/
