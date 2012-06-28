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

-- BEGINNING OF FREETYPE BUILD
if os.is("windows") then
	os.copyfile("maratis-read-only/3rdparty/freetype/win32/freetype.lib", "build/freetype.lib")
elseif os.is("linux") then
else
	project "freetype"

		kind "StaticLib"
		language "C++"

		includedirs { "maratis-read-only/3rdparty/freetype/include", 
			"maratis-read-only/3rdparty/freetype/depend/", 
			"maratis-read-only/3rdparty/freetype/" }
		files { "maratis-read-only/3rdparty/freetype/**.h", 
			"maratis-read-only/3rdparty/freetype/*.c", 
			"maratis-read-only/3rdparty/freetype/depend/ftobjs.c" }

		defines { "FT2_BUILD_LIBRARY" }


		if os.is("windows") then
		   defines { "WIN32" }
		end

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
end
-- END OF FREETYPE BUILD

-- BEGINNING OF LIBSNDFILE BUILD
	if os.is("linux") then
	   local libfiles = { "libsndfile.a", "libsndfile.la", "libsndfile.so" }
	   for k,file in pairs(libfiles) do
	      os.copyfile("maratis-read-only/3rdparty/libsndfile/linux/" .. file, "build/" .. file) 
	   end
	elseif os.is("windows") then
	   local libfiles = { "libsndfile-1.dll", "libsndfile-1.lib" }
	   for k,file in pairs(libfiles) do
	      os.copyfile("maratis-read-only/3rdparty/libsndfile/win32/" .. file, "build/" .. file) 
	   end
	end
-- END OF LIBSNDFILE BUILD

-- BEGINNING OF DEVIL BUILD
	project "il"

		kind "StaticLib"
		language "C++"

		includedirs { "maratis-read-only/3rdparty/devil/src-IL/include",
			      "maratis-read-only/3rdparty/devil/src-ILU/include" ,
			      "maratis-read-only/3rdparty/devil/" }
		files { "maratis-read-only/3rdparty/devil/**.h", 
				"maratis-read-only/3rdparty/devil/**.c", 
				"maratis-read-only/3rdparty/devil/**.cpp" }
		if os.is("windows") then
			defines { "WIN32", "_WIN32" }
			-- these should probably be searched for anyway
			includedirs { "maratis-read-only/3rdparty/libpng/",
						  "maratis-read-only/3rdparty/libjpeg/" ,
						  "maratis-read-only/3rdparty/zlib/" }
		end
		
		defines { "IL_STATIC_LIB" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF DEVIL BUILD

-- BEGINNING OF PNG BUILD
if os.is("windows") then
	project "png"

		kind "StaticLib"
		language "C++"

		includedirs { "maratis-read-only/3rdparty/zlib" }
		files { "maratis-read-only/3rdparty/libpng/**.h", 
				"maratis-read-only/3rdparty/libpng/**.c" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
end
-- END OF PNG BUILD

-- BEGINNING OF PNG BUILD
if os.is("windows") then
	project "jpeg"

		kind "StaticLib"
		language "C++"

		files { "maratis-read-only/3rdparty/libjpeg/**.h", 
				"maratis-read-only/3rdparty/libjpeg/**.c" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
end
-- END OF PNG BUILD

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