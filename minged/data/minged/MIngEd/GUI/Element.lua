class("GUI.Element")
{
   quad = MIngEd.Quad(0, 0, 0, 0);
   
   setPosition = function(self, ...)
		    local pos = MVec2(...)
		    local size = MVec2(self.quad.right - self.quad.left,
				       self.quad.bottom - self.quad.top)
		    self.quad = MIngEd.Quad(pos.x,
					    pos.y,
					    pos.x + size.x,
					    pos.y + size.y)		    
		 end,

   setSize = function(self, ...)
		local size = MVec2(...)
		self.quad.right = self.quad.left + size.x
		self.quad.bottom = self.quad.top + size.y
	     end,

   mouseEvent = function(self, event, pos, button)
		   button = button or "none"
		   if self.quad:intersects(pos) then
		      if self.onMouseEvent then self:onMouseEvent(event, pos, button) end
		      return true else return false
		   end
		end,
}

document[[Base class for all GUI elements, drawable or otherwise]](MIngEd.GUI.Element)