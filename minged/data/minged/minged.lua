require("MUtil")
require("MIngEd")

function mingedInit()
   atlas = MIngEd.Atlas()
   img = atlas:add_image("minged/test.png")
   atlas:generate()
   minged_image_destroy(img)
   atlas:test()

   help(MIngEd.GUI.Manager)
end

function mingedUpdate()
end

function mingedRender()
end

function mingedCleanup()
   atlas:destroy()
end