-- Maratis.lua

if os.is("windows") then
	defines { "_WIN32" }
	defines { "WIN32" }
elseif os.is("macosx") then
	defines { "__APPLE__" }
elseif os.is("linux") then
	defines { "LINUX" }
end

dofile "msdk.lua"
dofile "3rdparty.lua"