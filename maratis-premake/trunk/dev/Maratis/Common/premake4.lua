    project "Common"

        kind "StaticLib"
        language "C++"

        files { "**.h", "**.cpp" }

	includedirs { "../../MSDK/MCore/Includes/",
		   "../../MSDK/MEngine/Includes/",
		   "../../MSDK/MGui/Includes/",
		   "../../../../3rdparty/freetype/include/",
		   "../../../../3rdparty/tinyxml/",
		   "../../../../3rdparty/bullet/",
		   "../../../../3rdparty/glee/",
		   "../../../../3rdparty/devil/",
		   "../../../../3rdparty/lua/",
		   "../../../../3rdparty/libsndfile/include/",
		   "../../../../3rdparty/npk/include/"}
	libdirs { "../../build/" }
	links { "MCore", "MEngine", "MGui", "glee", "tinyxml", "npk", "OpenAL32" }
	defines { "M_PACKAGE_WRITABLE" }


	if os.is("windows") then
	   defines { "_WIN32" }
	   links { "OpenGL32" }
	   includedirs { "../../../../3rdparty/openal/include/" }
	elseif os.is("macosx") then
	   defines { "__APPLE__" }
	   links { "OpenGL.framework" }
	elseif os.is("linux") then
	   defines { "LINUX" }
	   links { "GL", "dl" }
	end


	targetdir("../../build/")

	configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }

	configuration "Debug"
	defines { "DEBUG" }
	flags { "Symbols" }
