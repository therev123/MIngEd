solution "maratis-minged"
	configurations { "Debug", "Release" }

	language "C++"
	kind "ConsoleApp"

	targetdir "build" 
	libdirs { "build" }
	
-- lua interpreter to build tools
-- this should be built first
	project "MTool"
		kind "ConsoleApp"
		language "C++"
			
		files { "MTool/**.cpp",
		        "MTool/**.h",
		        "maratis/trunk/dev/MSDK/MEngine/Includes/MPlugin.h",
		        "maratis/trunk/dev/MSDK/MEngine/Sources/MPlugin.cpp" }
		includedirs { "MTool",
			      "maratis/trunk/dev/Maratis/Common/",
			      "maratis/trunk/dev/MSDK/MEngine/Includes/",
			      "maratis/3rdparty/lua",
			      "maratis/3rdparty/npk/include" }

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

	-- forked version of Maratis player
	project "Maratis"
		files { "player/src/**.cpp", "player/include/**.h" }

		includedirs {   "maratis/trunk/dev/MSDK/MCore/Includes/",
				"maratis/trunk/dev/MSDK/MEngine/Includes/",
				"maratis/trunk/dev/MSDK/MGui/Includes/",
				"maratis/trunk/dev/Maratis/Common/",
				"maratis/3rdparty/bullet/",
				"maratis/3rdparty/lua/",
				"3rdparty/minidom/",
				"3rdparty/c_tokenizer/",
				"player/include" }
						
		if os.is("windows") then
		   defines { "WIN32" }
			includedirs { "maratis/3rdparty/openal/include/" }
			links { "OpenAL32", "Opengl32", "libsndfile-1", "Winmm" }
			libdirs { "maratis/3rdparty/openal/win32/" }
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

		files { "minged/**.cpp", "minged/**.h" }
		includedirs {   "maratis/trunk/dev/MSDK/MCore/Includes/",
				"maratis/trunk/dev/MSDK/MEngine/Includes/",
				"maratis/trunk/dev/MSDK/MGui/Includes/",
				"maratis/trunk/dev/Maratis/Common/MPlugin",
				"minged/include" }
		targetprefix ""
		defines { "MPLUGIN_DYNAMIC" }

		prebuildcommands("build/MTool scripts/Mnpk.lua minged/minged.npk minged/data")
		prebuildcommands("build/MTool scripts/MEmbedder.lua minged/minged.npk minged/include/minged_npk.h minged_npk")

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
			      "maratis/trunk/dev/MSDK/MCore/Includes",
			      "maratis/trunk/dev/MSDK/MEngine/Includes",
			      "minged/include/" }
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

