solution "maratis-minged"

-- BEGINNING OF BULLET BUILD
	project "bullet"
		kind "StaticLib"
		language "C++"

		files { "3rdparty/bullet/**.h", 
				"3rdparty/bullet/**.cpp" }

		includedirs {"3rdparty/bullet"}

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

		files { "3rdparty/tinyxml/**.h", 
				"3rdparty/tinyxml/**.cpp" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF TINYXML BUILD

-- BEGINNING OF TLMM BUILD
	project "tlmm"

		kind "StaticLib"
		language "C++"

		files { "3rdparty/tlmm/include/tlmm.h", 
			"3rdparty/tlmm/src/tlmm.cpp" }
		includedirs { "3rdparty/tlmm/include/" }

		defines{ "TLMM_HAS_IO", "TLMM_COMPILE" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "OptimizeSpeed",
				"EnableSSE", 
				"EnableSSE2",
				"FloatFast",
				"ExtraWarnings",
				"FatalWarnings",
				"NoFramePointer"}

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF TLMM BUILD

-- BEGINNING OF TLDR BUILD
	project "tldr"

		kind "StaticLib"
		language "C++"

		files { "3rdparty/tldr/include/tldr.h", 
			"3rdparty/tldr/src/tldr.cpp" }
		includedirs { "3rdparty/tldr/include/" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "OptimizeSpeed",
				"ExtraWarnings",
				"FatalWarnings",
				"NoFramePointer"}

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF TLDR BUILD

-- BEGINNING OF GLEE BUILD
	project "glee"

		kind "StaticLib"
		language "C++"

		files { "3rdparty/glee/**.h", 
				"3rdparty/glee/**.c" }

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
	os.copyfile("3rdparty/freetype/win32/freetype.lib", "build/freetype.lib")
elseif os.is("linux") then
else
	project "freetype"

		kind "StaticLib"
		language "C++"

		includedirs { "3rdparty/freetype/include", 
			"3rdparty/freetype/depend/", 
			"3rdparty/freetype/" }
		files { "3rdparty/freetype/**.h", 
			"3rdparty/freetype/*.c", 
			"3rdparty/freetype/depend/ftobjs.c" }

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
	      os.copyfile("3rdparty/libsndfile/linux/" .. file, "build/" .. file) 
	   end
	elseif os.is("windows") then
	   local libfiles = { "libsndfile-1.dll", "libsndfile-1.lib" }
	   for k,file in pairs(libfiles) do
	      os.copyfile("3rdparty/libsndfile/win32/" .. file, "build/" .. file) 
	   end
	end
-- END OF LIBSNDFILE BUILD

-- BEGINNING OF DEVIL BUILD
	project "il"

		kind "StaticLib"
		language "C++"

		includedirs { "3rdparty/devil/src-IL/include",
			      "3rdparty/devil/src-ILU/include" ,
			      "3rdparty/devil/" }
		files { "3rdparty/devil/**.h", 
				"3rdparty/devil/**.c", 
				"3rdparty/devil/**.cpp" }
		if os.is("windows") then
			defines { "WIN32", "_WIN32" }
			-- these should probably be searched for anyway
			includedirs { "3rdparty/libpng/",
						  "3rdparty/libjpeg/" ,
						  "3rdparty/zlib/" }
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

		includedirs { "3rdparty/zlib" }
		files { "3rdparty/libpng/**.h", 
				"3rdparty/libpng/**.c" }

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

		files { "3rdparty/libjpeg/**.h", 
				"3rdparty/libjpeg/**.c" }

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

		files { "3rdparty/zlib/**.h", 
			"3rdparty/zlib/**.c" }

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

		files { "3rdparty/lua/**.h", 
			"3rdparty/lua/**.c" }

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

			files { "3rdparty/npk/**.h", 
				"3rdparty/npk/**.c" }
			
			includedirs { "3rdparty/npk/include",
				      "3rdparty/zlib" }

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
		libdirs { "3rdparty/openal/win32" }
		includedirs { "3rdparty/openal/include" }
	     end


-- BEGINNING OF MINIDOM BUILD
	project "minidom"

		kind "StaticLib"
		language "C++"

		files { "3rdparty/minidom/**.h", 
			"3rdparty/minidom/**.cpp" }

		includedirs{ "3rdparty/minidom/" }

		defines { "MINIDOM_SUPPORT_XML",
			  "MINIDOM_SUPPORT_INI",
			  "MINIDOM_SUPPORT_JSON",
			  "MINIDOM_ENABLE_DUMP" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG", "MINIDOM_DEBUG" }
			flags { "Symbols" }
-- END OF MINIDOM BUILD

-- BEGINNING OF C_TOKENIZER BUILD
	project "c_tokenizer"

		kind "StaticLib"
		language "C++"

		files { "3rdparty/c_tokenizer/**.h", 
			"3rdparty/c_tokenizer/**.c" }

		includedirs{ "3rdparty/c_tokenizer/" }

		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
-- END OF C_TOKENIZER BUILD
