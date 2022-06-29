local sprite_test = sprite.load_file("player.png")
-- sprite.destroy(sprite_test)
-- sprite.load_file("test.png")

local x = 1


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

  sprite.draw(sprite_test, { x = x, y = x, angle = x })
  if keyboard.press(keyboard.code.LWIN) then
    print("Hello")
  end
  if keyboard.release(keyboard.code.LWIN) then
    print("Bye")
  end

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
