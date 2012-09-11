class("MMouse")
{
   pos;
   delta;
   observers = {};
   buttons =
   {
   MOUSE_BUTTON1 = "up",
   MOUSE_BUTTON2 = "up",
   MOUSE_BUTTON3 = "up",
   };


   __init__ = function(self)
		 delta = MVec2(0, 0)
	      end,

   update = function(self)
	       local prev
	       if self.pos then prev = self.pos end
	       self.pos = MVec2(getAxis("MOUSE_X"),
				getAxis("MOUSE_Y"))
	       local screen = MVec2(getScreenSize())
	       self.pos.x = self.pos.x * screen.x
	       self.pos.y = self.pos.y * screen.y

	       if prev then self.delta = self.pos - prev end

	       if not delta:isZero() then
		  for k,v in pairs(self.observers) do
		     if v.onMouseMove then v:onMouseMove(delta) end
		  end
	       end

	       -- handle mouse buttons
	       for k,v in pairs(self.buttons) do
		  local pressed = onKeyDown(k)
		  if pressed == true then
		     if v == "up" or v == "released" then
			self.buttons[k] = "pressed"
			self:sendButtonMessage(k, "pressed")
		     elseif v == "pressed" then
			self.buttons[k] = "down"
		     end
		  else
		     if v == "down" or v == "pressed" then
			self.buttons[k] = "released"
			self:sendButtonMessage(k, "released")
		     elseif v == "released" then
			self.buttons[k] = "up"
		     end
		  end
	       end
	    end,

   getPos = function(self)
	       return self.pos
	    end,
   
   getDelta = function(self)
		 return self.delta
	      end,

   addObserver = function(self, observer)
		    table.insert(self.observers, observer)
		 end,

   sendButtonMessage = function(self, button, state)	  
			  for k,v in pairs(self.observers) do
			     if v.onMouseButton then 
				v:onMouseButton(button, state) 
			     end
			  end
		       end,
}

local mouse = MMouse()
getMMouse = document[[Returns a singleton to the mouse manager]] (
   function() return mouse end
)