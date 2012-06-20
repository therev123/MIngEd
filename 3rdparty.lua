solution "maratis-minged"

-- BEGINNING OF BULLET BUILD
	project "bullet"
		kind "StaticLib"
		language "C++"

		files { "maratis-read-only/3rdparty/bullet/**.h", 
				"maratis-read-only/3rdparty/bullet/**.cpp" }

		includedirs {"maratis-read-only/3rdparty/bullet"}

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF BULLET BUILD

-- BEGINNING OF TINYXML BUILD
	project "tinyxml"

		kind "StaticLib"
		language "C++"

		files { "maratis-read-only/3rdparty/tinyxml/**.h", 
				"maratis-read-only/3rdparty/tinyxml/**.cpp" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF TINYXML BUILD

-- BEGINNING OF GLEE BUILD
	project "glee"

		kind "StaticLib"
		language "C++"

		files { "maratis-read-only/3rdparty/glee/**.h", 
				"maratis-read-only/3rdparty/glee/**.c" }

		if os.is("macosx") then
			links { "OpenGL.framework", "CoreServices.framework", "Foundation.framework" }
		elseif os.is("windows") then
		   defines { "WIN32" }
		end

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF GLEE BUILD

-- BEGINNING OF ZLIB BUILD
	project "zlib"

		kind "StaticLib"
		language "C++"

		files { "maratis-read-only/3rdparty/zlib/**.h", 
				"maratis-read-only/3rdparty/zlib/**.c" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF ZLIB BUILD

-- BEGINNING OF LUA BUILD
	project "lua"
		kind "StaticLib"
		language "C++"

		files { "maratis-read-only/3rdparty/lua/**.h", 
				"maratis-read-only/3rdparty/lua/**.c" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF LUA BUILD

-- BEGINNING OF NPK BUILD
	project "npk"

		kind "StaticLib"
		language "C++"

		files { "maratis-read-only/3rdparty/npk/**.h", 
				"maratis-read-only/3rdparty/npk/**.c" }

		includedirs { "maratis-read-only/3rdparty/npk/include",
						"maratis-read-only/3rdparty/zlib" }

		defines { "M_PACKAGE_WRITABLE", "NPK_DEV"  }

		links { "zlib" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF NPK BUILD
	
	if os.is("windows") then
		libdirs { "maratis-read-only/3rdparty/openal/win32" }
		includedirs { "maratis-read-only/3rdparty/openal/include" }
	end