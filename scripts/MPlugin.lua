function PrintHelp()
   print("usage: action file")
   print("")
   print("action:")
   print("  install - install the plugin to the user directory")
   print("  check   - check the plugin is compatible with installed version of Maratis")
   print("")
   print("file: the plugin file")
end

function SplitPath(path)
   return string.match(path, "(.-)([^/]-([^%.]+))$")
end

function CheckPlugin(plugin)
   local path, file, ext = SplitPath(plugin)
   if not ext == tool.plugin_ext() then
      print("Plugin " .. plugin .. " has incorrect extension " .. ext)
      return false
   end

   local p = string.gsub(plugin, "." .. ext, "")
   local success, error = tool.load_plugin(p)
   if not success then
      print("Plugin " .. plugin .. " is incompatible with current version of Maratis")
      print("  " .. error)
      return false
   end

   print("Plugin " .. plugin .. " verified")
   return true
end

function InstallPlugin(plugin)
   local dir = tool.plugin_dir()
   local path, file, ext = SplitPath(plugin)

   print("Installing " .. file .. " to " .. dir)

   local f = io.open(plugin, "r")
   local t = f:read("*all")
   f:close()
   local p = io.open(dir .. "/" .. file, "w")
   p:write(t)
   p:close()
end

function main(args)
   if #args < 2 then
      PrintHelp()
   elseif args[1] == "install" then
      if CheckPlugin(args[2]) then
	 InstallPlugin(args[2])
      end
   elseif args[1] == "check" then
      CheckPlugin(args[2])
   end
end