function PrintHelp()
   print("usage: file [output] name")
   print("")
   print("file: the file to embed")
   print("")
   print("output: the name of the output file (usually a .h)")
   print("")
   print("name: the name of the object from within C/C++")
end

function main(args)
   if #args < 2 then
      PrintHelp()
   else
      local out = args[1]
      local output = "embedded.h"
      local name = "embedded_data"
      if #args == 2 then
	 name = args[2]
	 output = name .. ".h"
      elseif #args == 3 then
	 output = args[2]
	 name = args[3]
      end
	 
      file.embed( out, output, name )
   end
end