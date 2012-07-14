solution "maratis-minged"

-- BEGINNING OF MCORE BUILD
	project "MCore"

		kind "SharedLib"
		language "C++"

		files { "maratis-read-only/trunk/dev/MSDK/MCore/Includes/**.h",
				"maratis-read-only/trunk/dev/MSDK/MCore/Sources/**.cpp" }

		if os.is("macosx") then
			links { "CoreServices.framework" }
		elseif os.is("windows") then
		   defines { "WIN32" }
		   includedirs { "maratis-read-only/3rdparty/dirent/include/" }
		end
		defines { "MCORE_DLL" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF MCORE BUILD

-- BEGINNING OF MENGINE BUILD
	project "MEngine"

		kind "SharedLib"
		language "C++"

		files { "maratis-read-only/trunk/dev/MSDK/MEngine/Includes/**.h",
				"maratis-read-only/trunk/dev/MSDK/MEngine/Sources/**.cpp" }
				
		includedirs { "maratis-read-only/trunk/dev/MSDK/MCore/Includes" }

		-- move MPlugin into MEngine
		files { "maratis-read-only/trunk/dev/Maratis/Common/MPlugin/*" }
		includedirs { "maratis-read-only/trunk/dev/Maratis/Common/MPlugin",
			      "maratis-read-only/trunk/dev/MSDK/MEngine/Includes",
			      "maratis-read-only/trunk/dev/MSDK/MGui/Includes" }
		defines { "MPLUGIN_DYNAMIC" }

		if os.is("windows") then
		   defines { "WIN32" }
		end
		links { "MCore", "MGui" }
		defines { "MENGINE_DLL" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF MENGINE BUILD

-- BEGINNING OF MGUI BUILD
	project "MGui"

		--kind "SharedLib"
		kind "SharedLib"
		language "C++"

		files { "maratis-read-only/trunk/dev/MSDK/MGui/Includes/*.h",
				"maratis-read-only/trunk/dev/MSDK/MGui/Sources/*.cpp" }
				
		includedirs { "maratis-read-only/trunk/dev/MSDK/MCore/Includes" }

		links { "MCore" }
		defines { "MGUI_DLL" }
		
		if os.is("windows") then
			-- don't like this, but meh
			includedirs { "maratis-read-only/trunk/dev/MSDK/MGui/Includes" }
			files { "maratis-read-only/trunk/dev/MSDK/MGui/Includes/WIN32/*.h",
					"maratis-read-only/trunk/dev/MSDK/MGui/Sources/WIN32/*.cpp" }
			links { "opengl32", "winmm" }
			defines { "WIN32" }
		elseif os.is("macosx") then
			files { "maratis-read-only/trunk/dev/MSDK/MGui/Includes/COCOA/*.h",
					"maratis-read-only/trunk/dev/MSDK/MGui/Sources/COCOA/*.mm" }
		   links { "Cocoa.framework", "Foundation.framework" }
		elseif os.is("linux") then
			files { "maratis-read-only/trunk/dev/MSDK/MGui/Includes/X11/*.h",
					"maratis-read-only/trunk/dev/MSDK/MGui/Sources/X11/*.cpp" }
		end

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF MGUI BUILD

-- BEGINNING OF COMMON BUILD
	project "Common"

		kind "StaticLib"
		language "C++"

		files { "maratis-read-only/trunk/dev/Maratis/Common/**.h", 
				"maratis-read-only/trunk/dev/Maratis/Common/**.cpp" }

		includedirs {   "maratis-read-only/trunk/dev/MSDK/MCore/Includes/",
						"maratis-read-only/trunk/dev/MSDK/MEngine/Includes/",
						"maratis-read-only/trunk/dev/MSDK/MGui/Includes/",
						"maratis-read-only/3rdparty/tinyxml/",
						"maratis-read-only/3rdparty/bullet/",
						"maratis-read-only/3rdparty/glee/",
						"maratis-read-only/3rdparty/devil/",
						"maratis-read-only/3rdparty/lua/",
						"maratis-read-only/3rdparty/libsndfile/include/",
				"maratis-read-only/3rdparty/npk/include/"}

		-- move MPlugin to MEngine
		excludes { "maratis-read-only/trunk/dev/Maratis/Common/MPlugin/*" }

		if os.is("linux") then
		   defines { "linux" }
		   includedirs { "/usr/include/freetype2" } 
		else
		   includedirs { "maratis-read-only/3rdparty/freetype/include/" } 
		end

		links { "MCore", "MEngine", "MGui", "glee", "tinyxml", "npk", "OpenAL32" }
		defines { "M_PACKAGE_WRITABLE", "MPLUGIN_DYNAMIC" }
		
		if os.is("windows") then
		   defines { "WIN32", "IL_STATIC_LIB" }
			includedirs { "maratis-read-only/3rdparty/openal/include/" }
			libdirs { "maratis-read-only/3rdparty/openal/win32/" }
		end

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF COMMON BUILD
