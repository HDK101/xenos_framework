local sprite_test = sprite.load_file("player.png")
-- sprite.destroy(sprite_test)
-- sprite.load_file("test.png")

local x = 1


local sample = sound.load_sample("audio_elephant.wav")
sound.play(sample)

music.load_stream("test.ogg")
music.play()
music.load_stream("test.ogg")
music.play()
music.load_stream("test.ogg")
music.play()
music.load_stream("test.ogg")
music.play()

function process(delta)
  print(delta)
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
end
