/* MGameClock.cpp
  version 0.0.2, February 12th, 2012

  Copyright (C) 2012 Philipp Geyer

  This software is provided 'as-is', without any express or implied
  warranty.  In no event will the authors be held liable for any damages
  arising from the use of this software.

  Permission is granted to anyone to use this software for any purpose,
  including commercial applications, and to alter it and redistribute it
  freely, subject to the following restrictions:

  1. The origin of this software must not be misrepresented; you must not
     claim that you wrote the original software. If you use this software
     in a product, an acknowledgment in the product documentation would be
     appreciated but is not required.
  2. Altered source versions must be plainly marked as such, and must not be
     misrepresented as being the original software.
  3. This notice may not be removed or altered from any source distribution.

  Philipp Geyer
*/

#include "MGameClock.h"

#include "MTimer.h"
#include "MUtil.h"

#include <algorithm>
#include <MEngine.h>

MClockID CLOCK_MAIN = Util::Hash("CLOCK_MAIN");
MGameClock::clockMap MGameClock::m_Clocks;

//----------------------------------------
// getCurTime
// quick helper function to lookup tick
// time
//----------------------------------------
clocktime getCurTime()
{
	MEngine* engine = MEngine::getInstance();
	MSystemContext* system = engine->getSystemContext();

	return system->getSystemTick();
}
//----------------------------------------
MGameClock::MGameClock()
{
	m_StartTime = getCurTime();
	m_CurTime = m_StartTime;
	m_Delta = 0;
	m_Scale = 1.0f;
}
//----------------------------------------
void MGameClock::_update()
{
    // get the new time and then work out the delta
    clocktime prevTime = m_CurTime;
    m_CurTime = getCurTime();
    m_Delta = (clocktime)(((float)(m_CurTime - prevTime)) * m_Scale);
    
    // update any attached timers
    for(timerVecIter iTimer = m_Timers.begin(); 
	iTimer != m_Timers.end(); 
	iTimer++)
    {
	(*iTimer)->Update(m_Delta);
    }
    
#ifdef CAN_INVOKE
    for(invokeListIter iInvoke = m_Invokes.begin();
	iInvoke != m_Invokes.end();
	iInvoke++)
	if((*iInvoke)->Zombie())
	{
	    delete *iInvoke;
	    invokeListIter toErase = iInvoke;
	    iInvoke--;		
	    m_Invokes.erase(toErase);
	}
	else
	{
	    (*iInvoke)->Tick(m_Delta);
	}
#endif
}
//----------------------------------------
MTimer* MGameClock::CreateTimer(int id)
{
    // create a new timer and push it
    // back onto the update list
    MTimer* timer = new MTimer(id);
    m_Timers.push_back(timer);
    return timer;
}
//----------------------------------------
void MGameClock::DestroyTimer(MTimer* timer)
{
    // try to find the timer and remove from
    // the update list
    timerVecIter iTimer = std::find(m_Timers.begin(), 
				    m_Timers.end(), 
				    timer);
    if(iTimer != m_Timers.end())
    {
	m_Timers.erase(iTimer);
    }
    
    // regardless of whether it was in the
    // list, we need to delete the timer
    delete timer;
}
#ifdef CAN_INVOKE
//----------------------------------------
void MGameClock::Invoke(func cb, clocktime t)
{
    Function* f = new Function(cb, t);
    m_Invokes.push_back(f);
}
//----------------------------------------
void MGameClock::CancelInvoke(func cb)
{
    for(invokeListIter iInvoke = m_Invokes.begin();
	iInvoke != m_Invokes.end();
	iInvoke++)
    {
	if((*iInvoke) && (*iInvoke)->Is("Function"))
	{
	    if( ((Function*)*iInvoke)->Compare(cb) )
	    {
		delete *iInvoke;
		m_Invokes.erase(iInvoke);
		return;
	    }
	}
    }
}
#endif
//----------------------------------------
clocktime MGameClock::GetDeltaMs()
{
    return m_Delta;
}
//----------------------------------------
void MGameClock::SetScale(float s)
{
    m_Scale = s;
}
//----------------------------------------
MGameClock* MGameClock::GetClock(MClockID id)
{
    clockMapIter iClock = m_Clocks.find(id);
    if(iClock != m_Clocks.end())
	return iClock->second;
    return 0;
}
//----------------------------------------
void MGameClock::SetClock(MGameClock* clock, MClockID id)
{
    m_Clocks[id] = clock;
}
//----------------------------------------
void MGameClock::Update()
{
    for(clockMapIter iClock = m_Clocks.begin();
	iClock != m_Clocks.end();
	++iClock)
	iClock->second->_update();
}
