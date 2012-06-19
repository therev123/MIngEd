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
      dircmd = "dir /b/s maratis"
   end
   os.execute(dircmd .. " > .maratis_tmpfile")
   for f in io.lines(".maratis_tmpfile") do
      newf, r = string.gsub(f, "maratis-premake", "maratis-read-only", 1)
      os.copyfile(f, newf)
   end
end

UpdateMaratis()
ApplyPremake()

include "maratis-read-only/trunk/dev"