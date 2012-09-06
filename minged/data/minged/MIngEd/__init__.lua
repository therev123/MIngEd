-- MIngEd/__init__.lua
MIngEd = {}
document[[Maratis Ingame Editor module]](MIngEd)

dofile("Geom.lua")
dofile("Atlas.lua")
dofile("System.lua")
dofile("GUI.lua")

MIngEd.init = 
   document[[Initialise the MIngEd system]](
   function()
      MIngEd.getSystem():init()
   end
)

MIngEd.update = 
   document[[Update the MIngEd system]](
   function()
      MIngEd.getSystem():update()
   end
)

MIngEd.render = 
   document[[Render the MIngEd system]](
   function()
      MIngEd.getSystem():render()
   end
)

MIngEd.cleanup = 
   document[[Clean up the MIngEd system]](
   function()
      MIngEd.getSystem():cleanup()
   end
)