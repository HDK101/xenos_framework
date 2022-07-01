local sprite_test = sprite.load_file("player.png")
-- sprite.destroy(sprite_test)
-- sprite.load_file("test.png")

local x = 1

print("walter")

function dumpTable(table, depth)
  if (depth > 200) then
    print("Error: Depth > 200 in dumpTable()")
    return
  end
  for k,v in pairs(table) do
    if (type(v) == "table") then
      print(string.rep("  ", depth)..k..":")
      dumpTable(v, depth+1)
    else
      print(string.rep("  ", depth)..k..": ",v)
    end
  end
end


print(json_test.array[1].a)
dumpTable(json_test, 199)

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
