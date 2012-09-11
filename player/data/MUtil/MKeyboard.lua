class("MKeyboard")
{
}

local keyboard = MKeyboard()
getMKeyboard = document[[Returns a singleton to the keyboard manager]] (
   function() return keyboard end
)