-- nui.lua

-- checks out, updates and builds everything needed for libnui

function UpdateNUI()
   if os.isdir("nui3") then
      --os.execute("svn update maratis-read-only")
   else
      --os.execute("git clone https://github.com/meeloo/nui3.git")
   end
   os.chdir("nui3")
   --os.execute("git checkout nui4")
   os.chdir("..")
end

function PatchNUI()
   local dircmd = "find nui-patch -type f -print"
   if os.is("windows") then
      dircmd = "dir /b/s nui-patch"
   end
   os.execute(dircmd .. " > .nui_tmpfile")
   for f in io.lines(".nui_tmpfile") do
      newf, r = string.gsub(f, "-patch", "3", 1)
      if string.find(f, ".patch") then
	 -- apply patch
	 newf, r = string.gsub(newf, ".patch", "")
	 os.execute("patch -N " .. newf .. " " .. f)
      else
	 os.copyfile(f, newf)
      end
   end
end


UpdateNUI()
PatchNUI()

dofile "nuideps.lua"

solution "maratis-minged"

    if os.is("linux") then
       buildoptions { "`pkg-config --cflags glib-2.0`" }
    end

    project "nui"
        kind "StaticLib"
        language "C++"

        files { "nui3/src/**.cpp",
	        "nui3/include/**.h",
	        "nui3/graphics/**.cpp",
	        "nui3/graphics/**.h",
	        "MNui/src/**.cpp",
	        "MNui/include/**.h" }

	if os.is("linux") then
	   if os.is64bit() then
	      buildoptions { "-fPIC" }
	   end
	end

        includedirs { "nui3/include/", 
		      "nui3",
		      "nui3/src/Text/HTML",
		      "nui3/src/Stream/Zip",
		      "nui3/deps/libcss/include",
		      "nui3/deps/tracemonkey",
		      "nui3/deps/ucdata",
		      "nui3/deps/tidy",
		      "nui3/deps/harfbuzz",
		      "maratis-read-only/3rdparty/freetype/include",
		      "maratis-read-only/3rdparty/libpng",
		      "maratis-read-only/3rdparty/zlib",
		      "maratis-read-only/3rdparty/libjpeg",
		      "MNui/include" }

	excludes { "nui3/src/Time/Android/**.cpp",
		   "nui3/src/Application/Android/**.cpp",
		   "nui3/src/Audio/ASIO/**.cpp",
		   "nui3/src/Audio/Android/**.cpp",
		   "nui3/src/Window/Android/**.cpp",
		   "nui3/src/Window/Unix/nglWindow_Unix.cpp", -- replaced by MNui window
		   "nui3/src/Application/Unix/nglApplication_Unix.cpp", -- replaced by MNui application
		   "nui3/src/Net/Android/**.cpp",
		   "nui3/src/Window/nglOffscreenContext.cpp",
		   "nui3/src/Renderers/nuiGLDrawContext.cpp",
		   "nui3/src/Renderers/nuiMemoryDrawContext.cpp",
		   "nui3/src/Renderers/nuiMetaDrawContext.cpp",
		   "nui3/src/String/nglStringConv_Android.cpp",
		   "nui3/src/Font/nuiFontLayout.cpp" }


	--------------------------------------------------------------------------------
	-- Beginning of OS specific excludes
	--------------------------------------------------------------------------------
	if not os.is("macosx") then
	   excludes { "nui3/src/Image/nglImageCGCodec.cpp",
		      "nui3/src/String/nglStringConv_Carbon.cpp",
		      "nui3/src/String/nglStringConv_CoreFoundation.cpp",
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
		      "nui3/src/Application/Win/**.cpp",
		      "nui3/src/String/nglStringConv_mlang.cpp",
		      "nui3/src/Video/nuiVideoDecoder_WinXX.cpp" }
	end
	
	if not os.is("linux") then
	   excludes { "nui3/src/Window/Unix/**.cpp",
				  "nui3/src/Time/Unix/**.cpp",
				  "nui3/src/Threading/posix/**.cpp"}
	end
	
	--------------------------------------------------------------------------------
	-- End of OS specific excludes
	--------------------------------------------------------------------------------

	if os.is("linux") then
	   defines { "__LINUX__", "HAVE_GLIB" }
	   includedirs { "nui3/src/Application/Unix" }
	   excludes { "nui3/src/String/nglStringConv_none.cpp" }
	end
	
	if os.is("windows") then
		includedirs { "nui3/src/Application/Win" }
		excludes { "nui3/src/String/nglStringConv_iconv.cpp" }
	end

	configuration "Release"
	    defines { "NDEBUG" }
	    flags { "Optimize" }
	    
	configuration "Debug"
	    defines { "DEBUG" }
	    flags { "Symbols" }