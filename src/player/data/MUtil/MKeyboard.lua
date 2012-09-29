dofile("keys.lua")

class("MKeyboard")
{
   keys = {};
   observers = {};

   __init__ = function(self)
		 for i=65,90 do
		    self.keys[string.char(i)] = "up"
		 end
		 
		 for i=1,#keys do
		    self.keys[keys[i]] = "up"
		 end
	      end,

   update = function(self)
	       for k,v in pairs(self.keys) do 
		  if isKeyPressed(k) == true then
		     if v == "up" or v == "released" then
			self.keys[k] = "pressed"
			self:sendKeyMessage(k, "pressed")
		     elseif v == "pressed" then
			self.keys[k] = "down"
		     end
		  else
		     if v == "down" or v == "pressed" then
			self.keys[k] = "released"
			self:sendKeyMessage(k, "released")
		     elseif v == "released" then
			self.keys[k] = "up"
		     end
		  end
	       end
	    end,

   getKey = function(self, key)
		  key = key or "SPACE" -- logical default?
		  return self.keys[key]
	       end,

   isKeyPressed = function(self, key)
			local state = self:getKey(key)
			if state == "down" or state == "pressed" then
			   return true else return false end
		     end,

   addObserver = function(self, observer)
		    table.insert(self.observers, observer)
		 end,

   sendKeyMessage = function(self, key, state)
			  for k,v in pairs(self.observers) do
			     if v.onKeyboard then 
				v:onKeyboard(key, state) 
			     end
			  end
		    end,
}

local keyboard = MKeyboard()
getMKeyboard = document[[Returns a singleton to the keyboard manager]] (
   function() return keyboard end
)