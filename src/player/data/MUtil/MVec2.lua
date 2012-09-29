--module(..., package.seeall)

class "MVec2"
{
   x = 0;
   y = 0;
   z = 0;

   __init__ = function(self, v1, v2)
		 if type(v1) == "number" and type(v2) == "number" then
		    self.x = v1 -- we've been passed 3 numbers, just stick them in
		    self.y = v2
		 elseif type(v1) == "number" and v2 == nil then
		    self.x = v1 -- we've only been given 1 number, use it for all
		    self.y = v1
		 elseif classmatches(self, v1) then
		    self.x = v1.x -- we've been given another MVec2, copy it
		    self.y = v1.y
		 else
		    self.x = v1[1] -- just assume we've been given a table, try and get values from it
		    self.y = v1[2]
		 end
	      end,

   __getattr__ = function(self, key)
		    if key == 1 or key == "x" then return self.x
		    elseif key == 2  or key == "y" then return self.y
		    else return end
		 end,

   __add__ = function(self, other)
		if classmatches(self, other) then
		   return MVec2(self.x + other.x, self.y + other.y)
		end
		if type(other) == "table" then
		   return MVec2(self.x + other[1], self.y + other[2])
		end
	     end,
   
   __sub__ = function(self, other)
		if classmatches(self, other) then
		   return MVec2(self.x - other.x, self.y - other.y)
		end
		if type(other) == "table" then
		   return MVec2(self.x - other[1], self.y - other[2])
		end
	     end,
   
   __mul__ = function(self, other)
		if type(other) == "number" then
		   return MVec2(self.x * other, self.y * other)
		end
	     end,

   -- I tried to make this work with the # operator, but I couldn't
   magnitude = function(self)
		  return math.sqrt(self:magnitudeSquared())
	       end,
   
   magnitudeSquared = function(self)
      return self.x^2 + self.y^2
   end,

   cross = function(self, other)
	   end,

   dot = function(self, ...)
	    other = MVec2(...)
	    return (self.x * other.x) + (self.y + other.y)
	 end,

   normalize = function(self)
		  local mag = self:magnitude()
		  self.x = self.x / mag
		  self.y = self.y / mag
	       end,

   getNormalized = function(self)
		  local mag = self:magnitude()
		  return MVec2( self.x / mag,
				self.y / mag )
	       end,

   isZero = function(self)
	       if self.x == 0 and self.y == 0 then
		  return true
	       else
		  return false
	       end
	    end,
}