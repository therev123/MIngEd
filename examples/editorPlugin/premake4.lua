-- Maratis Game plugin tutorial
solution "ExampleEditor"
	configurations { "Debug", "Release" }
	language "C++"

	-- OS defines
	if os.is("windows") then
	   defines { "WIN32" }
	elseif os.is("linux") then
	   defines { "linux" }
	elseif os.is("macosx") then
	   defines { "__APPLE__" }
	end
 
	-- Build the example plugin
	project "ExampleEditor"
		kind "SharedLib"
		language "C++"
		
		-- make sure we can search and link properly
		libdirs { os.getenv("MSDKDIR") .. "SDK/MCore/Libs",
			  os.getenv("MSDKDIR") .. "SDK/MEngine/Libs",
			  os.getenv("MSDKDIR") .. "SDK/MGui/Libs"}
		includedirs { os.getenv("MSDKDIR") .. "SDK/MCore/Includes",
			      os.getenv("MSDKDIR") .. "SDK/MEngine/Includes",
			      os.getenv("MSDKDIR") .. "SDK/MGui/Includes",
			      os.getenv("MSDKDIR") .. "SDK/MIngEd/Includes",
					"."}
		
		-- include all the files, including Maratis SDK ones
		files {
			"**.cpp",
			"**.h",
			os.getenv("MSDKDIR") .. "SDK/**.h"
		}

		-- we're using dynamic plugins for now
		defines { "MPLUGIN_DYNAMIC" }
		
		-- split the files up a bit nicer inside Visual Studio
		vpaths { 
			["MCore/*"] = os.getenv("MSDKDIR") .. "SDK/MCore/Includes/**.h",
			["MEngine/*"] = os.getenv("MSDKDIR") .. "SDK/MEngine/Includes/**.h",
			["MIngEd/*"] = os.getenv("MSDKDIR") .. "SDK/MIngEd/Includes/**.h",
			["Source/*"] = { "**.h", "**.cpp" },
			["Editor/*"] = { "data/editor/**.lua"},
			["Scripts/*"] = { "data/**.lua" }
		}		
		
		prebuildcommands("Mnpk ExampleEditor.npk data")
		prebuildcommands("MEmbedder ExampleEditor.npk ExampleEditor_npk.h ExampleEditor_npk")

		
		-- link to Maratis
		links { "MCore", "MEngine", "MGui" }
		
		-- put the library back into the main project directory

		targetprefix ""
		targetdir "../"
		
		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }
	 
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }  

