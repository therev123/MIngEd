class("GUI.Image")
{
   name;
   id;
   atlas;
   __init__ = function(self, name, atlas)
		 self.name = name
		 self.id = atlas:addImage(name)
		 self.atlas = atlas
		 if not self.id then print("Unable to add " .. name) end
	      end,

   destroy = function(self)
		mingedImageDestroy(self.id)
	     end,
   
   render = function(self, quad, _uv1, _uv2)
	       self.atlas:select()
	       local uv1, uv2 = self.atlas:getUVs(self.name)
	       local xrange = uv2.x - uv1.x
	       local yrange = uv2.y - uv1.y
	       if _uv1 and _uv2 then
		  uv1.x = uv1.x + _uv1.x * xrange
		  uv1.y = uv1.y + _uv1.y * yrange
		  uv2.x = uv1.x + _uv2.x * xrange
		  uv2.y = uv1.y + _uv2.y * yrange
	       end

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
		return MVec2(mingedImageGetSize(self.id))
	     end,
}