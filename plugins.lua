-- example plugin extension, built in the Maratis/MIngEd source tree
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

