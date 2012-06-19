    project "MCore"

        kind "SharedLib"
        language "C++"

        files { "Includes/**.h", "Sources/**.cpp" }

	targetdir("../../build/")
		
	if os.is("macosx") then
	   links { "CoreServices.framework" }
	end
	defines { "MCORE_DLL" }

	configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }

	configuration "Debug"
	defines { "DEBUG" }
	flags { "Symbols" }

