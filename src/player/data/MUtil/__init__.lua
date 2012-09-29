-- load all of MUtil

-- Bartbes' class implementation
dofile("slither.lua")

-- Documentation
dofile("MDoc.lua")

-- Utility functions
dofile("MVec2.lua")
dofile("MVec3.lua")

dofile("MCol.lua")

-- C++/lua interface wrappers
dofile("MBehaviour.lua")
dofile("MEntity.lua")

-- inherited entity types
dofile("MCamera.lua")
dofile("MPhysics.lua")

-- Input
dofile("MMouse.lua")
dofile("MKeyboard.lua")

function min(x,y)
   if x < y then return x else return y end
end

function max(x,y)
   if x > y then return x else return y end
end

function clamp(val, _min, _max)
   return min(max(val, _min), _max)
end