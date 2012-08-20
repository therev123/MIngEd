
function make_pkg_file(key)
   print("make_pkg_file")
   print(key or "0 0 0 0")
   pkg = {}
   id = npk.new("published/somefile.npk")
   print(id);
   return pkg;
end

function package_libs(pkg)
   print("package_libs")
end

function package_game(pkg)
   print("package_game")
end

function package_plugins(pkg)
   print("package_plugins")
end

function package_data(pkg)
   print("package_data")
   package_maps(pkg)
   package_meshs(pkg)
   package_sound(pkg)
   package_scripts(pkg)
end

function main(Args)
   pkg = make_pkg_file(Args[1])
   package_libs(pkg)
   package_game(pkg)
   package_plugins(pkg)
   package_data(pkg)
end

--------------------------------------------------------------------------------
-- project packaging
--------------------------------------------------------------------------------
function package_maps(pkg)
   print("  maps")
end

function package_meshs(pkg)
   print("  meshs")
end

function package_sound(pkg)
   print("  sound")
end

function package_scripts(pkg)
   print("  scripts")
end