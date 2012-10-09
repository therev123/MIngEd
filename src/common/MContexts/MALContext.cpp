/////////////////////////////////////////////////////////////////////////////////////////////////////////
// MCore
// MSOpenALContext.cpp
/////////////////////////////////////////////////////////////////////////////////////////////////////////

//========================================================================
// Copyright (c) 2003-2011 Anael Seghezzi <www.maratis3d.com>
//
// This software is provided 'as-is', without any express or implied
// warranty. In no event will the authors be held liable for any damages
// arising from the use of this software.
//
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
//
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would
//    be appreciated but is not required.
//
// 2. Altered source versions must be plainly marked as such, and must not
//    be misrepresented as being the original software.
//
// 3. This notice may not be removed or altered from any source
//    distribution.
//
//========================================================================


#include <stdio.h>
#include "MALContext.h"

#include <MEngine.h>

ALenum returnALFormat(M_SOUND_FORMAT format)
{
    M_PROFILE_SCOPE(returnALFormat);
	switch(format)
	{
	default:
	case M_SOUND_FORMAT_MONO8 :
		return AL_FORMAT_MONO8;

	case M_SOUND_FORMAT_STEREO8 :
		return AL_FORMAT_STEREO8;

	case M_SOUND_FORMAT_MONO16 :
		return AL_FORMAT_MONO16;

	case M_SOUND_FORMAT_STEREO16 :
		return AL_FORMAT_STEREO16;
	}
}

MALContext::MALContext(void)
{
    M_PROFILE_SCOPE(MALContext::MALContext);
    // device
	m_device = alcOpenDevice(NULL);
    if(! m_device)
	{
		printf("ERROR OpenAL : unable to create device\n");
        return;
	}

    // context
	m_context = alcCreateContext(m_device, NULL);
    if(! m_context)
	{
		printf("ERROR OpenAL : unable to create context\n");
        return;
	}

	// init context
	initContext();
}

MALContext::~MALContext(void)
{
    M_PROFILE_SCOPE(MALContext::~MALContext);
    ALCcontext * context = alcGetCurrentContext();
    ALCdevice * device = alcGetContextsDevice(context);

    alcMakeContextCurrent(NULL);
    alcDestroyContext(context);
    alcCloseDevice(device);
}

void MALContext::initContext(void)
{
    M_PROFILE_SCOPE(MALContext::initContext);
	// make current context
    if(! alcMakeContextCurrent(m_context))
	{
		printf("ERROR OpenAL : unable to make current context\n");
        return;
	}
}

void MALContext::createBuffer(unsigned int * bufferId)
{
    M_PROFILE_SCOPE(MALContext::createBuffer);
	alGenBuffers(1, bufferId);
}

void MALContext::deleteBuffer(unsigned int * bufferId)
{
    M_PROFILE_SCOPE(MALContext::deleteBuffer);
	if(*bufferId > 0)
	{
		// unlink sources
		map<unsigned int, unsigned int>::iterator
		mit (m_sources.begin()),
		mend(m_sources.end());

		for(;mit!=mend;++mit)
		{
		  if(mit->second == (*bufferId))
		  {
			  stopSource(mit->first);
			  setSourceBufferId(mit->first, 0);
		  }
		}	

		// delete buffer
		alDeleteBuffers(1, bufferId);
	}
}

void MALContext::sendBufferSound(unsigned int bufferId, MSound * sound)
{
    M_PROFILE_SCOPE(MALContext::sendBufferSound);
	if(! sound)
		return;

	// properties
	ALenum format = returnALFormat(sound->getFormat());

    // send data
	alBufferData(bufferId, format, sound->getData(), sound->getSize(), sound->getSampleRate());

	ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
       printf("ERROR OpenAL : unable to send sound data\n");
	   return;
    }
}

void MALContext::sendBufferData(unsigned int bufferId, M_SOUND_FORMAT format, void * data, unsigned int size, unsigned int freq)
{
    M_PROFILE_SCOPE(MALContext::sendBufferData);
	// properties
	ALenum alFormat = returnALFormat(format);

    // send data
	alBufferData(bufferId, alFormat, data, size, freq);

	ALenum error = alGetError();
    if(error != AL_NO_ERROR)
    {
       printf("ERROR OpenAL : unable to send sound data\n");
	   return;
    }
}

float MALContext::getBufferDuration(unsigned int bufferId)
{
    M_PROFILE_SCOPE(MALContext::getBufferDuration);
	int size;
	int bits;
	int freq;

	alGetBufferi(bufferId, AL_SIZE, &size);
	alGetBufferi(bufferId, AL_BITS, &bits);
	alGetBufferi(bufferId, AL_FREQUENCY, &freq);

	float duration = size / (float)((bits/8.0f) * freq);
	return duration;
}

// source
void MALContext::createSource(unsigned int * sourceId, unsigned int bufferId)
{
    M_PROFILE_SCOPE(MALContext::createSource);
	alGenSources(1, sourceId);
	setSourceBufferId(*sourceId, bufferId);

	// init velocity
	ALfloat sourceVel[] = {0.0, 0.0, 0.0};
	alSourcefv(*sourceId, AL_VELOCITY, sourceVel);

	m_sources[*sourceId] = bufferId;
}

void MALContext::deleteSource(unsigned int * sourceId)
{
    M_PROFILE_SCOPE(MALContext::deleteSource);
	if(*sourceId > 0)
	{
		map<unsigned int, unsigned int>::iterator iter = m_sources.find(*sourceId);
		if(iter != m_sources.end())
			m_sources.erase(iter);
	
		alDeleteSources(1, sourceId);
	}
}

void MALContext::enableSourceLoop(unsigned int sourceId)
{
    M_PROFILE_SCOPE(MALContext::enableSourceLoop);
	alSourcei(sourceId, AL_LOOPING, 1);
}

void MALContext::disableSourceLoop(unsigned int sourceId)
{
    M_PROFILE_SCOPE(MALContext::disableSourceLoop);
	alSourcei(sourceId, AL_LOOPING, 0);
}

void MALContext::setSourceRelative(unsigned int sourceId, bool relative)
{
    M_PROFILE_SCOPE(MALContext::setSourceRelative);
	alSourcei(sourceId, AL_SOURCE_RELATIVE, (ALint)relative);
}

void MALContext::setSourceBufferId(unsigned int sourceId, unsigned int bufferId)
{
    M_PROFILE_SCOPE(MALContext::setSourceBufferId);
	alSourcei(sourceId, AL_BUFFER, bufferId);
	m_sources[sourceId] = bufferId;
}

void MALContext::setSourcePosition(unsigned int sourceId, const MVector3 & position)
{
    M_PROFILE_SCOPE(MALContext::setSourcePosition);
	alSourcefv(sourceId, AL_POSITION, position);
}

void MALContext::setSourceRadius(unsigned int sourceId, float radius)
{
    M_PROFILE_SCOPE(MALContext::setSourceRadius);
	// the distance under which the volume for the source would normally drop by half
	alSourcef(sourceId, AL_REFERENCE_DISTANCE, radius);
}

void MALContext::setSourcePitch(unsigned int sourceId, float pitch)
{
    M_PROFILE_SCOPE(MALContext::setSourcePitch);
	alSourcef(sourceId, AL_PITCH, pitch);
}

void MALContext::setSourceGain(unsigned int sourceId, float gain)
{
    M_PROFILE_SCOPE(MALContext::setSourceGain);
	alSourcef(sourceId, AL_GAIN, gain);
}

void MALContext::setSourceRolloff(unsigned int sourceId, float rolloff)
{
    M_PROFILE_SCOPE(MALContext::setSourceRolloff);
	alSourcef(sourceId, AL_ROLLOFF_FACTOR, rolloff);
}

void MALContext::setSourceOffset(unsigned int sourceId, float offset)
{
    M_PROFILE_SCOPE(MALContext::setSourceOffset);
	alSourcef(sourceId, AL_SEC_OFFSET, offset);
}

float MALContext::getSourceOffset(unsigned int sourceId)
{
    M_PROFILE_SCOPE(MALContext::getsourceOffset);
	float time;
	alGetSourcef(sourceId, AL_SEC_OFFSET, &time);
	return time;
}

void MALContext::playSource(unsigned int sourceId)
{
    M_PROFILE_SCOPE(MALContext::playSource);
	alSourcePlay(sourceId);
}

void MALContext::pauseSource(unsigned int sourceId)
{
    M_PROFILE_SCOPE(MALContext::pauseSource);
	alSourcePause(sourceId);
}

void MALContext::stopSource(unsigned int sourceId)
{
    M_PROFILE_SCOPE(MALContext::stopSource);
	alSourceStop(sourceId);
}

bool MALContext::isSourcePaused(unsigned int sourceId)
{
    M_PROFILE_SCOPE(MALContext::isSourcePaused);
	ALint state;
	alGetSourcei(sourceId, AL_SOURCE_STATE, &state);

	if(state == AL_PAUSED)
		return true;
	else
		return false;
}

bool MALContext::isSourcePlaying(unsigned int sourceId)
{
    M_PROFILE_SCOPE(MALContext::isSourcePlaying);
	ALint state;
	alGetSourcei(sourceId, AL_SOURCE_STATE, &state);

	if(state == AL_PLAYING)
		return true;
	else
		return false;
}

float MALContext::getSourceTimePos(unsigned int sourceId)
{
    M_PROFILE_SCOPE(MALContext::getSourceTimePos);
	int bits;
	int freq;
	int bOffset;

	ALint buffer;
	alGetSourcei(sourceId, AL_BUFFER, &buffer);
	alGetSourcei(sourceId, AL_BYTE_OFFSET, &bOffset);

	alGetBufferi(buffer, AL_BITS, &bits);
	alGetBufferi(buffer, AL_FREQUENCY, &freq);

	float timePos = bOffset / (float)((bits/8.0f) * freq);
	return timePos;
}

void MALContext::queueSourceBuffer(unsigned int sourceId, unsigned int * buffers, unsigned int bufferSize)
{
    M_PROFILE_SCOPE(MALContext::queueSourceBuffer);
	alSourceQueueBuffers(sourceId, bufferSize, buffers);
}

void MALContext::unqueueSourceBuffer(unsigned int sourceId, unsigned int * buffers, unsigned int bufferSize)
{
    M_PROFILE_SCOPE(MALContext::unqueueSourceBuffer);
	alSourceUnqueueBuffers(sourceId, bufferSize, buffers);
}

unsigned int MALContext::getSourceBuffersQueued(unsigned int sourceId)
{
    M_PROFILE_SCOPE(MALContext::getSourceBuffersQueued);
	ALint queued = 0;
    alGetSourcei(sourceId, AL_BUFFERS_QUEUED, &queued);
	return (unsigned int)queued;
}

unsigned int MALContext::getSourceBuffersProcessed(unsigned int sourceId)
{
    M_PROFILE_SCOPE(MALContext::getSourceBuffersProcessed);
	ALint processed = 0;
    alGetSourcei(sourceId, AL_BUFFERS_PROCESSED, &processed);
	return (unsigned int)processed;
}

// listener
void MALContext::updateListenerPosition(const MVector3 & position, const MVector3 & direction, const MVector3 & up)
{
    M_PROFILE_SCOPE(MALContext::updateListenerPosition);
	ALfloat ListenerPos[] = { position.x, position.y, position.z };
	ALfloat ListenerVel[] = { 0.0, 0.0, 0.0 };
	ALfloat ListenerOri[] = { 
		direction.x, direction.y, direction.z, 
		up.x, up.y, up.z
	};

	alListenerfv(AL_POSITION,    ListenerPos);
	alListenerfv(AL_VELOCITY,    ListenerVel);
	alListenerfv(AL_ORIENTATION, ListenerOri);
}
