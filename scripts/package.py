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
    mkdir("pkg")
    mkdir("pkg/Maratis")
    mkdir("pkg/Maratis/SDK")
    for m in ['MCore', 'MEngine', 'MGui']:
        mkdir("pkg/Maratis/SDK/" + m);
        for d in ['Includes', 'Libs']:
            mkdir("pkg/Maratis/SDK/" + m + "/" + d);
    mkdir("pkg/Maratis/Bin")

def package_headers(pkg):
    print("package_headers")
    for m in ['MCore', 'MEngine', 'MGui']:
        dir_util.copy_tree("maratis-read-only/trunk/dev/MSDK/" + m + "/Includes", 
                           "pkg/Maratis/SDK/" + m + "/Includes")

def package_libs(pkg):
    print("package_libs")
    for filename in glob.glob(os.path.join("build", "*.so")):
        shutil.copy(filename, "pkg/Maratis/Bin")
    shutil.copy(os.path.join("build", "Maratis"), "pkg/Maratis/Bin")

def archive_package(pkg):
    print("archive_package")

if __name__ == "__main__":
    pkg = make_package_dir()
    package_headers(pkg)
    package_libs(pkg)
    archive_package(pkg)
