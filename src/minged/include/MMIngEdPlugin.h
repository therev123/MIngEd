#pragma once
#ifndef __MINGED_PLUGIN_H__
#define __MINGED_PLUGIN_H__

#include <MEngine.h>
#include <MPlugin.h>

#define PPCAT_NX(A,B) A ## B
#define PPCAT(A,B) PPCAT_NX(A,B)
#define STRINGIZE_NX(A) #A
#define STRINGIZE(A) STRINGIZE_NX(A)
#define STRINGIZE_CAT(A,B) STRINGIZE(PPCAT(A,B))

#ifdef MPLUGIN_DYNAMIC
#include "MEmbedFile.h"
#  define MINGED_PLUGIN_DECLARE(x)		\
    MPLUGIN_DECLARE(x)				\
    extern "C" {				\
    MPLUGIN_EXPORT void StartEditor();		\
    MPLUGIN_EXPORT void EndEditor();		\
    }

#  define MINGED_PLUGIN_START_IMPLEMENT(x)				\
    MPackage s_Data = 0;						\
    void StartEditor()							\
    {									\
	MEngine* engine = MEngine::getInstance();			\
	if(engine->getEmbedFileManager())				\
	    engine->getEmbedFileManager()->AddEmbeddedFile(#x".npk", x##_npk, x##_npkSize()); \
	if(engine->getPackageManager())					\
	    s_Data = engine->getPackageManager()->loadPackage(#x".npk"); \
	if(engine->getScriptContext())					\
	    engine->getScriptContext()->addScript("editor/"#x"/__init__.lua"); \
    }									\
    MPLUGIN_START_IMPLEMENT(x)

#  define MINGED_PLUGIN_END_IMPLEMENT(x)				\
    void EndEditor()							\
    {									\
        MEngine* engine = MEngine::getInstance();			\
	if(s_Data && engine && engine->getPackageManager())		\
	    engine->getPackageManager()->unloadPackage(s_Data);		\
	s_Data = 0;							\
    }									\
    MPLUGIN_END_IMPLEMENT(x)
#endif/*MPLUGIN_DYNAMIC*/

#endif/*__MINGED_PLUGIN_H__*/
