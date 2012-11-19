#ifndef __STATE_MACHINE_H__
#define __STATE_MACHINE_H__

#include <map>

class MStateMachine
{
public:
MStateMachine() : m_CurrentState(0), m_NextState(0) {}
    
    class State
    {
    public:
        State() : m_StateMachine(0) {}
	virtual void Enter() {}
	virtual void Update() {}
	virtual void Exit() {}
	
	void SetStateMachine(MStateMachine* machine) 
	{ m_StateMachine = machine; }
	
    protected:
	void Transition(int state) 
	{ if(m_StateMachine) m_StateMachine->Transition(state); }
	
	MStateMachine* ExposeMStateMachine() { return m_StateMachine; }
	
    private:
	MStateMachine* m_StateMachine;
    };
    
    void AddState(State* state, int id);
    
    void UpdateStateMachine();
    
    void Transition(int id);
    
    State* GetCurrentState() { return m_CurrentState; }
private:
    typedef std::map<int, State*> stateMap;
    typedef stateMap::iterator    stateMapIter;
    
    stateMap m_States;
    State*   m_NextState;
    State*   m_CurrentState;
};

#endif/*__STATE_MACHINE_H__*/
