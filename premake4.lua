solution "maratis-minged"
	configurations { "Debug", "Release" }

	language "C++"
	kind "ConsoleApp"

	targetdir "build" 
	libdirs { "build" }
	
-- lua interpreter to build tools
-- this should be built first
-- ideally this should be a bootstrap
-- version of Maratis...
	project "MTool"
		kind "ConsoleApp"
		language "C++"
			
		files { "src/tool/**.cpp",
		        "src/tool/**.h",
		        "src/engine/Includes/MPlugin.h",
		        "src/engine/Sources/MPlugin.cpp" }
		includedirs { "src/tool",
			      "src/common/",
			      "src/engine/Includes/",
			      "3rdparty/lua",
			      "3rdparty/npk/include" }

		defines { "MPLUGIN_DYNAMIC" }

		if os.is("linux") then
		   defines { "__LINUX__" }
		   links { "dl" }
		   linkoptions { "-Wl,-rpath=$(MSDKDIR)/Bin" }
		elseif os.is("windows") then
		   defines { "WIN32", "_CRT_SECURE_NO_DEPRECATE" }
		end
		
		links { "lua", "npk", "zlib" }
	
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

-- plugins
dofile("plugins.lua")

	-- forked version of Maratis player
	project "Maratis"
		files { "src/player/src/**.cpp", "src/player/include/**.h" }

		includedirs {   "src/core/Includes/",
				"src/engine/Includes/",
				"src/gui/Includes/",
				"src/common/",
				"3rdparty/bullet/",
				"3rdparty/lua/",
				"3rdparty/minidom/",
				"3rdparty/c_tokenizer/",
				"src/player/include" }
						
		if os.is("windows") then
		   defines { "WIN32" }
			includedirs { "3rdparty/openal/include/" }
			links { "OpenAL32", "Opengl32", "libsndfile-1", "Winmm" }
			libdirs { "3rdparty/openal/win32/" }
		elseif os.is("linux") then
			links { "GL", "openal", "dl", "X11", "Xxf86vm", "sndfile"}
			linkoptions { "-Wl,-rpath=." }
		end
		links { "MCore", 
			"MEngine", 
			"MGui",
			"Common", 
			"glee", 
			"tinyxml", 
			"npk", 
			"lua", 
			"bullet",  
			"png", 
			"zlib", 
			"jpeg", 
			"il", 
			"freetype",
		        "minidom",
			"c_tokenizer"}

		defines { "MPLUGIN_DYNAMIC" }

		prebuildcommands("build/MTool scripts/Mnpk.lua player/maratis.npk player/data")
		prebuildcommands("build/MTool scripts/MEmbedder.lua player/maratis.npk player/include/maratis_npk.h maratis_npk")
			
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

	-- load up Maratis into the build
	dofile "maratis.lua"

	project "minged"
		kind "SharedLib"
		language "C++"

		files { "src/minged/**.cpp", "src/minged/**.h" }
		includedirs {   "src/core/Includes/",
				"src/engine/Includes/",
				"src/gui/Includes/",
				"src/minged/include" }
		targetprefix ""
		defines { "MPLUGIN_DYNAMIC" }

		prebuildcommands("build/MTool scripts/Mnpk.lua src/minged/minged.npk src/minged/data")
		prebuildcommands("build/MTool scripts/MEmbedder.lua src/minged/minged.npk src/minged/include/minged_npk.h minged_npk")

		postbuildcommands("build/MTool scripts/MPackager.lua")
		
		if os.is("linux") then
		   defines { "__LINUX__" }
		elseif os.is("windows") then
		   defines { "WIN32" }
		   links { "OpenGL32" }
		end

		links { "MCore", "MEngine" }
	
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
			
-- example plugin extension, built in the Maratis/MIngEd source tree
	project "Example"
		kind "SharedLib"
		language "C++"
			
		files { "examples/plugin/*" }
		includedirs { "examples/plugin",
			      "src/core/Includes",
			      "src/engine/Includes",
			      "src/minged/include/" }
		targetprefix "" -- Maratis plugins don't have lib*.so		

		prebuildcommands("Mnpk examples/plugin/Example.npk examples/plugin/data")
		prebuildcommands("MEmbedder examples/plugin/Example.npk examples/plugin/Example_npk.h Example_npk")


		if os.is("linux") then
		   defines { "__LINUX__" }
		elseif os.is("windows") then
		   defines { "WIN32" }
		end
		defines { "MPLUGIN_DYNAMIC" }

		links { "MCore", "MEngine" }
	
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

