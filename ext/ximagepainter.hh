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
#ifndef HORNETSEYE_XIMAGEPAINTER_HH
#define HORNETSEYE_XIMAGEPAINTER_HH

#include <X11/X.h>
#include <boost/shared_ptr.hpp>
#include "x11painter.hh"
#include "x11display.hh"

class X11Window;

///
class XImagePainter: public X11Painter
{
public:
  ///
  XImagePainter(void) {}
  ///
  virtual void paint( bool x11Event ) throw (Error);
  ///
  virtual XVisualInfo *visualInfo( X11DisplayPtr display ) throw (Error);
  ///
  static VALUE cRubyClass;
  ///
  static VALUE registerRubyClass( VALUE module, VALUE cX11Output );
  ///
  static VALUE wrapNew( VALUE rbClass );
protected:
  ///
  XVisualInfo m_visualInfo;
  ///
  X11DisplayPtr m_display;
};

///
typedef boost::shared_ptr< XImagePainter > XImagePainterPtr;

#endif

