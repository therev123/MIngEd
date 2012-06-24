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
	language "C"
	
	files { "nui3/deps/libcss/include/**.h", 
		"nui3/deps/libcss/src/**.c" }
	
	includedirs {"nui3/deps/libcss/include",
		     "nui3/deps/libcss/src",
		     "nui3/deps/libcss/src/libparserutils"}

	configuration "Release"
	    defines { "NDEBUG" }
	    flags { "Optimize" }

	configuration "Debug"
	    defines { "DEBUG" }
	    flags { "Symbols" }
-- END OF LIBCSS BUILD

-- BEGINNING OF EXPAT BUILD
if os.is("windows") then
    project "expat"
        kind "StaticLib"
	language "C"
	
	files { "nui3/deps/expat/lib/**.h", 
		"nui3/deps/expat/lib/**.c" }
	
	includedirs {"nui3/deps/expat/lib"}

	configuration "Release"
	    defines { "NDEBUG" }
	    flags { "Optimize" }

	configuration "Debug"
	    defines { "DEBUG" }
	    flags { "Symbols" }
end
-- END OF EXPAT BUILD
