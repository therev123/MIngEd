/* MessageSystem.h
version 0.0.1, February 12th, 2012

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

#ifndef __MESSAGE_SYSTEM_H__
#define __MESSAGE_SYSTEM_H__

typedef unsigned int MMessage;

//----------------------------------------
// Message System
//
// Currently just a simple wrapper for an
// ID. Can be extended to a more complete
// message system
//----------------------------------------
class M_UTIL_EXPORT MMessageSystem
{
public:
    MMessage RegisterMessage(const char* message);

    static MMessageSystem Instance;
private:
    MMessageSystem();
    MMessageSystem(const MMessageSystem& o);
    ~MMessageSystem(){}
};

// RegisterMessage
// quick accessor for the message system
MMessage MRegisterMessage(const char* message);


// DECLARE_MESSAGE
// this macro should be used in header files to
// expose messages you wish to be sent externally
#define MDECLARE_MESSAGE(msg) \
    extern MMessage msg;

// REGISTER_MESSAGE
// this macro should go into the source file
#define MREGISTER_MESSAGE(msg) \
    MMessage msg = MRegisterMessage(#msg);

#include <vector>

class MSubject;
//----------------------------------------
// MObserver
// 
// Currently just an interface for subject
// to send messages to
// Could be extended to automatically
// remove itself from subjects on destruction
//----------------------------------------
class M_UTIL_EXPORT MObserver
{
public:
    ~MObserver();

    virtual void OnMessage(MMessage message, int param) = 0;

    void AttachToSubject(MSubject* subject);

private:
    std::vector<MSubject*> m_Subjects;
};

//----------------------------------------
// MSubject
// 
// Base class for anything wanting to send
// messages
//----------------------------------------
class MSubject
{
public:
    void AttachObserver(MObserver* observer);
    void DetachObserver(MObserver* observer);

protected:
    void SendMessage(MMessage message, int param = 0);

private:
    typedef std::vector<MObserver*>	observerVec;
    typedef observerVec::iterator	observerVecIter;

    observerVec		m_Observers;
};

#endif /*__MESSAGE_SYSTEM_H__*/
