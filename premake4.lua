solution "maratis-minged"
	configurations { "Debug", "Release" }

	language "C++"
	kind "ConsoleApp"

	targetdir "build" 
	libdirs { "build" }


	-- forked version of Maratis player
	project "Maratis"
		files { "player/src/**.cpp", "player/include/**.h" }

		includedirs {   "maratis-read-only/trunk/dev/MSDK/MCore/Includes/",
				"maratis-read-only/trunk/dev/MSDK/MEngine/Includes/",
				"maratis-read-only/trunk/dev/MSDK/MGui/Includes/",
				"maratis-read-only/trunk/dev/Maratis/Common/",
				"maratis-read-only/3rdparty/bullet/",
				"maratis-read-only/3rdparty/lua/",
				"player/include" }
						
		if os.is("windows") then
		   defines { "WIN32" }
			includedirs { "maratis-read-only/3rdparty/openal/include/" }
			links { "OpenAL32", "Opengl32", "libsndfile-1", "Winmm" }
			libdirs { "maratis-read-only/3rdparty/openal/win32/" }
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
			"freetype" }

		defines { "MPLUGIN_DYNAMIC" }
			
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

	-- load up Maratis into the build
	dofile "maratis.lua"

-- lua interpreter to build tools
	project "MTool"
		kind "ConsoleApp"
		language "C++"
			
		files { "MTool/**.cpp",
		        "MTool/**.h",
		        "maratis-read-only/trunk/dev/Maratis/Common/MPlugin/*" }
		includedirs { "MTool",
			      "maratis-read-only/trunk/dev/Maratis/Common/",
			      "maratis-read-only/3rdparty/lua",
			      "maratis-read-only/3rdparty/npk/include" }

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

	project "minged"
		kind "SharedLib"
		language "C++"

		files { "minged/**.cpp", "minged/**.h" }
		includedirs {   "maratis-read-only/trunk/dev/MSDK/MCore/Includes/",
				"maratis-read-only/trunk/dev/MSDK/MEngine/Includes/",
				"maratis-read-only/trunk/dev/MSDK/MGui/Includes/",
				"maratis-read-only/trunk/dev/Maratis/Common/MPlugin",
				"minged/include" }
		targetprefix ""
		defines { "MPLUGIN_DYNAMIC" }

		prebuildcommands("build/MTool scripts/Mnpk.lua minged/minged.npk minged/data")
		prebuildcommands("build/MTool scripts/MEmbedder.lua minged/minged.npk minged/include/minged_npk.h minged_npk")
		
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
			      "maratis-read-only/trunk/dev/MSDK/MCore/Includes",
			      "maratis-read-only/trunk/dev/MSDK/MEngine/Includes",
			      "maratis-read-only/trunk/dev/Maratis/Common/MPlugin" } -- this is an unfortunate line
		targetprefix "" -- Maratis plugins don't have lib*.so

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

