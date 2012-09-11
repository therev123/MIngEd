local defCharmap = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789"

local keywords =
{
"base",
"charmap",
"size",
}

class("GUI.Font")
{
   atlas;
   def;
   chars = {};

   __init__ = function(self, def)
		 self.atlas = MIngEd.Atlas()
		 self.def = def or resources.fonts.system
		 
		 local images = {}
		 local base = self.def.base

		 local charmap = self.def.charmap or defCharmap
		 for i=1, charmap:len() do
		    local c = string.char(charmap:byte(i))
		    img = self:addChar(c, base .. c .. ".png")
		    if img then table.insert(images, img) end
		 end

		 for i,v in pairs(self.def) do
		    local keyword = false
		    for j,k in pairs(keywords) do
		       if k == i then keyword = true end
		    end

		    if keyword == false then
		       img = self:addChar(i, base .. v)
		       if img then table.insert(images, img) end
		    end
		 end
		 self.atlas:generate()

		 for i,v in pairs(images) do
		    mingedImageDestroy(v)
		 end
	      end,

   addChar = document[[Add a character to the font]] (
      function(self, ch, file)
	 img = self.atlas:addImage(file)
	 if img then
	    self.chars[ch] = 
	    {
	       size = MVec2(mingedImageGetSize(img)), 
	       filename = file
	    }
	 end
	 return img
      end
   ),

   print = function(self, text, posx, posy, ptSize, quad)
	      local pos = MVec2(posx, posy)
	      ptSize = ptSize or self.def.size
	      local scale = ptSize / self.def.size
	      for i=1, text:len() do
		 local c = string.char(text:byte(i))
		 local char = self.chars[c]
		 
		 if char then
		    local file = char.filename
		    local size = char.size * scale
		    
		    local uv1, uv2 = self.atlas:getUVs(file)		 
		    
		    self.atlas:select()
		    RendererAddQuad(
		       { pos.x, pos.y },
		       { uv1.x, uv1.y },
		       
		       { pos.x + size.x, pos.y },
		       { uv2.x, uv1.y },
		       
		       { pos.x + size.x, pos.y + size.y },
		       { uv2.x, uv2.y },
		       
		       { pos.x, pos.y + size.y},
		       { uv1.x, uv2.y }
		    )
		    pos.x = pos.x + size.x
		 end
	      end
	   end,
}