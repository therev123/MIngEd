solution "maratis-minged"
    project "zlib"

        kind "StaticLib"
        language "C++"

        files { "**.h", "**.c" }

	targetdir("../../../build/")

	configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }

	configuration "Debug"
	defines { "DEBUG" }
	flags { "Symbols" }
