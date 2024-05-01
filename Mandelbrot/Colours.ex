
# Moritz
defmodule Colours do

  # conversion of depth to RGB:
  # depth of a point d, maximum depth m
  # create 5 section diving range 0-m
  # f = (d / m) * 4
  # truncate(f) = x = {0..4} (integer)
  # offset y = 255 * (a - x)
  def convert(depth, max) do
    a = (depth / max) * 4
    x = trunc(a)
    y = trunc(255*(a-x))

    case (x) do
      0 -> {:rgb, y,0,0}
      1 -> {:rgb, 255,y,0}
      2 -> {:rgb, 255-y,255,0}
      3 -> {:rgb, 0,255,y}
      4 -> {:rgb, 0, 255-y, 255}
      #_ -> :nul
    end

  end



end
