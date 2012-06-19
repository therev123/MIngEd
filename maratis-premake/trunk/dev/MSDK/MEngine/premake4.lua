    project "MEngine"

        kind "SharedLib"
        language "C++"

        files { "Includes/**.h", "Sources/**.cpp" }

	includedirs { "../MCore/Includes/" }
	libdirs { "../../build/" }
	links { "MCore" }
		
	defines { "MENGINE_DLL" }

	targetdir("../../build/")

	configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }

	configuration "Debug"
	defines { "DEBUG" }
	flags { "Symbols" }

