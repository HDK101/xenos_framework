local sprite_test = sprite.load_file("player.png")
-- sprite.destroy(sprite_test)
-- sprite.load_file("test.png")

a = {}
a.x = {}
a.x.b = 1
a.x.a = {}
a.x.a.a = 2
a.x.a.b = 2

str = json.stringify({1,2,"asdasd", false, a})
-- json.stringify(a)
print(str)

-- parsed = json.parse("{ \"a\": 1 }")
parsed = json.parse("{ 1 }")

print(parsed.a)

--local sample = sound.load_sample("audio_elephant.wav")
-- sound.play(sample)

--music_test = music.load_stream("test.ogg")
--print(music_test.index)
--print(music_test.id)
--music.play(music_test)

function process(delta)
--  print(delta)
end

function draw()
  x = x + 1

  sprite.draw(sprite_test, { x = mouse.get_position().x, y = mouse.get_position().y, angle = x })
  if keyboard.press(keyboard.code.LWIN) then
    print("Hello")
  end
  if keyboard.release(keyboard.code.LWIN) then
    print("Bye")
  end

  print(mouse.get_position().x)
  print(mouse.get_position().y)

  if mouse.press(0) then
    print("Mouse press!")
  end
  
  if mouse.release(0) then
    print("Mouse release!")
  end
  
  if mouse.press(2) then
    print("Mouse press!")
  end
  
  if mouse.release(2) then
    print("Mouse release!")
  end
end
