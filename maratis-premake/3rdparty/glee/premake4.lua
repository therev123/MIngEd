    project "glee"

        kind "StaticLib"
        language "C++"

        files { "**.h", "**.c" }

	targetdir("../../trunk/dev/build/")

	if os.is("macosx") then
	   links { "OpenGL.framework", "CoreServices.framework", "Foundation.framework" }
	end

	configuration "Release"
	defines { "NDEBUG" }
	flags { "Optimize" }

	configuration "Debug"
	defines { "DEBUG" }
	flags { "Symbols" }
