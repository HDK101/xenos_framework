joj = "asdasd"
print(bigchungus.numba)

local sprite_test = sprite.load_file("player.png")
-- sprite.destroy(sprite_test)
-- sprite.load_file("test.png")

local x = 1

function draw()
  x = x + 1

  sprite.draw(sprite_test, { x = x, y = x, angle = x })

--    drawSprite()
end
