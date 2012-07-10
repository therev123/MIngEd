-- Maratis.lua

-- should checkout Maratis if we don't already have it
-- then paste the premake build system on top of it
-- and apply any patches if necessary

function UpdateMaratis()
   if os.isdir("maratis-read-only") then
      --os.execute("svn update maratis-read-only")
   else
      os.execute("svn checkout http://maratis.googlecode.com/svn/ maratis-read-only")
   end
end

function ApplyPatches()
   os.chdir("maratis-read-only")
   os.execute("patch -p0 -i ../maratis-minged.patch")
   os.chdir("..")
end

UpdateMaratis()
ApplyPatches()

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