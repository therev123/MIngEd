solution "maratis-minged"
project "bullet"
        kind "StaticLib"
        language "C++"

        files { "**.h", "**.cpp" }

	targetdir("../../../build/")
	includedirs {"."}

	configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }

	configuration "Debug"
	defines { "DEBUG" }
	flags { "Symbols" }
