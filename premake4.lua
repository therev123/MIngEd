solution "maratis-minged"
	configurations { "Debug", "Release" }

	language "C++"
	kind "ConsoleApp"

	targetdir "build" 
	libdirs { "build" }

	project "minged"
		files { "src/**.cpp", "include/**.h" }

		includedirs {   "maratis-read-only/trunk/dev/MSDK/MCore/Includes/",
						"maratis-read-only/trunk/dev/MSDK/MEngine/Includes/",
						"maratis-read-only/trunk/dev/MSDK/MGui/Includes/",
						"maratis-read-only/trunk/dev/Maratis/Common/",
						"maratis-read-only/3rdparty/bullet/",
						"maratis-read-only/3rdparty/lua/",
						"include" }
						
		if os.is("windows") then
		   defines { "WIN32" }
			includedirs { "maratis-read-only/3rdparty/openal/include/" }
			libdirs { "maratis-read-only/3rdparty/openal/win32/" }
		end
		links { "MCore", "MEngine", "MGui", "Common" }
	-- load up Maratis into the build
	dofile "maratis.lua"