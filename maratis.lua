-- Maratis.lua

-- should checkout Maratis if we don't already have it
-- then paste the premake build system on top of it
-- and apply any patches if necessary

function UpdateMaratis()
   if os.isdir("maratis-read-only") then
      os.execute("svn update maratis-read-only")
   else
      os.execute("svn checkout http://maratis.googlecode.com/svn/ maratis-read-only")
   end
end

function ApplyPatches()
   local dircmd = "find maratis-premake -type f -print"
   if os.is("windows") then
      dircmd = "dir /b/s maratis-premake"
   end
   os.execute(dircmd .. " > .maratis_tmpfile")
   for f in io.lines(".maratis_tmpfile") do
      newf, r = string.gsub(f, "-premake", "-read-only", 1)
      if string.find(f, ".patch") then
	 -- apply patch
	 newf, r = string.gsub(newf, ".patch", "")
	 os.execute("patch -N " .. newf .. " " .. f)
      else
	 os.copyfile(f, newf)
      end
   end
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