solution "maratis-minged"

-- BEGINNING OF MCORE BUILD
project "MCore"

kind "SharedLib"
language "C++"

files { "src/core/Includes/**.h",
	"src/core/Sources/**.cpp" }

if os.is("macosx") then
   links { "CoreServices.framework" }
elseif os.is("windows") then
   defines { "WIN32" }
   includedirs { "3rdparty/dirent/include/" }
end
defines { "MCORE_DLL" }

configuration "Release"
defines { "NDEBUG" }
flags { "Optimize" }

configuration "Debug"
defines { "DEBUG", "M_SHOULD_PROFILE" }
flags { "Symbols" }
-- END OF MCORE BUILD

-- BEGINNING OF MENGINE BUILD
project "MEngine"

kind "SharedLib"
language "C++"

files { "src/engine/Includes/**.h",
	"src/engine/Sources/**.cpp" }

includedirs { "src/engine/Includes",
	      "src/core/Includes",
	      "src/gui/Includes" }
defines { "MPLUGIN_DYNAMIC" }

if os.is("windows") then
   defines { "WIN32" }
end
links { "MCore", "MGui" }
defines { "MENGINE_DLL" }

configuration "Release"
defines { "NDEBUG" }
flags { "Optimize" }

configuration "Debug"
defines { "DEBUG", "M_SHOULD_PROFILE" }
flags { "Symbols" }
-- END OF MENGINE BUILD

-- BEGINNING OF MGUI BUILD
project "MGui"

kind "SharedLib"
--kind "StaticLib"
language "C++"

files { "src/gui/Includes/*.h",
	"src/gui/Sources/*.cpp" }

includedirs { "src/core/Includes" }

links { "MCore" }
defines { "MGUI_DLL" }

if os.is("windows") then
   -- don't like this, but meh
   includedirs { "src/gui/Includes" }
   files { "src/gui/Includes/WIN32/*.h",
	   "src/gui/Sources/WIN32/*.cpp" }
   links { "opengl32", "winmm" }
   defines { "WIN32" }
elseif os.is("macosx") then
   files { "src/gui/Includes/COCOA/*.h",
	   "src/gui/Sources/COCOA/*.mm" }
   links { "Cocoa.framework", "Foundation.framework" }
elseif os.is("linux") then
   files { "src/gui/Includes/X11/*.h",
	   "src/gui/Sources/X11/*.cpp" }
end

configuration "Release"
defines { "NDEBUG" }
flags { "Optimize" }

configuration "Debug"
defines { "DEBUG", "M_SHOULD_PROFILE" }
flags { "Symbols" }
-- END OF MGUI BUILD

-- BEGINNING OF COMMON BUILD
project "Common"

kind "StaticLib"
language "C++"

files { "src/common/**.h", 
	"src/common/**.cpp" }

includedirs {   "src/core/Includes/",
		"src/engine/Includes/",
		"src/gui/Includes/",
		"3rdparty/tinyxml/",
		"3rdparty/bullet/",
		"3rdparty/glee/",
		"3rdparty/devil/",
		"3rdparty/lua/",
		"3rdparty/libsndfile/include/",
		"3rdparty/npk/include/"}

if os.is("linux") then
   defines { "linux" }
   includedirs { "/usr/include/freetype2" } 
else
   includedirs { "maratis/3rdparty/freetype/include/" } 
end

links { "MCore", "MEngine", "MGui", "glee", "tinyxml", "npk", "OpenAL32" }
defines { "M_PACKAGE_WRITABLE" }

if os.is("windows") then
   defines { "WIN32", "IL_STATIC_LIB" }
   includedirs { "3rdparty/openal/include/" }
   libdirs { "3rdparty/openal/win32/" }
end

configuration "Release"
defines { "NDEBUG" }
flags { "Optimize" }

configuration "Debug"
defines { "DEBUG", "M_SHOULD_PROFILE" }
flags { "Symbols" }
-- END OF COMMON BUILD
