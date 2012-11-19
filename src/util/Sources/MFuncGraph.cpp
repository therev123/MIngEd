#include "MUtil.h"
#include "MGraph.h"

#include "tlmm.h"
class MFuncGraph : public MGraph
{
public:
    MFuncGraph();
    ~MFuncGraph();
    float getValue(float ref);
private:
    tlmmProgram* m_equation;
};

MFuncGraph::MFuncGraph()
{
    m_equation = tlmmInitProgram();
}

MFuncGraph::~MFuncGraph()
{
    tlmmTerminateProgram(&m_equation);
}

float MFuncGraph::getValue(float ref)
{
    return tlmmGetValue(m_equation, ref);
}
