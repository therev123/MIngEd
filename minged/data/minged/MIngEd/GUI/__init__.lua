dofile("__resources.lua")

MIngEd.GUI = {}

document[[module for handling editor GUI]](MIngEd.GUI)

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
	     self.font = GUI.Font()
	     if self.atlas then self.atlas:destroy() end
	     self.atlas = MIngEd.Atlas()
	     self.images = {}
	     for i,v in ipairs(resources.images) do
		self.images[v] = GUI.Image(v, self.atlas)
	     end
	     self.atlas:generate()

	     getMMouse():addObserver(self)
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

   onMouseButton = function(self, button, state)
		      for i=#self.elements, 1, -1 do
			 if self.elements.mouseEvent then
			    self.elements:mouseEvent(state, 
						     getMMouse():getPos(),
						     button)
			 end
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
dofile("DialogueBox.lua")
dofile("Font.lua")