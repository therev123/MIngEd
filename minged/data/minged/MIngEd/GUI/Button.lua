class("MIngEd.GUI.Button")
{
   inactiveLeft;
   inactiveCenter;
   inactiveRight;
   
   quad;
   scale;
   __init__ = function(self, quad, scale)
		 local gui = MIngEd.GUI.getManager()
		 self.scale = scale or 1.0
		 self.inactiveLeft = gui:getImage("button/inactiveleft.png")
		 self.inactiveCenter = gui:getImage("button/inactivecenter.png")
		 self.inactiveRight = gui:getImage("button/inactiveright.png")

		 self.quad = quad
		 if self.init then self:init() end
	      end,

   destroy = function(self)
	     end,
   
   render = function(self)
	       local quad = self.quad
	       local scale= self.scale

	       local size = self.inactiveLeft:getSize()
	       local w = size.x * scale

	       local lq = MIngEd.Quad(quad.left,
				      quad.top,
				      quad.left + w,
				      quad.bottom)
	       self.inactiveLeft:render(lq)

	       size = self.inactiveRight:getSize()
	       w = size.x * scale
	       local rq = MIngEd.Quad(quad.right - w,
				      quad.top,
				      quad.right,
				      quad.bottom)
	       self.inactiveRight:render(rq)
	       
	       local cq = MIngEd.Quad(lq.right,
				      quad.top,
				      rq.left,
				      quad.bottom)
	       self.inactiveCenter:render(cq)

	       if self.onButtonRender then self:onButtonRender() end
	    end,
}