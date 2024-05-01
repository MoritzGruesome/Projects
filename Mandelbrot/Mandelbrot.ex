

# Moritz

# - if abs(zN) > 2 then c is not in mandel brot set
# - we set upper limit n which is the depth of the search
# - when we know that a number does not belong to the mandbrot set
#   we have a number i at which abs(zi) > 2

defmodule Brot do
  # C is the given complex number
  # maximum depth m
  def mandelbrot(c, m) do
    z0 = Complex.new(0,0)
    i = 0
    test(i, z0, c, m)
    end

    defp test(m, zN, c, m) do
      0
    end

    defp test(i, zN, c, m) do
      zNext = Complex.add(Complex.sqr(zN),c)
      if (Complex.abs(zNext) > 2) do
        i
      else
        test(i+1, zNext, c, m)
      end
    end
end


defmodule Mandel do

  # generate image of size width by height
  # upper left corner is x + yi
  # offset between two points is k
  # first pixels of upper row corresponds to depth
  # first pixels of uperrow correspond to "depth" of
  # x+yi, (x+k) + yi, (x+2k) + yi

  # trans function takes a pixel at position (w,h) and return complex number
  # that is the one we compute depth of

  # rows function return list of rows, where each row is a list of colours


  def mandelbrot(width, height, x, y, k, depth) do
    trans = fn(w, h) ->
      Complex.new(x + k * (w - 1), y - k * (h - 1))
    end
    rows(width, height, trans, depth, [])
  end

  def rows(width, height, trans, depth, list) when width > 0 do
    c = trans.(width, height)
    newpix = Brot.mandelbrot(c,depth) |> Colours.convert(depth)
    rows(width-1, height, trans, depth, [newpix | list])
  end
  def rows(0, height, trans, depth, list) when height > 0 do
    [list | rows(length(list),height-1,trans, depth, [])]
  end
  def rows(0,0, _, _, list) do
    [list]
  end

  # (0,500) . . . (500,500)
  # .
  # .
  # .
  # (0,0)
end

defmodule Demo do
def demo() do
  small(-0.45, 2, 0.034)
end
def small(x0, y0, xn) do
  width = 1920
  height = 1080
  depth = 64
  k = (xn - x0) / width
  image = Mandel.mandelbrot(width, height, x0, y0, k, depth)
  IO.inspect(image)
  PPM.write("small.png", image)
end

def test do

list =
  [
    [
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0}
    ],
    [
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0}
    ],
    [
      {:rgb, 102.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0}
    ],
    [
      {:rgb, 204.0, 0, 0},
      {:rgb, 102.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0}
    ],
    [
      {:rgb, 102.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0}
    ],
    [
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0},
      {:rgb, 0.0, 0, 0}
    ]
  ]
  PPM.write("test.ppm",list)



end
















end
