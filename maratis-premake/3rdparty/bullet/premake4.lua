        kind "StaticLib"
        language "C++"

        files { "**.h", "**.cpp" }

	targetdir("../../trunk/dev/build/")
	includedirs {"."}

	configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }

	configuration "Debug"
	defines { "DEBUG" }
	flags { "Symbols" }
