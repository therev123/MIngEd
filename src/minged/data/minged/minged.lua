require("MUtil")
require("MIngEd")

-- I guess I don't need these any more?
function mingedInit()
   MIngEd.init()
   MIngEd.Modes.set("Main")
end

function mingedUpdate()
   if getMKeyboard():isKeyPressed("F2")then 
      print("Switching to main mode")
      MIngEd.Modes.set("Main")
   elseif getMKeyboard():isKeyPressed("F3") then
      print("Switching to atlas mode")
      MIngEd.Modes.set("Atlas")
   end
   MIngEd.update()
end

function mingedRender()
   MIngEd.render()
end

function mingedCleanup()
   MIngEd.cleanup()
end