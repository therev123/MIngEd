-- rocket.lua

-- checks out, updates and builds everything needed for libRocket

function UpdateRocket()
   if os.isdir("libRocket") then
      -- update libRocket
   else
      os.execute("git clone https://github.com/lloydw/libRocket.git")
      -- clone libRocket
   end
end


UpdateRocket()

solution "maratis-minged"

    project "rocket"
    language "C++"
    kind "StaticLib"

    includedirs { "libRocket/Include",
	          "maratis-read-only/3rdparty/freetype/include" }

    files { "libRocket/Source/Core/**.cpp",
	    "libRocket/Source/Core/**.h" }

    -- I realise that I should have just done *.cpp and *.h above
    -- I've done it this way in case libRocket adds new interesting
    -- features
    excludes { "libRocket/Source/Core/Python/**.cpp",
	       "libRocket/Source/Core/Python/**.h" }
		   
    defines { "STATIC_LIB" }

    links { "freetype", "zlib" }

    configuration "Release"
        defines { "NDEBUG" }
	flags { "Optimize" }
	    
    configuration "Debug"
	defines { "DEBUG" }
	flags { "Symbols" }