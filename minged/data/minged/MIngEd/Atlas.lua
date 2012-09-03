
class("MIngEd.Atlas")
{
   id;
   __init__ = function(self, size)
		 size = size or 1024
		 id = minged_atlas_create()
	      end,

   destroy = function(self)
		minged_atlas_destroy(id)
	     end,

   generate = 
      document[[Generate the atlas]](
      function(self, clear)
	 clear = clear or false
	 minged_atlas_generate(id)
      end
   ),

   add_image = 
      document[[Add an image to the atlas]](
      function(self, name)
	 return minged_atlas_add(id, name)
      end
   ),

   test = function(self)
	     print(id)
	  end,
}

document[[Low level automatic atlas handling]](MIngEd.Atlas)