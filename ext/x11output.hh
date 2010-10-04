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
#ifndef HORNETSEYE_X11OUTPUT_HH
#define HORNETSEYE_X11OUTPUT_HH

#include "frame.hh"
#include "x11painter.hh"

///
class X11Output
{
public:
  ///
  X11Output( X11PainterPtr painter );
  ///
  virtual ~X11Output(void);
  ///
  virtual void write( FramePtr frame ) throw (Error);
  ///
  virtual bool status(void) const;
  ///
  X11PainterPtr painter(void) { return m_painter; }
  ///
  FramePtr frame(void) { return m_frame; }
  ///
  static VALUE cRubyClass;
  ///
  static VALUE registerRubyClass( VALUE module );
  ///
  static void deleteRubyObject( void *ptr );
  ///
  static void markRubyObject( void *ptr );
  ///
  static VALUE wrapStatus( VALUE rbSelf );
  ///
  static VALUE wrapWrite( VALUE rbSelf, VALUE rbFrame );
protected:
  ///
  FramePtr m_frame;
  ///
  X11PainterPtr m_painter;
};

///
typedef boost::shared_ptr< X11Output > X11OutputPtr;

#endif

