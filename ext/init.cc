/* HornetsEye - Computer Vision with Ruby
   Copyright (C) 2006, 2007, 2008, 2009, 2010   Jan Wedekind

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>. */
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include "rubyinc.hh"
#include "x11output.hh"
#include "ximagepainter.hh"
#include "openglimagepainter.hh"
#include "xvideoimagepainter.hh"
#include "x11display.hh"
#include "x11window.hh"

#ifdef WIN32
#define DLLEXPORT __declspec(dllexport)
#define DLLLOCAL
#else
#define DLLEXPORT __attribute__ ((visibility("default")))
#define DLLLOCAL __attribute__ ((visibility("hidden")))
#endif

extern "C" DLLEXPORT void Init_hornetseye_xorg(void);

extern "C" {

  void Init_hornetseye_xorg(void)
  {
    // XInitThreads();
    rb_eval_string( "require 'hornetseye_frame'" );
    VALUE rbHornetseye = rb_define_module( "Hornetseye" );
    X11Output::registerRubyClass( rbHornetseye );
    XImagePainter::registerRubyClass( rbHornetseye, X11Output::cRubyClass );
    OpenGLImagePainter::registerRubyClass( rbHornetseye, X11Output::cRubyClass );
    XVideoImagePainter::registerRubyClass( rbHornetseye, X11Output::cRubyClass );
    X11Display::registerRubyClass( rbHornetseye );
    X11Window::registerRubyClass( rbHornetseye );
    rb_require( "hornetseye_xorg_ext.rb" );
  }

}
