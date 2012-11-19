#include "MUtil.h"
#include "MGraph.h"

#include <map>

MGraph* MGraph::getGraphNamed(const char* name)
{
    return 0;
}

class MValGraph : public MGraph
{
public:
    MValGraph();
    float getValue(float ref);

    void addValue(float ref, float val);

    void eraseValue(float ref);

private:
    typedef std::map<float, float> valMap;
    typedef valMap::iterator       valMapIter;
    valMap m_Values;
};

MValGraph::MValGraph()
{
    // just add y = x as basic graph
    addValue(0.0f, 0.0f);
    addValue(1.0f, 1.0f);
}

float MValGraph::getValue(float ref)
{
    unsigned int size = m_Values.size();
    if(size == 0)
	return 0.0f;
    if(size == 1)
	return m_Values.begin()->second;

    valMapIter iVal = m_Values.find(ref);
    if(iVal != m_Values.end())
	return iVal->second;
    
    valMapIter iValPrev = m_Values.begin();
    valMapIter iValNext = m_Values.begin();

    if(ref < iValPrev->first)
	return iValPrev->second;

    while(iValNext != m_Values.end())
    {
	iValPrev = iValNext;
	iValNext++;
	if( (ref < iValNext->first) &&
	    (ref > iValPrev->first) )
	{
	    float lowref = iValPrev->first;
	    float hiref  = iValNext->first;
	    float lowval = iValPrev->second;
	    float hival  = iValNext->second;

	    float delta = (ref - lowref) / (hiref - lowref);
	    return lowval + ( (hival - lowval) * delta);
	}
    }
    return iValPrev->second;
}

void MValGraph::addValue(float ref, float val)
{
    m_Values[ref] = val;
}

void MValGraph::eraseValue(float ref)
{
    valMapIter iVal = m_Values.find(ref);
    if(iVal != m_Values.end())
	m_Values.erase(iVal);
}
