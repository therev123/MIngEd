require("MUtil")
require("MIngEd")

-- I guess I don't need these any more?
function mingedInit()
   MIngEd.init()
   MIngEd.GUI.DialogueBox(MIngEd.Quad(0, 0, 200, 300), 0.5)
end

function mingedUpdate()
   MIngEd.update()
end

function mingedRender()
   MIngEd.render()
end

function mingedCleanup()
   MIngEd.cleanup()
end