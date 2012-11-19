#include "MStateMachine.h"

void MStateMachine::AddState(State* state, int id)
{
    state->SetStateMachine(this);
    m_States[id] = state;
}

void MStateMachine::UpdateStateMachine()
{
    if(m_NextState)
    {
	if(m_CurrentState)
	    m_CurrentState->Exit();
	m_CurrentState = m_NextState;
	m_NextState= 0;
	if(m_CurrentState)
	    m_CurrentState->Enter();
    }
    else if(m_CurrentState)
	m_CurrentState->Update();
}

void MStateMachine::Transition(int id)
{
    stateMapIter iState = m_States.find(id);
    
    if(iState != m_States.end())
    {
	m_NextState = iState->second;
    }
}
