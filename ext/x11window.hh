
/* HornetsEye - Computer Vision with Ruby
   Copyright (C) 2006, 2007, 2008 ,2009 Jan Wedekind
   
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
#ifndef HORNETSEYE_X11WINDOW_HH
#define HORNETSEYE_X11WINDOW_HH

#include <boost/shared_ptr.hpp>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "x11display.hh"
#include "x11painter.hh"
#include "error.hh"

/** Class for opening X11 windows.

@date Fri Apr 29 23:10:01 UTC 2005 */
class X11Window
{
  friend class X11Display;
public:
  X11Window(X11DisplayPtr display, X11PainterPtr painter,
            int width, int height, Window parent = 0, bool border = true);
  virtual ~X11Window(void);
  int width(void) const { return m_width; }
  int height(void) const { return m_height; }
  void show(void) throw (Error);
  void close(void);
  void setTitle( const char *_title );
  XVisualInfo *visualInfo(void) { return m_visualInfo; }
  X11DisplayPtr display(void) { return m_display; }
  Window get(void) { return m_window; }
  void resize( int width, int height ) throw (Error);
  void repaint( bool x11Event = false ) throw (Error);
  GC gc(void) { return m_gc; }
  virtual void handleEvent( XEvent &event ) throw (Error);
  virtual void paintEvent( bool x11Event ) throw (Error);
  virtual void keyEvent( XKeyEvent &xkey ) throw (Error);
  static VALUE cRubyClass;
  static VALUE registerRubyClass( VALUE module );
  static void deleteRubyObject( void *ptr );
  static VALUE wrapNew( VALUE rbClass, VALUE rbDisplay, VALUE rbX11Output,
                        VALUE rbWidth, VALUE rbHeight, VALUE rbBorder );
  static VALUE wrapSetTitle( VALUE rbSelf, VALUE rbTitle );
  static VALUE wrapWidth( VALUE rbSelf );
  static VALUE wrapHeight( VALUE rbSelf );
  static VALUE wrapResize( VALUE rbSelf, VALUE rbWidth, VALUE rbHeight );
  static VALUE wrapShow( VALUE rbSelf );
  static VALUE wrapClose( VALUE rbSelf );
protected:
  static Bool waitForNotify( Display *, XEvent *e, char *arg );
  X11DisplayPtr m_display;
  XVisualInfo *m_visualInfo;
  Colormap m_colourMap;
  Window m_window;
  GC m_gc;
  X11PainterPtr m_painter;
  int m_width;
  int m_height;
  Atom m_wmProtocols;
  Atom m_wmDeleteWindow;
};

typedef boost::shared_ptr< X11Window > X11WindowPtr;

#endif

