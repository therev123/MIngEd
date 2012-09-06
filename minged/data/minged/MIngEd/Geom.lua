class ("MIngEd.Quad")
{
   top;
   bottom;
   left;
   right;
   __init__ = function(self, ...)
		 args = {...}
		 if #args == 2 and classmatches(MVec2, args[1]) and classmatches(MVec2, args[2]) then
		    self.left = args[1].x
		    self.top = args[1].y
		    self.right = args[2].x
		    self.bottom = args[2].y
		 elseif #args == 4 then
		    self.left = args[1]
		    self.top = args[2]
		    self.right = args[3]
		    self.bottom = args[4]
		 end
	      end,
}