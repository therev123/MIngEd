/* MInvoke.h
  version 0.0.1, August 1st, 2012

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
   0.0.1 - 01.08.2012 - First implementation of C#-esque invoke system. - PG
*/

#ifndef __INVOKE_H__
#define __INVOKE_H__

#ifdef M_CAN_INVOKE
# ifdef IN_GAME_CLOCK_H

#include "MUtil.h"
#include "MObject.h"

typedef void(*func)(void);

//--------------------------------------------
// MInvoke types
// 
// used internally. Can be safely ignored
//--------------------------------------------
class _MInvoke
{
public:
    _MInvoke(clocktime t)
    : _time(t) {}
    void Tick(clocktime dt)
    {
	_time -= dt;
	if(_time <= 0)
	    MInvoke();
    }
    virtual void MInvoke() = 0;
    virtual bool Zombie() const { return _time <= 0; }
    virtual bool Is(const char* type) = 0;
private:
    clocktime _time;
};

class MFunction : public _MInvoke
{
public:
    MFunction(func f, clocktime t)
    : _function(f)
    , _MInvoke(t)
    {}

    virtual void MInvoke() { if(_function) _function(); }
    virtual bool Zombie() const { return !_function || _MInvoke::Zombie(); }
    virtual bool Is(const char* type) { return strcmp(type, "Function") == 0; }

    bool Compare(func f) { return f == _function; }
private:
    func _function;
};

template <typename MObjectType>
class MMethod : public _MInvoke
{
public:
	MMethod(MObjectType* o, void (MObjectType::*m)(void), clocktime t)
	: _object(o), _method(m), _MInvoke(t) {}
	void MInvoke()
	{
		(*_object.*_method)();
	}
	void (MObjectType::*_method)(void);
	MObjectType* _object;

    virtual bool Zombie() const { return !_method || !_object || _MInvoke::Zombie(); }
    virtual bool Is(const char* type) { return strcmp(type, "Method") == 0; }
   
    bool Compare(MObjectType* o, void (MObjectType::*m)(void)) { return o == _object && m == _method; }
};
# else
#  pragma message WARN("Looks like you're trying to include MInvoke.h manually. This won't work")
# endif
#endif

#endif/*__INVOKE_H__*/
