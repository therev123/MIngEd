dofile("__resources.lua")

MIngEd.GUI = {}

document[[module for handling editor GUI]](MIngEd.GUI)

__DISABLE_FONT = 1

-- 
GUI = MIngEd.GUI

class("GUI.Manager")
{
   atlas;
   images = {};
   elements = {};
   font;
   __init__ = function(self)
		 -- register
		 MIngEd.getSystem():manage(self)
	      end,

   init = function(self)
	     if not __DISABLE_FONT then
		self.font = GUI.Font()
	     end
	     if self.atlas then self.atlas:destroy() end
	     self.atlas = MIngEd.Atlas(512, "GUI")
	     self.images = {}
	     for i,v in ipairs(resources.images) do
		self.images[v] = GUI.Image(v, self.atlas)
	     end
	     self.atlas:generate()

	     getMMouse():addObserver(self)
	     getMKeyboard():addObserver(self)
	  end,

   destroy = function(self)
		self.atlas:destroy()
		for i,v in ipairs(self.images) do
		   v:destroy()
		end
		self.images = {}
	     end,

   update = function(self)
	       --print("Update")
	    end,

   render = function(self)
	       for i,v in ipairs(self.elements) do
		  if v.render then v:render() end
	       end
	    end,

   getImage = function(self, name)
		 return self.images["gui/" .. name]
	      end,

   addElement = function(self, elem)
		   table.insert(self.elements, elem)
		end,

   removeElement = function(self, elem)
		      for i,v in pairs(self.elements) do
			 if v == elem then table.remove(self.elements, i) end
		      end
		   end,

   onMouseButton = function(self, button, state)
		      for i=#self.elements, 1, -1 do
			 if self.elements[i].mouseEvent then
			    self.elements[i]:mouseEvent(state, 
							getMMouse():getPos(),
							button)
			 end
		      end
		   end,

   onKeyboard = function(self, key, state)
		   if key == "SPACE" and state == "pressed" then
		      self.atlas:write("GUI.png")
		   end
		end,
}

document[[Manager for keeping GUI updated and rendering]](GUI.Manager)

local GUIManager = MIngEd.GUI.Manager()
MIngEd.GUI.getManager = document[[Returns the MIngEd GUI Manager singleton]](
   function()
      return GUIManager 
   end
)

dofile("Image.lua")
dofile("Window.lua")
dofile("Font.lua")
dofile("Button.lua")