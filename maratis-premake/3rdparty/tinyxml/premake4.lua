solution "maratis-minged"
    project "tinyxml"

        kind "StaticLib"
        language "C++"

        files { "**.h", "**.cpp" }

	targetdir("../../../build/")

	configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }

	configuration "Debug"
	defines { "DEBUG" }
	flags { "Symbols" }

