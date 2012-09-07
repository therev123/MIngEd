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

   getSize = function(self)
		return mingedImageGetSize(self.id)
	     end,
}