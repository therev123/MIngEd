-- Maratis.lua

-- should checkout Maratis if we don't already have it
-- then paste the premake build system on top of it
-- and apply any patches if necessary

function UpdateMaratis()
   if os.isdir("maratis") then
      os.execute("svn update maratis")
   else
      os.execute("svn checkout http://maratis.googlecode.com/svn/ maratis")
   end
end

function ApplyPatches()
   os.chdir("maratis")
   os.execute("patch -p0 -N -s -i ../maratis-patch/maratis-minged.patch")
   os.copyfile("../maratis-patch/MProfiler.h", "trunk/dev/MSDK/MEngine/Includes")
   os.copyfile("../maratis-patch/MProfileScope.cpp", "trunk/dev/MSDK/MEngine/Sources")
   os.chdir("..")
end

--UpdateMaratis()
--ApplyPatches()

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