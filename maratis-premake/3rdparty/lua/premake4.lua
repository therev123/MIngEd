        kind "StaticLib"
        language "C++"

        files { "**.h", "**.c" }

	targetdir("../../trunk/dev/build/")

	configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }

	configuration "Debug"
	defines { "DEBUG" }
	flags { "Symbols" }
