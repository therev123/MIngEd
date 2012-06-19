solution "maratis-minged"
  configurations { "Debug", "Release" }

  language "C++"
  kind "ConsoleApp"

  targetdir "build" 

  -- load up Maratis into the build
  dofile "maratis.lua"
  
  project "minged"

    files { "src/**.cpp", "include/**.h" }

    includedirs { "include" }