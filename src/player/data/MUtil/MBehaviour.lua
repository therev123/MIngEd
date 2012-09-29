
class "MBehavior"
{
   id = 0;
   parent = 0;
   variables = {};
   
   __init__ = function(self, id)
		 self.id = id
		 print(id)
		 for i=1, getVariablesNumber(id) do
		    local name = getVariableName(id, i)
		    self.variables[name] = i
		 end
	      end,
   
   setParentObject = function(self,parent)
			self.parent = parent
		     end,
   
   getParentObject = function(self)
			return self.parent
		     end,
   
   getVariable = function(self, name)
		    print("id: " .. self.id)
		    print("name: " .. name)
		    print("bh: " .. self.variables[name])
		    return getVariable(self.id, self.variables[name])
		 end,
   
   setVariable = function(self, name, val)
		    if type(val) == "boolean" then
		       if val == true then val = 1 else val = 0 end
		    end
		    setVariable(self.id, self.variables[name], val)
		 end,
}