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

/** Wrapper for X11-display.
    This wrapper provides automatic destruction for an
    X11-display-variable.

    @date Fri Apr 08 10:07:26 UTC 2005 */
class X11Display
{
public:
  /** Constructor.
      Open X11-display.
      @param name Name of X11-display to use (default: use content of
      <TT>DISPLAY</TT>-variable. */
  X11Display( const std::string &name = "" )
    throw (Error);
  /// Destructor.
  virtual ~X11Display(void);
  /// Get pointer to X11-display.
  Display *get(void) { return m_display; }
  /// Get width of screen.
  int width(void);
  /// Get height of screen.
  int height(void);
  /** Loop for events.
      Loop for events until \c quit() is called by an \c X11Window object or
      until a time-out occurs.
      @param timeout Time limit for the event-loop in milliseconds. Specify
      \c INT_MAX for no limit (default).
      @see processEvents */
  void eventLoop( int timeout = INT_MAX, bool resetQuit = false )
    throw (Error);
  /** Process pending events.
      @see eventLoop */
  void processEvents(void)
    throw (Error);
  ///
  void addWindow( X11Window *win );
  ///
  void removeWindow( X11Window *win );
  ///
  void setQuit( bool quit ) { m_quit = quit; }
  ///
  bool quit(void) { return m_quit; }
  ///
  static VALUE cRubyClass;
  ///
  static VALUE registerRubyClass( VALUE module );
  ///
  static void deleteRubyObject( void *ptr );
  ///
  static VALUE wrapNew( VALUE rbClass, VALUE rbName );
  ///
  static VALUE wrapWidth( VALUE rbSelf );
  ///
  static VALUE wrapHeight( VALUE rbSelf );
  ///
  static VALUE wrapEventLoop( VALUE rbSelf, VALUE rbTimeout );
  ///
  static VALUE wrapProcessEvents( VALUE rbSelf );
  ///
  static VALUE wrapStatus( VALUE rbSelf );
  ///
  static VALUE wrapSetStatus( VALUE rbSelf, VALUE rbStatus );
protected:
  //
  // static bool x11ThreadsInitialised;
  ///
  void handleEvent( XEvent &event ) throw (Error);
  /// X11 display-variable.
  Display *m_display;
  /// Map of event-handlers.
  std::map< Window, X11Window * > m_windows;
  ///
  bool m_quit;
};

///
typedef boost::shared_ptr< X11Display > X11DisplayPtr;

#endif
