function PrintHelp()
   print("usage: pkg dir")
   print("")
   print("pkg: the package to create")
   print("")
   print("dir: the directory to add to the package")
end

function main(args)
   if #args < 2 then
      PrintHelp()
   else
      local pkg = npk.open(args[1])
      npk.add_dir(pkg, args[2])
      npk.close(pkg)
   end
end