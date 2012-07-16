function ext()
   return ".so"
end
		
function static_ext()
   return ".so"
end
		
function executable_ext()
   return ""
end
		
function loader_ext()
   return ""
end
		
function prefix()
   return "lib"
end


function make_package_dir()
    print("make_package_dir")
    pkg = {}
    
    os.mkdir("pkg")
    pkg["dir"]        = "pkg/Maratis/"
    pkg["sdkdir"]     = pkg["dir"] .. "SDK/"
    pkg["scriptdir"]  = pkg["dir"] .. "Scripts/"
    pkg["toolsdir"]   = pkg["dir"] .. "Tools/"
    pkg["bindir"]     = pkg["dir"] .. "Bin/"
    pkg["plugindir"]     = pkg["dir"] .. "Plugins/"
    pkg["sdkmodules"] = {"MCore", "MEngine", "MGui"}
    pkg["sdkdirs"]    = {"Includes", "Libs", "Doc"}

    os.mkdir(pkg["dir"])
    os.mkdir(pkg["bindir"])
    os.mkdir(pkg["scriptdir"])
    os.mkdir(pkg["toolsdir"])
    os.mkdir(pkg["sdkdir"])
    for i,m in pairs(pkg["sdkmodules"]) do
       os.mkdir(pkg["sdkdir"] ..  m )
       for j,d in pairs(pkg["sdkdirs"]) do
	  os.mkdir(pkg["sdkdir"] ..  m .. "/" .. d)
       end
    end

    return pkg
end

function package_headers(pkg)
    print("package_headers")
    for i,m in pairs(pkg["sdkmodules"]) do
       os.cp_of_type("maratis-read-only/trunk/dev/MSDK/" .. m .. "/Includes", pkg["sdkdir"] .. m .. "/Includes", ".h")
    end

    os.cp("maratis-read-only/trunk/dev/Maratis/Common/MPlugin/MPlugin.h", pkg["sdkdir"] .. "MEngine/Includes")
 end

function package_libs(pkg)
   print("package_libs")
   os.cp_of_type("build/", pkg["bindir"], ext());

   for i,m in pairs(pkg["sdkmodules"]) do
      os.cp("build/" .. prefix() .. m .. static_ext(), pkg["sdkdir"] .. m .. "/Libs")
   end
   os.cp("build/Maratis" .. executable_ext(), pkg["bindir"])
end

function package_tools(pkg)
   print("package_tools")
   os.cp("scripts/loaders/", pkg["toolsdir"])
   os.cp("build/MTool" .. executable_ext(), pkg["bindir"])

   os.cp_of_type("scripts/", pkg["scriptdir"], ".lua")

   os.cp("MaratisPlayer" .. loader_ext(), pkg["bindir"])
   os.cp("MaratisEditor" .. loader_ext(), pkg["bindir"])
end

function package_plugins(pkg)
   print("package_plugins")
   os.cp("build/minged" .. ext(), pkg["plugindir"])
   os.cp("build/Example" .. ext(), pkg["plugindir"])
end

function archive_package(pkg)
   print("archive_package")
end

function main(Args)
    pkg = make_package_dir()
    package_headers(pkg)
    package_libs(pkg)
    package_tools(pkg)
    archive_package(pkg)
end