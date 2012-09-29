
class("MIngEd.System")
{
   objects = {};

   __init__ = function(self)
	      end,

   init =
      document[[Initialise the MIngEd system]](
      function(self)
	 for k,v in ipairs(self.objects) do
	    if v.init then v:init() end
	 end
      end
   ),

   update =
      document[[Update the MIngEd system]](
      function(self)
	 for k,v in ipairs(self.objects) do
	    if v.update then v:update() end
	 end
      end
   ),

   render =
      document[[Render the MIngEd system]](
      function(self)
	 for k,v in ipairs(self.objects) do
	    if v.render then v:render() end
	 end
      end
   ),

   cleanup =
      document[[Clean up the MIngEd system]](
      function(self)
	 for k,v in ipairs(self.objects) do
	    if v.destroy then v:destroy() end
	 end
      end
   ),

   manage =
      document[[Register an object for the MIngEd system to manage]](
      function(self, obj)
	 if obj then
	    table.insert(self.objects, obj)
	 end
      end
   ),
}

local System = MIngEd.System()

MIngEd.getSystem = document[[Returns the MIngEd system singleton]] (
   function() return System end
)

document[[Interface for managing and updating all MIngEd modules]](MIngEd.System)