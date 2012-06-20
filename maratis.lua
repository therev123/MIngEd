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

function ApplyPremake()
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
ApplyPremake()

include "maratis-read-only/trunk/dev"