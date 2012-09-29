local docstrings = setmetatable({}, { __mode = "kv" })

function document(str)
   return function(obj) docstrings[obj] = str; return obj end
end

function help(obj)
   print(docstrings[obj])
end