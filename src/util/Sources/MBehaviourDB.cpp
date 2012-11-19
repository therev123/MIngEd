#include "MUtil.h"
#include "MBehaviourDB.h"

MBehaviourDB MBehaviourDB::Instance;

//--------------------------------------------
// Behaviour
//--------------------------------------------
MBehaviour::MBehaviour(MObject3d * parentObject, ID id)
: MBehavior(parentObject) 
, m_started(false)
{
    // On creation, we need to register ourselves
    MBehaviourDB::Instance.RegisterBehaviour(getParentObject(), this, id);
}
//--------------------------------------------
MBehaviour::~MBehaviour()
{
    // We're not needed any more? Make sure we don't stay in
    // the database
    MBehaviourDB::Instance.RemoveBehaviour(getParentObject(), this);
    
    std::map<unsigned int, MVariable*>::iterator iVar = m_Variables.begin();
    
    for(iVar; iVar != m_Variables.end(); ++iVar)
	delete iVar->second;
    
    m_Variables.clear();
}
//--------------------------------------------
unsigned int MBehaviour::getVariablesNumber()
{
    return m_Variables.size();
}
//--------------------------------------------
MVariable MBehaviour::getVariable(unsigned int id)
{
    if(id > m_Variables.size())
	return MVariable("NULL", NULL, M_VARIABLE_NULL);
    
    std::map<unsigned int, MVariable*>::iterator iVar = m_Variables.begin();
    
    for(unsigned int i = 0; i < id; ++i)
	++iVar;
    
    return *iVar->second;  
}
//--------------------------------------------
MBehaviour* MBehaviour::GetBehaviour(ID behaviour)
{
    // Just pass the request onto the database
    return MBehaviourDB::Instance.GetBehaviour(getParentObject(), behaviour);
}
//--------------------------------------------
void MBehaviour::RegisterVariable(MVariable var)
{
    unsigned int hash = Util::Hash(var.getName());
    
    m_Variables[hash] = new MVariable(var.getName(), var.getPointer(), var.getType());
}
//--------------------------------------------
void MBehaviour::UnregisterVariable(MVariable var)
{
    unsigned int hash = Util::Hash(var.getName());
    
    std::map<unsigned int, MVariable*>::iterator iVar = m_Variables.find(hash);
    
    if(iVar != m_Variables.end())
    {
	delete iVar->second;
	m_Variables.erase(iVar);
    }
}
//----------------------------------------
void MBehaviour::update()
{
    MEngine* engine = MEngine::getInstance();
    MGame* game = engine->getGame();
    
    if(game == 0 || !game->isRunning())
    {
        m_started = false;
	return;
    }
    
    if(!m_started)
    {
        Start();
        m_started = true;
    }
    
    Update();
}
//--------------------------------------------
void MBehaviour::OnMessage(MMessage message, int param)
{
    std::map<MMessage, func>::iterator iFunc = m_Functions.find(message);
    if(iFunc != m_Functions.end())
	iFunc->second(param);
}
//--------------------------------------------
void MBehaviour::RegisterFunction(const char* name, func function)
{
    MMessage id = Util::Hash(name);
    m_Functions[id] = function;
}
//--------------------------------------------
// BehaviourDB
//--------------------------------------------
MBehaviour::ID MBehaviourDB::GetBehaviourID(const char* name)
{
    // There are a great many ways we could sort IDs. The
    // simple method used for the messages wouldn't work
    // here as potentially we will call GetBehaviourID
    // multiple times. We could keep a list of already
    // registered behaviours, but that's hassle. Instead
    // we'll just grab a hash of the name. That should
    // be unique enough
    return Util::Hash(name);
}
//--------------------------------------------
MBehaviour* MBehaviourDB::GetBehaviour(MObject3d* obj, MBehaviour::ID id)
{
    // we could just use m_Objects[obj][id] at this point
    // it would even return 0 if there wasn't a behaviour
    // registered. The issue is that it will make empty 
    // entries in our map, which we don't want
    // instead, we'll use map::find to look through the
    // map and pull out what we want
    objectMapIter iObj = m_Objects.find(obj);
    if(iObj != m_Objects.end())
    {
	behaviourMapIter iBehaviour = iObj->second.find(id);
	if(iBehaviour != iObj->second.end())
	{
	    return iBehaviour->second;
	}
    }
    return 0;
}
//--------------------------------------------
void MBehaviourDB::RemoveBehaviour(MObject3d* obj, MBehaviour* behaviour)
{
    // at this point, we can assume that m_Objects[obj] exists
    // so we shouldn't need to do a map::find on that, however
    // we could potentially be looking for a behaviour that
    // has already been removed. So we need to safeguard
    // against that.
    behaviourMapIter iBehaviour = m_Objects[obj].find(behaviour->GetID());
    if(iBehaviour != m_Objects[obj].end())
    {
	m_Objects[obj].erase(iBehaviour);
    }
    
    // If the object is now empty, we can remove it and save some space
    objectMapIter iObj = m_Objects.find(obj);
    if(iObj->second.size() == 0)
    {
	m_Objects.erase(iObj);
    }
}
//--------------------------------------------
void MBehaviourDB::RegisterBehaviour(MObject3d* obj, MBehaviour* behaviour, MBehaviour::ID id)
{
    // absolutely no point in doing any kind of find behaviour here.
    // just stick it in the map, making new entries where needed
    m_Objects[obj][id] = behaviour;
}
//--------------------------------------------
