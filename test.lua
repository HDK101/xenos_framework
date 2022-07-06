local sprite_test = sprite.load_file("player.png")
print(xenos.keyboard)
print(xenos.mouse)
print(xenos.music)
print(xenos.sound)

error("asd")

a = xenos.json.parse("{ \"a\": 1 }")

print(xenos.json.stringify(a))

function process(delta)
--  print(delta)
end

function draw()
  local keyboard = xenos.keyboard

  if keyboard.press(keyboard.code.B) then
    print("Hello!")
  end

--  sprite.draw(sprite_test, { x = mouse.get_position().x, y = mouse.get_position().y, angle = x })
end
