    project "npk"

        kind "StaticLib"
        language "C++"

        files { "**.h", "**.c" }
		
	includedirs { "include/", "../zlib/" }

	targetdir("../../trunk/dev/build/")
	defines { "M_PACKAGE_WRITABLE" }
	
	links { "zlib" }

	configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }

	configuration "Debug"
	defines { "DEBUG" }
	flags { "Symbols" }
