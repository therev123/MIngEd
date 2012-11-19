#ifndef __MUTIL_H__
#define __MUTIL_H__

#ifdef WIN32
# if defined(MUTIL_DLL)
#  define M_UTIL_EXPORT __declspec( dllexport )
# elif defined(MUTIL_STATIC)
#  define M_UTIL_EXPORT
# else
#  define M_UTIL_EXPORT __declspec( dllimport )
# endif
#else
# define M_UTIL_EXPORT
#endif

//--------------------------------------------
// Util namespace
// just somewhere to put some generally useful
// functions
//--------------------------------------------
namespace Util
{
	//----------------------------------------
	// Hash
	// A general purpose hash function
	// http://www.partow.net/programming/hashfunctions/
	//----------------------------------------
	unsigned int Hash(const char* string);

	// returns smallest power of 2 which is greater than the value supplied
	int Pow2(int x);
};


#include "c_tokenizer.h"

//--------------------------------------------
// typedef's
//--------------------------------------------
typedef long int clocktime;
typedef unsigned int MClockID;

//--------------------------------------------
// Module includes
//--------------------------------------------
#include "MObject.h"
#include "MBehaviourDB.h"
#include "MFlags.h"
#include "MGameClock.h"
#include "MInputManager.h"
#include "MMessageSystem.h"
#include "MStateMachine.h"
#include "MTimer.h"

#include <MCore.h>
#include <MFileTools.h>
#include <stdio.h>

#ifdef WIN32
# ifndef max
#  define max(x, y) (x > y ? x : 0)
# endif

# ifndef min
#  define min(x, y) (x < y ? x : 0)
# endif

# ifndef clamp
#  define clamp(x, low, hi) (max(min(x, hi), low))
# endif
#endif

#define STRINGISE_IMPL(x) #x
#define STRINGISE(x) STRINGISE_IMPL(x)

// http://stackoverflow.com/questions/471935/user-warnings-on-msvc-and-gcc
// Use: #pragma message WARN("My message")
#if _MSC_VER
#   define FILE_LINE_LINK __FILE__ "(" STRINGISE(__LINE__) ") : "
#   define WARN(exp) (FILE_LINE_LINK "WARNING: " exp)
#else//__GNUC__ - may need other defines for different compilers
#   define WARN(exp) ("WARNING: " exp)
#endif


#endif/*__MUTIL_H__*/
