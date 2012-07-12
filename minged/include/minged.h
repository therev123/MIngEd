
#ifndef __MINGED_H__
#define __MINGED_H__

#ifdef WIN32
#   include <windows.h>
#   define MINGED_EXPORT __declspec(dllexport)
#else
// automagically exported
#   define MINGED_EXPORT
#endif /*WIN32*/

#include <MEngine.h>
#include <MPlugin.h>

MPLUGIN_DECLARE(minged);

// don't munge the symbol names
#ifdef __cplusplus
extern "C" {
#endif
  //#define MINGED_USE_PTHREAD

  MINGED_EXPORT void Render();
  MINGED_EXPORT void EndPlugin();

  typedef unsigned int uint32;

#ifdef __cplusplus
}
#endif

#endif/*__MINGED_H__*/
