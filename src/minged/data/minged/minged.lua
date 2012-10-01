require("MUtil")
require("MIngEd")

-- I guess I don't need these any more?
function mingedInit()
   MIngEd.init()
   MIngEd.Modes.set("Main")
   MIngEd.Modes.setShortcut("Main", "F2")
   MIngEd.Modes.setShortcut("Atlas", "F3")
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