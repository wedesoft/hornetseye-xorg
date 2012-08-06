#!/usr/bin/ruby
#/usr/bin/ruby -Ilib -Iext -rrubygems -rhornetseye_xorg -rhornetseye_ffmpeg
require 'rubygems'
require 'hornetseye_xorg'
require 'hornetseye_ffmpeg'
include Hornetseye
input = AVInput.new '/home/jan/Videos/V22.asf'
X11Display.show( input.width, input.height, :title => 'V22', :output => XVideoOutput,
                 :frame_rate => input.frame_rate, :border => true ) { input.read }

