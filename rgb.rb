#!/usr/bin/env ruby
require 'rubygems'
require 'hornetseye_xorg'
require 'hornetseye_ffmpeg'
include Hornetseye
input = AVInput.new 'http://vision.eng.shu.ac.uk/jan/hornetseye2010.avi'
dt = 1.0 / input.frame_rate
width, height = input.width, input.height
p width
p height
display = X11Display.new
output = (0 ... 4).collect { XImageOutput.new }
window = output.collect { |o| X11Window.new display, o, width, height }
window.each { |w| w.show }
t = Time.new.to_f
while display.status?
  img = input.read.to_ubytergb
  output[0].write img
  output[1].write img * RGB( 1, 0, 0 )
  output[2].write img * RGB( 0, 1, 0 )
  output[3].write img * RGB( 0, 0, 1 )
  display.event_loop [ t + dt - Time.new.to_f, 0 ].max
  t += dt
end

