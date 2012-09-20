class("MIngEd.Modes.Atlas")
{
   window;
   onStart = function(self)
		self.window = MIngEd.Windows.WindowAtlas(MIngEd.Quad(0, 0, 300, 400))
		self.window:setAtlas(MIngEd.GUI.getManager().atlas)
	     end,

   onStop = function(self)
	       self.window:destroy()
	       self.window = nil
	    end,
}