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
       os.cp("maratis-read-only/trunk/dev/MSDK/" .. m .. "/Includes", pkg["sdkdir"] .. m .. "/Includes")
    end

    os.cp("maratis-read-only/trunk/dev/Maratis/Common/MPlugin/MPlugin.h", pkg["sdkdir"] .. "MEngine/Includes")
 end

function package_libs(pkg)
   print("package_libs")
   os.cp("build/Maratis" .. executable_ext(), pkg["bindir"])
end

function package_tools(pkg)
   print("package_tools")
   os.cp("build/MTool" .. executable_ext(), pkg["bindir"])
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