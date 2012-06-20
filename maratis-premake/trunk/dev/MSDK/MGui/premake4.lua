solution "maratis-minged"
    project "MGui"

        kind "SharedLib"
        language "C++"

        files { "Includes/*.h", "Sources/*.cpp" }

		if os.is("windows") then
		   files { "Includes/WIN32/*.h" }
		   files { "Sources/WIN32/*.cpp" }
		   links { "opengl32", "winmm" }
		elseif os.is("macosx") then
		   files { "Includes/COCOA/*.h" }
		   files { "Sources/COCOA/*.mm" }
		   links { "Cocoa.framework", "Foundation.framework" }
		elseif os.is("linux") then
		   files { "Includes/X11/*.h" }
		   files { "Sources/X11/*.cpp" }
		end
		
		defines { "MGUI_DLL" }

		includedirs { "../MCore/Includes/", "Includes" }
		libdirs { "../../../../../build/" }
		links { "MCore" }

		targetdir("../../../../../build/")

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

