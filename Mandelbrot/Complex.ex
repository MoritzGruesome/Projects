# Moritz
# complex number {R,i}
defmodule Complex do

  def new(real, imaginary) do {real, imaginary} end

  def add(a,b) do # adds two complex numbers
  {r1,i1} = a
  {r2,i2} = b
  {r1+r2,i1+i2}
  end

  def sqr(a) do# squares a complex number
  {r,i} = a
  {(r*r)-(i*i),(2*r*i)}
  end

  def abs(a) do
  {r,i} = a
  :math.sqrt((r*r)+(i*i))
  end

end
