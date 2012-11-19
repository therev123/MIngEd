/* MessageSystem.cpp
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

// Standard includes
#include "MUtil.h"
#include "MMessageSystem.h"

// Project includes

// Engine includes
#include <MEngine.h>

// System includes
#include <algorithm> // std::find

MMessageSystem MMessageSystem::Instance;

MMessageSystem::MMessageSystem()
{
}

MMessage MMessageSystem::RegisterMessage(const char* message)
{
    // For now, we will just hash, we could look up to
    // make sure we don't get hash clashes...
    return Util::Hash(message);
}

MMessage RegisterMessage(const char* message)
{
    return MMessageSystem::Instance.RegisterMessage(message);
}

MObserver::~MObserver()
{
    std::vector<MSubject*>::iterator iSub;
    for(iSub = m_Subjects.begin(); iSub != m_Subjects.end(); ++iSub)
        (*iSub)->DetachObserver(this);

    m_Subjects.clear();
}

void MObserver::AttachToSubject(MSubject* subject)
{
    std::vector<MSubject*>::iterator iSub = std::find(m_Subjects.begin(), m_Subjects.end(), subject);

    if(iSub != m_Subjects.end())
        m_Subjects.erase(iSub);
}

void MSubject::AttachObserver(MObserver* observer)
{
    // if we're not already being observed by
    // the one being added - then add to the list
    if(std::find(   m_Observers.begin(),
        m_Observers.end(),
        observer) == m_Observers.end())
    {
        m_Observers.push_back(observer);
    }
    observer->AttachToSubject(this);
}
void MSubject::DetachObserver(MObserver* observer)
{
    // find the correct observer to remove
    observerVecIter iObserver =
        std::find(	m_Observers.begin(),
        m_Observers.end(),
        observer);
    if(iObserver != m_Observers.end())
    {
        m_Observers.erase(iObserver);
    }
}
void MSubject::SendMessage(MMessage message, int param)
{
    // for now, we just send everything to everyone
    // and let them deal with filtering. This isn't
    // an ideal solution, we probably want to extend
    // AttachObserver so we can subscribe to specific
    // messages. 
    observerVecIter iObserver;
    for(iObserver = m_Observers.begin();
        iObserver != m_Observers.end();
        iObserver++)
    {
        (*iObserver)->OnMessage(message, param);
    }
}
