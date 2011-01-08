hornetseye-xorg
===============

**Author**:       Jan Wedekind
**Copyright**:    2010
**License**:      GPL

Synopsis
--------

This Ruby extension provides graphical output under X.Org.

Installation
------------

*hornetseye-xorg* requires the X.Org headers. If you are running Debian or (K)ubuntu, you can install them like this:

    $ sudo aptitude install xserver-xorg-dev

To install this Ruby extension, use the following command:

    $ sudo gem install hornetseye-xorg

Alternatively you can build and install the Ruby extension from source as follows:

    $ rake
    $ sudo rake install

Usage
-----

Simply run Interactive Ruby:

    $ irb

You can load and use the X.Org wrappers as shown below. This example shows how to display an image:

    require 'rubygems'
    require 'hornetseye_xorg'
    include Hornetseye
    img = lazy( 640, 480 ) { RGB 255, 0, 0 }
    img.show

Here is another example. It demonstrates display of a video:

    require 'rubygems'
    require 'hornetseye_xorg'
    include Hornetseye
    X11Display.show do
      lazy( 320, 240 ) { RGB ( Time.new.to_f * 256 ) % 256, 0, 0 }
    end

