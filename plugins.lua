-- Movie studio plugin
	project "Movie"
		kind "SharedLib"
		language "C++"
			
		files { "src/movie/*" }
		includedirs { "src/movie",
			      "src/core/Includes",
			      "src/engine/Includes",
			      "src/minged/include/" }
		targetprefix "" -- Maratis plugins don't have lib*.so		

		prebuildcommands("Mnpk src/movie/Movie.npk src/movie/data")
		prebuildcommands("MEmbedder src/movie/Movie.npk src/movie/Movie_npk.h Movie_npk")

		targetdir "build/plugins"

		defines { "MPLUGIN_DYNAMIC" }

		links { "MCore", "MEngine" }
	
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }


			
-- example plugin extension, built in the Maratis/MIngEd source tree
	project "Example"
		kind "SharedLib"
		language "C++"
			
		files { "examples/plugin/*" }
		includedirs { "examples/plugin",
			      "src/core/Includes",
			      "src/engine/Includes",
			      "src/minged/include/" }

		targetprefix "" -- Maratis plugins don't have lib*.so		

		if os.is("linux") then
		   defines { "__LINUX__" }
		elseif os.is("windows") then
		   defines { "WIN32" }
		end
		defines { "MPLUGIN_DYNAMIC" }

		links { "MCore", "MEngine" }
	
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

			
-- example plugin extension, built in the Maratis/MIngEd source tree
	project "ExampleEditor"
		kind "SharedLib"
		language "C++"
			
		files { "examples/editorPlugin/*" }
		includedirs { "examples/editorPlugin",
			      "src/core/Includes",
			      "src/engine/Includes",
			      "src/minged/include/" }
		targetprefix "" -- Maratis plugins don't have lib*.so		

		prebuildcommands("Mnpk examples/editorPlugin/ExampleEditor.npk examples/editorPlugin/data")
		prebuildcommands("MEmbedder examples/editorPlugin/ExampleEditor.npk examples/editorPlugin/ExampleEditor_npk.h ExampleEditor_npk")


		if os.is("linux") then
		   defines { "__LINUX__" }
		elseif os.is("windows") then
		   defines { "WIN32" }
		end
		defines { "MPLUGIN_DYNAMIC" }

		links { "MCore", "MEngine" }
	
		configuration "Release"
			defines { "NDEBUG" }
			flags { "Optimize" }

		configuration "Debug"
			defines { "DEBUG" }
			flags { "Symbols" }

