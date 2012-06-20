solution "maratis-minged"
	if os.is("windows") then
	   defines { "_WIN32" }
	   defines { "WIN32" }
	elseif os.is("macosx") then
	   defines { "__APPLE__" }
	elseif os.is("linux") then
	   defines { "LINUX" }
	end
	
    include "../../3rdparty"
    include "MSDK"
    include "Maratis"
