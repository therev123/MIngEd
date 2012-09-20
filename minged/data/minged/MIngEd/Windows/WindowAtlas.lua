class("MIngEd.Windows.WindowAtlas") (MIngEd.GUI.Window)
{
   atlas;
   view;
   tile;
   bg;

   saveBtn;
   
   init = function(self)
	     self.view = MIngEd.Quad(self.quad.left + 25,
				     self.quad.top + 50,
				     self.quad.right - 20,
				     self.quad.bottom - 20)

	     self.tile = MIngEd.GUI.getManager():getImage("tile.png")
	     self.bg = MIngEd.GUI.getManager():getImage("view.png")

	     self.saveBtn = MIngEd.GUI.Button(MIngEd.Quad(self.view.right - 100,
							  self.view.bottom - 50,
							  self.view.right,
							  self.view.bottom))
						     
	  end,
   
   setAtlas = function(self, atlas)
		 self.atlas = atlas
	      end,

   onWindowRender = function(self)       
		       self.atlas:select()
		       local uv1, uv2= MVec2(0, 0), MVec2(1, 1)
		       local available = MVec2(self.view.right - self.view.left,
					       self.view.bottom - self.view.top)
		       local size = self.atlas:getSize()
		       local scalexy = MVec2(available.x / size.x,
					     available.y / size.y)
		       local mid = MVec2(self.view.left + (available.x / 2),
					 self.view.top  + (available.y / 2))
		       local scale = min(scalexy.x, scalexy.y)
		       size.x = size.x * scale
		       size.y = size.y * scale
		       local quad = MIngEd.Quad(mid.x - (size.x / 2),
						mid.y - (size.y / 2),
						mid.x + (size.x / 2),
						mid.y + (size.y / 2))

		       self.bg:render(self.view)

		       local tileSize = self.tile:getSize()
		       local pos = MVec2(quad.left, quad.top)
		       while pos.y < quad.bottom do
			  while pos.x < quad.right do
			     local uv1 = MVec2(0,0)
			     local uv2 = MVec2(1,1)
			     local ts = MVec2(tileSize)
			     ts.x = min(ts.x, quad.right - pos.x)
			     ts.y = min(ts.y, quad.bottom - pos.y)
			     uv2.x = ts.x / tileSize.x
			     uv2.y = ts.y / tileSize.y
			     self. tile:render(MIngEd.Quad(pos.x, 
							   pos.y, 
							   pos.x + ts.x,
							   pos.y + ts.y),
					       uv1, uv2)
			     pos.x = pos.x + ts.x
			  end
			  pos.x = quad.left
			  pos.y = pos.y + tileSize.y
		       end
		       
		       RendererAddQuad( 
			  { quad.left+1,     quad.top+1},
			  {     uv1.x,        uv1.y},
			  
			  { quad.right-1,    quad.top+1},
			  {      uv2.x,       uv1.y},
			  
			  { quad.right-1, quad.bottom-1},
			  {      uv2.x,       uv2.y},
			  
			  { quad.left+1,  quad.bottom+1},
			  {     uv1.x,        uv2.y}
		       )
		       
		       local mouse = getMMouse():getPos()
		       if self.view:intersects(mouse.x, mouse.y) then
			  self.saveBtn:render()
		       end
		    end,

   mouseEvent = function(self, state, pos, button)
		   if button == "MOUSE_BUTTON1" and state == "pressed" then
		      if self.saveBtn.quad:intersects(pos.x, pos.y) then
			 self.atlas:write()
		      end
		   end
		end,
}