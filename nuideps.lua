solution "maratis-minged"

-- BEGINNING OF TRACEMONKEY BUILD
--    project "tracemonkey"
--        kind "StaticLib"
--	language "C++"
	
--	files { "nui3/deps/tracemonkey/**.h", 
--		"nui3/deps/tracemonkey/**.cpp" }
--	
--	includedirs {"nui3/deps/tracemonkey"}
--
--	defines { "XP_UNIX",
--		  "NGL_STATIC_BUILD",
--		  "HAVE_BCOPY",
--		  "FEATURE_NANOJIT",
--		  "JS_TRACER",
--		  "DEBUG" }
--
--	configuration "Release"
--	    defines { "NDEBUG" }
--	    flags { "Optimize" }
--
--	configuration "Debug"
--	    defines { "DEBUG" }
--	    flags { "Symbols" }
-- END OF TRACEMONKEY BUILD

-- BEGINNING OF LIBCSS BUILD
    project "css"
        kind "StaticLib"
	language "C++"
	
	files { "nui3/deps/libcss/include/**.h", 
		"nui3/deps/libcss/src/**.c" }
	
	includedirs {"nui3/deps/libcss/include",
		     "nui3/deps/libcss/src",
		     "nui3/deps/libcss/src/libparserutils"}
			 
	if os.is("windows") then
		defines { "WIN32" }
	end

	configuration "Release"
	    defines { "NDEBUG" }
	    flags { "Optimize" }

	configuration "Debug"
	    defines { "DEBUG" }
	    flags { "Symbols" }
-- END OF LIBCSS BUILD

-- BEGINNING OF LIBTIDY BUILD
    project "tidy"
        kind "StaticLib"
	language "C"
	
	files { "nui3/deps/tidy/**.h", 
		"nui3/deps/tidy/**.c" }
	
	includedirs {"nui3/deps/tidy"}

	configuration "Release"
	    defines { "NDEBUG" }
	    flags { "Optimize" }

	configuration "Debug"
	    defines { "DEBUG" }
	    flags { "Symbols" }
-- END OF LIBTIDY BUILD

-- BEGINNING OF UCDATA BUILD
    project "ucdata"
        kind "StaticLib"
	language "C++"

	files { "nui3/deps/ucdata/**.h", 
		"nui3/deps/ucdata/**.c", 
		"nui3/deps/ucdata/**.cpp" }
	
	includedirs {"nui3/deps/ucdata"}
 
	if os.is("windows") then
		defines { "WIN32", "__STDC__" }
	end
	
	configuration "Release"
	    defines { "NDEBUG" }
	    flags { "Optimize" }

	configuration "Debug"
	    defines { "DEBUG" }
	    flags { "Symbols" }
-- END OF UCDATA BUILD

-- BEGINNING OF UNGIF BUILD
    project "ungif"
        kind "StaticLib"
	language "C"

	files { "nui3/deps/ungif/**.h", 
		"nui3/deps/ungif/**.c" }
	
	includedirs {"nui3/deps/ngif"}

	configuration "Release"
	    defines { "NDEBUG" }
	    flags { "Optimize" }

	configuration "Debug"
	    defines { "DEBUG" }
	    flags { "Symbols" }
-- END OF UNGIF BUILD

-- BEGINNING OF HARFBUZZ BUILD
    project "harfbuzz"
        kind "StaticLib"
	language "C++"
	
	files { "nui3/deps/harfbuzz/**.h", 
		"nui3/deps/harfbuzz/**.c", 
		"nui3/deps/harfbuzz/**.cc" }

	excludes { "nui3/deps/harfbuzz/main.cc",
		   "nui3/deps/harfbuzz/test.cc",
		   "nui3/deps/harfbuzz/test.c",
		   "nui3/deps/harfbuzz/hb-view.c",
		   "nui3/deps/harfbuzz/hb-view.cc",
		   "nui3/deps/harfbuzz/hb-uniscribe-shape.cc",
		   "nui3/deps/harfbuzz/hb-tt-font.cc" }

	if os.is("linux") then
		defines { "HAVE_GLIB" }
	else
		excludes {  "nui3/deps/harfbuzz/hb-gobject**",
					"nui3/deps/harfbuzz/hb-glib.**",
					"nui3/deps/harfbuzz/hb-icu.**" }
	end

	includedirs { "nui3/deps/harfbuzz",
		      "maratis-read-only/3rdparty/freetype/include" }

	configuration "Release"
	    defines { "NDEBUG" }
	    flags { "Optimize" }

	configuration "Debug"
	    defines { "HB_DEBUG" }
	    flags { "Symbols" }
-- END OF HARFBUZZ BUILD

-- BEGINNING OF EXPAT BUILD
    project "expat"
        kind "StaticLib"
	language "C"
	
	files { "nui3/deps/expat/lib/**.h", 
		"nui3/deps/expat/lib/**.c" }
	
	includedirs {"nui3/deps/expat/lib"}

	defines { "HAVE_MEMMOVE" }

	configuration "Release"
	    defines { "NDEBUG" }
	    flags { "Optimize" }

	configuration "Debug"
	    defines { "DEBUG" }
	    flags { "Symbols" }
-- END OF EXPAT BUILD
