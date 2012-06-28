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
				"libRocket/Include",
				"minged/include" }
		targetprefix ""

		if os.is("linux") then
		   defines { "__LINUX__" }
		end
		elseif os.is("windows") then
		   defines { "WIN32" }
		end

		links {  -- first the Maratis dependencies
		   "MCore", "MEngine", "rocket", "freetype" } -- might need MGui evetually
	
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

	-- load up Maratis into the build
	dofile "maratis.lua"
	-- load libRocket
	dofile "rocket.lua"