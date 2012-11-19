/* MGameClock.h
  version 0.0.2, August 1st, 2012

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

/* Changelog
   0.0.2 - 01.08.2012 - Added basic C#-esque invoke system and support for
                        multiple clocks to run at the same time
   0.0.1 - 12.02.2012 - First implementation of Game Clock for tutorial at
                        http://nistur.com - PG
*/

#ifndef __GAME_CLOCK_H__
#define __GAME_CLOCK_H__

#define M_CAN_INVOKE

#include <vector>
#include <list>
#include <map>

#define IN_GAME_CLOCK_H
# include "MInvoke.h"
#undef IN_GAME_CLOCK_H
class MTimer;

//--------------------------------------------
// Clock IDs
//--------------------------------------------
extern MClockID CLOCK_MAIN;

//--------------------------------------------
// MGameClock
//
// Takes care of any active timers, also
// provides an interface for getting the delta
// between ticks
//--------------------------------------------
class MGameClock
{
public:
	MGameClock();
	
	//----------------------------------------
	// Public interface
	//----------------------------------------
	MTimer*		CreateTimer(int id);
	void		DestroyTimer(MTimer* timer);
	
	#ifdef M_CAN_INVOKE
	void		Invoke(func cb, clocktime t);
	void		CancelInvoke(func cb);
	template <typename T>
	void		Invoke(T* obj, void (T::*cb)(void), clocktime t);	
	template <typename T>
	void		CancelInvoke(T* obj, void (T::*cb)(void));
	#endif

	clocktime	GetDeltaMs();

	void		SetScale(float s);
	float		GetScale() { return m_Scale; }
	
	//----------------------------------------
	// Static interface
	//----------------------------------------
	static void	Update();
	static MGameClock* GetClock(MClockID id);
	static void SetClock(MGameClock* clock, MClockID id);

private:
	//----------------------------------------
	// Internal types
	//----------------------------------------
	typedef std::vector<MTimer*> timerVec;
	typedef timerVec::iterator	timerVecIter;
	#ifdef M_CAN_INVOKE
	typedef std::list<_MInvoke*> invokeList;
	typedef invokeList::iterator invokeListIter;
	#endif
	typedef std::map<MClockID, MGameClock*> clockMap;
	typedef clockMap::iterator clockMapIter;
	
	//----------------------------------------
	// Private interface
	//----------------------------------------
	void _update();
	
	//----------------------------------------
	// Members
	//----------------------------------------
	timerVec	m_Timers;
	#ifdef M_CAN_INVOKE
	invokeList   m_Invokes;
	#endif

	clocktime	m_StartTime;
	clocktime	m_CurTime;
	clocktime	m_Delta;
	float		m_Scale;
	
	//----------------------------------------
	// Static Members
	//----------------------------------------
	static clockMap m_Clocks;
};

#ifdef M_CAN_INVOKE
//--------------------------------------------
// Invoke defines
// 
// These can be used to tie to the main game
// clock.
//
// If invoking a standard function, just do:
// INVOKE(SomeFunction, 100);
// to invoke void SomeFunction() in 100ms
//
// To invoke a class method, do:
// CLASS_INVOKE(SomeClass::SomeFunction, 100);
// to invoke void SomeClass::SomeFunction() in
// 100ms, on the current instance of the
// object
//--------------------------------------------
# define M_INVOKE(fn, t)					\
    if(MGameClock* c = MGameClock::GetClock(CLOCK_MAIN))	\
        c->Invoke(fn, t);

# define M_CANCEL_INVOKE(fn, t)					\
    if(MGameClock* c = MGameClock::GetClock(CLOCK_MAIN))	\
        c->CancelInvoke(fn);

# define M_CLASS_INVOKE(fn, t)						\
    if(MGameClock* c = MGameClock::GetClock(CLOCK_MAIN))		\
        c->Invoke(this, &fn, t);

# define M_CANCEL_CLASS_INVOKE(fn, t)					\
    if(MGameClock* c = MGameClock::GetClock(CLOCK_MAIN))		\
        c->CancelInvoke(this, &fn);

//--------------------------------------------
// Templated inline invoke functions
//--------------------------------------------
template <typename T>
void MGameClock::Invoke(T* obj, void (T::*cb)(void), clocktime t)
{
    m_Invokes.push_back(new MMethod<T>(obj, cb, t));
}

template <typename T>
void MGameClock::CancelInvoke(T* obj, void (T::*cb)(void))
{
    for(invokeListIter iInvoke = m_Invokes.begin();
	iInvoke != m_Invokes.end();
	iInvoke++)
    {
	if((*iInvoke) && (*iInvoke)->Is("Method"))
	{
	    if( ((MMethod<T>*)*iInvoke)->Compare(obj, cb) )
	    {
		delete *iInvoke;
		m_Invokes.erase(iInvoke);
		return;
	    }
	}
    }
}
#else

# define M_INVOKE(fn, t)			\
    fn();

# define M_CLASS_INVOKE(fn, t)			\
    fn();

# define M_CANCEL_CLASS_INVOKE(fn, t) {}
# define M_CANCEL_INVOKE(fn, t) {}

#endif/*M_CAN_INVOKE*/

#endif /*__GAME_CLOCK_H__*/
