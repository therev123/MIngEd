dofile("__resources.lua")

MIngEd.GUI = {}

document[[module for handling editor GUI]](MIngEd.GUI)

-- 
GUI = MIngEd.GUI

class("GUI.Manager")
{
   atlas;
   images = {};
   __init__ = function(self)
		 -- register
		 MIngEd.getSystem():manage(self)
	      end,

   init = function(self)
	     if self.atlas then self.atlas:destroy() end
	     self.atlas = MIngEd.Atlas()
	     self.images = {}
	     for i,v in ipairs(resources.images) do
		self.images[v] = GUI.Image(v, self.atlas)
	     end
	     self.atlas:generate()
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
	       self.images["minged/test.png"]:render(
		  MIngEd.Quad(0, 0, 250, 250)
	       )
	    end,
}

document[[Manager for keeping GUI updated and rendering]](GUI.Manager)

local GUIManager = MIngEd.GUI.Manager()
MIngEd.GUI.getManager = document[[Returns the MIngEd GUI Manager singleton]](
   function()
      return GUIManager 
   end
)


class("GUI.Image")
{
   name;
   id;
   atlas;
   __init__ = function(self, name, atlas)
		 self.name = name
		 self.id = atlas:addImage(name)
		 self.atlas = atlas
	      end,

   destroy = function(self)
		mingedImageDestroy(self.id)
	     end,
   
   render = function(self, quad)	       
	       self.atlas:select()
	       local uv1, uv2 = self.atlas:getUVs(self.name)
	       
	       RendererAddQuad( 
		  { quad.left,     quad.top},
		  {     uv1.x,        uv1.y},

		  { quad.right,    quad.top},
		  {      uv2.x,       uv1.y},
		  
		  { quad.right, quad.bottom},
		  {      uv2.x,       uv2.y},

		  { quad.left,  quad.bottom},
		  {     uv1.x,        uv2.y}
	       )
	    end,
}