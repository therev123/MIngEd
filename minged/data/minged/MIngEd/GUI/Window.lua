class("GUI.Window")
{
   tl;
   tm;
   tr;
   cl;
   cm;
   cr;
   bl;
   bm;
   br;

   quad;
   scale;
   __init__ = function(self, quad, scale)
		 local gui = MIngEd.GUI.getManager()
		 self.scale = scale or 1.0
		 -- top
		 self.tl = gui:getImage("tl.png")
		 self.tm = gui:getImage("tm.png")
		 self.tr = gui:getImage("tr.png")
		 -- center
		 self.cl = gui:getImage("cl.png")
		 self.cm = gui:getImage("cm.png")
		 self.cr = gui:getImage("cr.png")
		 -- bottom
		 self.bl = gui:getImage("bl.png")
		 self.bm = gui:getImage("bm.png")
		 self.br = gui:getImage("br.png")

		 self.quad = quad
		 
		 MIngEd.GUI.getManager():addElement(self)
		 
		 if self.init then self:init() end
	      end,

   destroy = function(self)
		MIngEd.GUI.getManager():removeElement(self)
	     end,

   setQuad = function(self, quad)
		self.quad = quad
	     end,

   render = function(self)
	       -- for now, just use quad
	       local quad = self.quad
	       local scale = self.scale -- can apply global scale too?

	       -- draw top left
	       local size = self.tl:getSize()
	       local w = size.x * scale
	       local h = size.y * scale
	       local tlq = MIngEd.Quad(quad.left, 
				       quad.top,
				       quad.left + w,
				       quad.top + h)
	       self.tl:render(tlq)
	       
	       -- draw top right
	       size = self.tr:getSize()
	       w = size.x * scale
	       h = size.y * scale
	       local trq = MIngEd.Quad(quad.right - w,
				       quad.top,
				       quad.right,
				       quad.top + h)
	       self.tr:render(trq)
	       
	       -- draw top middle
	       size = self.tm:getSize()
	       w = size.x * scale
	       h = size.y * scale
	       local tmq = MIngEd.Quad(tlq.right,
				       quad.top,
				       trq.left,
				       quad.top + h)
	       self.tm:render(tmq)

	       -- draw bottom left
	       size = self.bl:getSize()
	       w = size.x * scale
	       h = size.y * scale
	       local blq = MIngEd.Quad(quad.left,
				       quad.bottom - h,
				       quad.left + w,
				       quad.bottom)
	       self.bl:render(blq)

	       -- draw bottom right
	       size = self.br:getSize()
	       w = size.x * scale
	       h = size.y * scale
	       local brq = MIngEd.Quad(quad.right - w,
				       quad.bottom - h,
				       quad.right,
				       quad.bottom)
	       self.br:render(brq)

	       -- draw bottom middle
	       size = self.bm:getSize()
	       w = size.x * scale
	       h = size.y * scale
	       local bmq = MIngEd.Quad(blq.right,
				       quad.bottom - h,
				       brq.left,
				       quad.bottom)
	       self.bm:render(bmq)

	       -- draw center left
	       size = self.cl:getSize()
	       w = size.x * scale
	       h = size.y * scale
	       local clq = MIngEd.Quad(quad.left,
				       tlq.bottom,
				       quad.left + w,
				       blq.top)
	       self.cl:render(clq)

	       -- draw center right
	       size = self.cr:getSize()
	       w = size.x * scale
	       h = size.y * scale
	       local crq = MIngEd.Quad(quad.right - w,
				       trq.bottom,
				       quad.right,
				       brq.top)
	       self.cr:render(crq)

	       -- draw center middle
	       local cmq = MIngEd.Quad(tlq.right,
				       tlq.bottom,
				       brq.left,
				       brq.top)
	       self.cm:render(cmq)
	       
	       if self.onWindowRender then self:onWindowRender() end
	    end,
}