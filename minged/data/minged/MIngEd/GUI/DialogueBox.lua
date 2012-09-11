class("GUI.DialogueBox")
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
	      end,

   setQuad = function(self, quad)
		self.quad = quad
	     end,

   render = function(self)
	       -- for now, just use quad
	       local quad = self.quad
	       local scale = self.scale -- can apply global scale too?

	       -- draw top left
	       local w, h = self.tl:getSize()
	       w = w * scale
	       h = h * scale
	       local tlq = MIngEd.Quad(quad.left, 
				       quad.top,
				       quad.left + w,
				       quad.top + h)
	       self.tl:render(tlq)
	       
	       -- draw top right
	       w, h = self.tr:getSize()
	       w = w * scale
	       h = h * scale
	       local trq = MIngEd.Quad(quad.right - w,
				       quad.top,
				       quad.right,
				       quad.top + h)
	       self.tr:render(trq)
	       
	       -- draw top middle
	       w, h = self.tm:getSize()
	       w = w * scale
	       h = h * scale
	       local tmq = MIngEd.Quad(tlq.right,
				       quad.top,
				       trq.left,
				       quad.top + h)
	       self.tm:render(tmq)

	       -- draw bottom left
	       w, h = self.bl:getSize()
	       w = w * scale
	       h = h * scale
	       local blq = MIngEd.Quad(quad.left,
				       quad.bottom - h,
				       quad.left + w,
				       quad.bottom)
	       self.bl:render(blq)

	       -- draw bottom right
	       w, h = self.br:getSize()
	       w = w * scale
	       h = h * scale
	       local brq = MIngEd.Quad(quad.right - w,
				       quad.bottom - h,
				       quad.right,
				       quad.bottom)
	       self.br:render(brq)

	       -- draw bottom middle
	       w, h = self.bm:getSize()
	       w = w * scale
	       h = h * scale
	       local bmq = MIngEd.Quad(blq.right,
				       quad.bottom - h,
				       brq.left,
				       quad.bottom)
	       self.bm:render(bmq)

	       -- draw center left
	       w, h = self.cl:getSize()
	       w = w * scale
	       h = h * scale
	       local clq = MIngEd.Quad(quad.left,
				       tlq.bottom,
				       quad.left + w,
				       blq.top)
	       self.cl:render(clq)

	       -- draw center right
	       w, h = self.cr:getSize()
	       w = w * scale
	       h = h * scale
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
	    end,
}