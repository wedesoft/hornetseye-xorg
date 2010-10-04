/* HornetsEye - Computer Vision with Ruby
   Copyright (C) 2006, 2007, 2008, 2009 Jan Wedekind
   
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
#ifndef HORNETSEYE_X11PAINTER_HH
#define HORNETSEYE_X11PAINTER_HH

#include <X11/Xutil.h>
#include "x11display.hh"

class X11Window;
class X11Output;

class X11Painter {
public:
  X11Painter(void): m_imageSource(NULL), m_window(NULL) {}
  virtual ~X11Painter(void) {}
  virtual void paint( bool x11Event ) throw (Error) = 0;
  X11Window *window(void) { return m_window; }
  void registerWindow( X11Window *window ) { m_window = window; }
  virtual void unregisterWindow(void) { m_window = NULL; }
  void registerImageSource( X11Output *imageSource )
  { m_imageSource = imageSource; }
  void unregisterImageSource(void) { m_imageSource = NULL; }
  virtual XVisualInfo *visualInfo( X11DisplayPtr display ) throw (Error) = 0;
protected:
  X11Output *m_imageSource;
  X11Window *m_window;
};

typedef boost::shared_ptr< X11Painter > X11PainterPtr;

#endif

