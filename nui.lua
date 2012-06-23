-- nui.lua

-- checks out, updates and builds everything needed for libnui

function UpdateNUI()
   if os.isdir("nui3") then
      --os.execute("svn update maratis-read-only")
   else
      os.execute("git clone https://github.com/meeloo/nui3.git")
   end
   os.chdir("nui3")
   os.execute("git checkout nui4")
   os.chdir("..")
end

UpdateNUI()

dofile "nuideps.lua"

solution "maratis-minged"

    project "nui"
        type "StaticLib"
        language "C++"

        files { "nui3/src/**.cpp",
	        "nui3/include/**.h" }

        includedirs { "nui3/include/", 
		      "nui3",
		      "nui3/src/Text/HTML",
		      "nui3/src/Stream/Zip",
		      "nui3/deps/libcss/include",
		      "nui3/deps/tracemonkey",
		      "nui3/deps/ucdata",
		      "maratis-read-only/3rdparty/libpng"}

	excludes { "nui3/src/Time/Android/**.cpp",
		   "nui3/src/Application/Android/**.cpp",
		   "nui3/src/Audio/ASIO/**.cpp",
		   "nui3/src/Audio/Android/**.cpp",
		   "nui3/src/Window/Android/**.cpp",
		   "nui3/src/Window/nglOffscreenContext.cpp",
		   "nui3/src/Renderers/nuiGLDrawContext.cpp",
		   "nui3/src/Renderers/nuiMemoryDrawContext.cpp",
		   "nui3/src/Renderers/nuiMetaDrawContext.cpp" }


	if not os.is("macosx") then
	   excludes { "nui3/src/Image/nglImageCGCodec.cpp",
		      "nui3/src/File/Cocoa/**.cpp",
		      "nui3/src/File/Carbon/**.cpp",
		      "nui3/src/Net/CoreFoundation/**.cpp",
		      "nui3/src/Net/UIKit/**.cpp",
		      "nui3/src/Time/CoreFoundation/**.cpp",
		      "nui3/src/Time/Carbon/**.cpp",
		      "nui3/src/Window/Carbon/**.cpp",
		      "nui3/src/Window/Cocoa/**.cpp",
		      "nui3/src/Window/UIKit/**.cpp",
		      "nui3/src/AudioSamples/OSX/**.cpp",
		      "nui3/src/Audio/CoreAudio/**.cpp",
		      "nui3/src/Application/Carbon/**.cpp",
		      "nui3/src/Application/UIKit/**.cpp",
		      "nui3/src/Application/Cocoa/**.cpp" }
	end
	
	if not os.is("windows") then
	   excludes { "nui3/src/File/Win/**.cpp",
		      "nui3/src/Net/Win/**.cpp",
		      "nui3/src/Time/Win/**.cpp",
		      "nui3/src/Window/Win/**.cpp",
		      "nui3/src/AudioSamples/Win/**.cpp",
		      "nui3/src/Audio/DirectSound/**.cpp",
		      "nui3/src/Threading/win32/**.cpp",
		      "nui3/src/Application/Win/**.cpp" }
	end

	if os.is("linux") then
	   defines { "__LINUX__" }
	   includedirs { "nui3/src/Application/Unix" }
	end

	configuration "Release"
	    defines { "NDEBUG" }
	    flags { "Optimize" }
	    
	configuration "Debug"
	    defines { "DEBUG" }
	    flags { "Symbols" }