--[[
Editor modes
--]]

MIngEd.Modes = {}
MIngEd.Modes.Keys = {}


local currMode
MIngEd.Modes.set = function(mode)
		      local next = MIngEd.Modes[mode]
		      if currMode == next then return else
			 if currMode and currMode.onStop then currMode:onStop() end
			 currMode = next
			 if currMode and currMode.onStart then currMode:onStart() end
		      end
		   end

MIngEd.Modes.setShortcut = function(mode, key)
			      MIngEd.Modes.Keys[key] = mode
			   end,

class ("ModeKeyWatcher")
{
   __init__ = function(self)
		 getMKeyboard():addObserver(self)
	      end,

   onKeyboard = function(self, key, state)
		   if state == "pressed" and
		      MIngEd.Modes.Keys[key] then
		      MIngEd.Modes.set(MIngEd.Modes.Keys[key])
		   end
		end,
}
local KeyWatcher = ModeKeyWatcher()

dofile("ModeMain.lua")
dofile("ModeAtlas.lua")