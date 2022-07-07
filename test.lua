local sprite_test = sprite.load_file("player.png")

--[[
save = Save:new("asd.json")

a = {}
a.x = {}
a.x.b = {1,2,3}

save:add("lll", a)
save:execute()
--]]

save = Save:load("asd.json")
t = save:get()

print(t.lll.x.b[1])

function process(delta)
end

function draw()
  local keyboard = xenos.keyboard
--  sprite.draw(sprite_test, { x = mouse.get_position().x, y = mouse.get_position().y, angle = x })
end
