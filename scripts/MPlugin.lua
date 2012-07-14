function PrintHelp()
   print("usage: action file")
   print("")
   print("action:")
   print("  install - install the plugin to the user directory")
   print("  check   - check the plugin is compatible with installed version of Maratis")
   print("")
   print("file: the plugin file")
end

function GetPluginDir()
   return os.getenv("HOME") .. "/.Maratis/plugins/"
end

function SplitPath(path)
   return string.match(path, "(.-)([^/]-([^%.]+))$")
end

function CheckPlugin(plugin)
   local path, file, ext = SplitPath(plugin)
   if not ext == "so" then
      print("Plugin " .. plugin .. " has incorrect extension " .. ext)
      return false
   end

   local p = string.gsub(plugin, "." .. ext, "")
   local success, error = load_plugin(p)
   if not success then
      print("Plugin " .. plugin .. " is incompatible with current version of Maratis")
      print("  " .. error)
      print("banana")
      return false
   end

   print("Plugin " .. plugin .. " verified")
   return true
end

function InstallPlugin(plugin)
   local dir = GetPluginDir()
   local path, file, ext = SplitPath(plugin)

   print("Installing " .. file .. " to " .. dir)

   local f = io.open(plugin, "r")
   local t = f:read("*all")
   f:close()
   local p = io.open(dir .. file, "w")
   p:write(t)
   p:close()
end

function main(Args)
   if #Args < 2 then
      PrintHelp()
   elseif Args[1] == "install" then
      if CheckPlugin(Args[2]) then
	 InstallPlugin(Args[2])
      end
   elseif Args[1] == "check" then
      CheckPlugin(Args[2])
   end
end