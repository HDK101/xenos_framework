local sprite_test = sprite.load_file("player.png")
local sprite_slice = sprite.slice(sprite_test, {
  sx = 0,
  sy = 0,
  sw = 32,
  sh = 32
})

local cenario = sprite.load_file("cenarioportifolio.png")
local cenario_slice_1 = sprite.slice(cenario, {
  sx = 0,
  sy = 0,
  sw = 32,
  sh = 32
})

local cenario_slice_2 = sprite.slice(cenario, {
  sx = 32,
  sy = 0,
  sw = 32,
  sh = 32
})

local cenario_slice_3 = sprite.slice(cenario, {
  sx = 64,
  sy = 0,
  sw = 32,
  sh = 32
})

xenos.sprite_batch.set_target(cenario)
xenos.sprite_batch.add(cenario_slice_1, { x = 0, y = 0 })
xenos.sprite_batch.add(cenario_slice_2, { x = 32, y = 0 })
xenos.sprite_batch.add(cenario_slice_3, { x = 64, y = 0 })
xenos.sprite_batch.clear()

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
--  local keyboard = xenos.keyboard
--  sprite.draw(sprite_slice, { x = xenos.mouse.get_position().x, y = xenos.mouse.get_position().y, angle = xenos.mouse.get_position().y })
--  xenos.sprite_batch.draw()
end
