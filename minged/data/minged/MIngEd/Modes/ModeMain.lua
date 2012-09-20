class("MIngEd.Modes.Main")
{
   window;
   onStart = function(self)
		self.window = MIngEd.GUI.Window(MIngEd.Quad(0, 0, 200, 300))
	     end,

   onStop = function(self)
	       self.window:destroy()
	       self.window = nil
	    end,
}