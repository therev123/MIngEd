#!/usr/bin/python

import os, errno, shutil, os.path, glob
from distutils import dir_util

def mkdir(path):
    try:
        os.makedirs(path)
    except OSError as exc:
        if exc.errno == errno.EEXIST:
            pass
        else: raise

def make_package_dir():
    print("make_package_dir")
    pkg = {}
    pkg["dir"]        = os.path.join("pkg", "Maratis")
    pkg["sdkdir"]     = os.path.join(pkg["dir"], "SDK")
    pkg["scriptdir"]  = os.path.join(pkg["dir"], "Scripts")
    pkg["toolsdir"]     = os.path.join(pkg["dir"], "Tools")
    pkg["bindir"]     = os.path.join(pkg["dir"], "Bin")
    pkg["sdkmodules"] = ["MCore", "MEngine", "MGui"]
    pkg["sdkdirs"]    = ["Includes", "Libs", "Doc"]

    mkdir(pkg["bindir"])
    mkdir(pkg["scriptdir"])
    mkdir(pkg["toolsdir"])
    for m in pkg["sdkmodules"]:
        for d in pkg["sdkdirs"]:
            mkdir(os.path.join(pkg["sdkdir"], m, d))
    return pkg    

def package_headers(pkg):
    print("package_headers")
    for m in pkg["sdkmodules"]:
        dir_util.copy_tree(os.path.join("maratis-read-only","trunk","dev","MSDK", m, "Includes"), 
                           os.path.join(pkg["sdkdir"], m, "Includes"))
    shutil.copy(os.path.join("maratis-read-only","trunk","dev","Maratis","Common","MPlugin","MPlugin.h"), 
                os.path.join(pkg["sdkdir"], "MEngine", "Includes"))

def package_libs(pkg):
    print("package_libs")
    for filename in glob.glob(os.path.join("build", "*.so")):
        shutil.copy(filename, pkg["bindir"])
    for m in pkg["sdkmodules"]:
        shutil.copy(os.path.join("build","lib" + m + ".so"), os.path.join(pkg["sdkdir"], m, "Libs"))
    shutil.copy(os.path.join("build", "Maratis"), pkg["bindir"])
    shutil.copy(os.path.join("build", "MTool"), pkg["bindir"])

def package_tools(pkg):
    print("package_tools")
    shutil.copy(os.path.join("build", "Maratis"), os.path.join("pkg", "Maratis", "Bin"))
    for loader in glob.glob(os.path.join("scripts", "loaders", "*")):
        shutil.copy(loader, pkg["toolsdir"])
    for script in glob.glob(os.path.join("scripts", "*.lua")):
        shutil.copy(script, pkg["scriptdir"])

    # Just some wrapper scripts
    shutil.copy("MaratisPlayer", os.path.join("pkg", "Maratis", "Bin"))
    shutil.copy("MaratisEditor", os.path.join("pkg", "Maratis", "Bin"))

def archive_package(pkg):
    print("archive_package")

if __name__ == "__main__":
    pkg = make_package_dir()
    package_headers(pkg)
    package_libs(pkg)
    package_tools(pkg)
    archive_package(pkg)
