class ("MIngEd.Quad")
{
   top;
   bottom;
   left;
   right;
   __init__ = function(self, ...)
		 local args = {...}
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

   __add__ = function(self, dir)
		return MInged.Quad(self.left  + dir.x,
				   self.top   + dir.y,
				   self.right + dir.x,
				   self.bottom+ dir.y)
	     end,

   translate = function(self, ...)
		  local dir  = MVec2(...)
		  self.left  = self.left + dir.x
		  self.right = self.right+ dir.x
		  self.top   = self.top  + dir.y
		  self.bottom=self.bottom+ dir.y
	       end,

   intersects = function(self, ...)
		   local args = {...}
		   if #args == 2 and type(args[1]) == "number" and type(args[2]) == "number" then
		      return args[1] > self.left and
			 args[1] < self.right and
			 args[2] > self.top and
			 args[2] < self.bottom
		   else
		      return false
		   end
		end,
}