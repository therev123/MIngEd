
class("MIngEd.Atlas")
{
   id;
   name;
   __init__ = function(self, size, name)
		 size = size or 1024
		 self.name = name
		 self.id = mingedAtlasCreate(size, name)
	      end,

   destroy = function(self)
		mingedAtlasDestroy(self.id)
	     end,

   generate = 
      document[[Generate the atlas]](
      function(self, clear)
	 clear = clear or false
	 mingedAtlasGenerate(self.id)
      end
   ),

   addImage = 
      document[[Add an image to the atlas]](
      function(self, name)
	 name = name or ""
	 return mingedAtlasAdd(self.id, name)
      end
   ),

   select =
      document[[Use the atlas to draw]](
      function(self)
	 mingedAtlasSelect(self.id)
      end
   ),

   getUVs =
      document[[Find the UVs for an image in the atlas]](
      function(self, name)
	 name = name or ""
	 u1,v1,u2,v2 = mingedAtlasGetUVs(self.id, name)
	 return MVec2(u1,v1), MVec2(u2,v2)
      end
   ),

   write = 
      document[[Write the atlas to file]](
      function(self)
	 mingedAtlasWrite(self.id, "atlases/" .. self.name .. ".png")
      end
   ),

   getSize =
      document[[Returns the size of the atlas image]](
      function(self)
	 return MVec2(mingedAtlasGetSize(self.id))
      end
   ),
}

document[[Low level automatic atlas handling]](MIngEd.Atlas)