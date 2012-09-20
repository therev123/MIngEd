--[[
Editor modes
--]]

MIngEd.Modes = {}


local currMode
MIngEd.Modes.set = function(mode)
		      local next = MIngEd.Modes[mode]
		      if currMode == next then return else
			 if currMode and currMode.onStop then currMode:onStop() end
			 currMode = next
			 if currMode and currMode.onStart then currMode:onStart() end
		      end
		   end

dofile("ModeMain.lua")
dofile("ModeAtlas.lua")