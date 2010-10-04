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
#ifndef HORNETSEYE_X11DISPLAY_HH
#define HORNETSEYE_X11DISPLAY_HH

#include <boost/shared_ptr.hpp>
#include <map>
#include "rubyinc.hh"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include "error.hh"

class X11Window;

class X11Display
{
public:
  X11Display( const std::string &name = "" )
    throw (Error);
  virtual ~X11Display(void);
  Display *get(void) { return m_display; }
  int width(void);
  int height(void);
  void eventLoop( int timeout = INT_MAX, bool resetQuit = false )
    throw (Error);
  void processEvents(void)
    throw (Error);
  void addWindow( X11Window *win );
  void removeWindow( X11Window *win );
  void setQuit( bool quit ) { m_quit = quit; }
  bool quit(void) { return m_quit; }
  static VALUE cRubyClass;
  static VALUE registerRubyClass( VALUE module );
  static void deleteRubyObject( void *ptr );
  static VALUE wrapNew( VALUE rbClass, VALUE rbName );
  static VALUE wrapWidth( VALUE rbSelf );
  static VALUE wrapHeight( VALUE rbSelf );
  static VALUE wrapEventLoop( VALUE rbSelf, VALUE rbTimeout );
  static VALUE wrapProcessEvents( VALUE rbSelf );
  static VALUE wrapStatus( VALUE rbSelf );
  static VALUE wrapSetStatus( VALUE rbSelf, VALUE rbStatus );
protected:
  void handleEvent( XEvent &event ) throw (Error);
  Display *m_display;
  std::map< Window, X11Window * > m_windows;
  bool m_quit;
};

typedef boost::shared_ptr< X11Display > X11DisplayPtr;

#endif
