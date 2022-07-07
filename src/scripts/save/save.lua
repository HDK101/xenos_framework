Save = { refs = {} }

function Save:new(filename)
  o = {}
  setmetatable(o, self)
  self.__index = self
  self.filename = filename
  self.in_watch = {}
  return o
end

function Save:load(filename)
  print("saves/" .. filename)
  local content = ""

  for line in io.lines("saves/" .. filename) do
    content = content .. line
  end
  
  local save = Save:new(filename)

  local t = xenos.json.parse(content)
  save:set(t)

  return save
end

function Save:add(name, ref)
  self.in_watch[name] = ref
end

function Save:set(in_watch)
  self.in_watch = in_watch
end

function Save:get()
  return self.in_watch
end

function Save:execute()
  lfs.mkdir("saves")

  local file = io.open("saves/" .. self.filename, "w")

  io.output(file)
  
  io.write(xenos.json.stringify(self.in_watch))

  io.close()
end

function Save:clear()
end
