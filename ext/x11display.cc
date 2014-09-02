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
#ifndef NDEBUG
#include <iostream>
#endif
#include "timer.hh"
#include "x11display.hh"
#include "x11window.hh"
#include "rubytools.hh"

using namespace std;

// bool X11Display::x11ThreadsInitialised = false;

VALUE X11Display::cRubyClass = Qnil;

X11Display::X11Display( const std::string &name ) throw (Error):
  m_display(NULL), m_quit( false )
{
  // if ( !x11ThreadsInitialised ) {
  //   XInitThreads();
  //   x11ThreadsInitialised = true;
  // }
  m_display = XOpenDisplay( name.empty() ?
                            (const char *)NULL : name.c_str() );
  ERRORMACRO( m_display != NULL, Error, , "Error opening X11-display "
              << ( name.empty() ? "$DISPLAY" : name.c_str() ) << "." );
}

void X11Display::eventLoop( int timeout, bool resetQuit )
  throw (Error)
{
  if ( resetQuit )
    m_quit = false;

  if ( timeout < INT_MAX ) {

#ifndef NDEBUG
    cerr << "Looping for " << timeout << " milliseconds" << endl;
#endif

    Timer t;

    // Remaining time in milliseconds.
    int usecs_remaining;

    do {

      // Check for remaining time.
      usecs_remaining = (int)( ( timeout * 0.001 - t.elapsed() ) * 1E+6 );
#ifndef NDEBUG
      cerr << usecs_remaining << " microseconds remaining" << endl;
#endif
      if ( usecs_remaining > 0 ) {
        // Initialise timeout-variable.
        struct timeval tval;
        tval.tv_usec = usecs_remaining % 1000000;
        tval.tv_sec  = usecs_remaining / 1000000;
	
        // Get file-descriptor to event-pipe.
        int fd = ConnectionNumber( m_display );
	
        // Create file-descriptor set.
        fd_set fds;
        FD_ZERO( &fds );
        FD_SET( fd, &fds );
	
        // Perform limited wait on event-pipe.
        select( fd + 1, &fds, NULL, NULL, &tval );
      };

      // Empty event-pipe.
      processEvents();

    } while ( !m_quit && usecs_remaining > 0 );

  } else {

#ifndef NDEBUG
    cerr << "Entered event loop without timeout" << endl;
#endif

    while ( !m_quit ) {

      XEvent event;

      // Wait for events.
      // XMaskEvent( m_display, mask, &event );
      XNextEvent( m_display, &event );

      // Process the event.
      handleEvent( event );

    };
  };

}

X11Display::~X11Display(void)
{
#ifndef NDEBUG
  cerr << "Destroying X11-display." << endl;
#endif
  XCloseDisplay( m_display );
}

int X11Display::width(void)
{
  return DisplayWidth( m_display, DefaultScreen( m_display ) );
}

int X11Display::height(void)
{
  return DisplayHeight( m_display, DefaultScreen( m_display ) );
}

static Bool alwaysTrue( Display *, XEvent *, XPointer )
{
  return True;
}

void X11Display::processEvents(void) throw (Error)
{
  // Check for pending events.
  XEvent event;
  //while ( XCheckMaskEvent( m_display, 0xFFFF, &event ) == True ) {
  //  handleEvent( event );
  //};
#ifndef NDEBUG
  cerr << "Processing pending events" << endl;
#endif
  while ( XCheckIfEvent( m_display, &event, alwaysTrue, NULL ) )
    handleEvent( event );
}

void X11Display::handleEvent( XEvent &event ) throw (Error)
{
#ifndef NDEBUG
  cerr << "Getting event for window " << event.xany.window << endl;
#endif
  // Find affected window.
  map< Window, X11Window * >::iterator entry =
    m_windows.find( event.xany.window );

  // Forward event.
  if ( entry != m_windows.end() )
    entry->second->handleEvent( event );
}

void X11Display::addWindow( X11Window *win )
{
#ifndef NDEBUG
  cerr << "Registering window: " << win->get() << endl;
#endif
  m_windows[ win->get() ] = win;
}

void X11Display::removeWindow( X11Window *win )
{
#ifndef NDEBUG
  cerr << "Removing window: " << win->get() << endl;
#endif
  m_windows.erase( win->get() );
}

VALUE X11Display::registerRubyClass( VALUE module )
{
  cRubyClass = rb_define_class_under( module, "X11Display", rb_cObject );
  rb_define_const( cRubyClass, "INFINITE", INT2NUM(INT_MAX) );
  rb_define_singleton_method( cRubyClass, "new",
                              RUBY_METHOD_FUNC( wrapNew ), 1 );
  rb_define_method( cRubyClass, "width",
                    RUBY_METHOD_FUNC( wrapWidth ), 0 );
  rb_define_method( cRubyClass, "height",
                    RUBY_METHOD_FUNC( wrapHeight ), 0 );
  rb_define_method( cRubyClass, "event_loop",
                    RUBY_METHOD_FUNC( wrapEventLoop ), 1 );
  rb_define_method( cRubyClass, "process_events",
                    RUBY_METHOD_FUNC( wrapProcessEvents ), 0 );
  rb_define_method( cRubyClass, "status?",
                    RUBY_METHOD_FUNC( wrapStatus ), 0 );
  rb_define_method( cRubyClass, "status=",
                    RUBY_METHOD_FUNC( wrapSetStatus ), 1 );
  return cRubyClass;
}

void X11Display::deleteRubyObject( void *ptr )
{
  delete (X11DisplayPtr *)ptr;
}

VALUE X11Display::wrapNew( VALUE rbClass, VALUE rbName )
{
  VALUE retVal = Qnil;
  try {
    rb_check_type( rbName, T_STRING );
    X11DisplayPtr ptr( new X11Display( StringValuePtr( rbName ) ) );
    retVal = Data_Wrap_Struct( rbClass, 0, X11Display::deleteRubyObject,
                               new X11DisplayPtr( ptr ) );
  } catch ( std::exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  };
  return retVal;
}

VALUE X11Display::wrapWidth( VALUE rbSelf )
{
  X11DisplayPtr *self; Data_Get_Struct( rbSelf, X11DisplayPtr, self );
  return INT2NUM( (*self)->width() );
}

VALUE X11Display::wrapHeight( VALUE rbSelf )
{
  X11DisplayPtr *self; Data_Get_Struct( rbSelf, X11DisplayPtr, self );
  return INT2NUM( (*self)->height() );
}

VALUE X11Display::wrapEventLoop( VALUE rbSelf, VALUE rbTimeout )
{
  try {
    X11DisplayPtr *self; Data_Get_Struct( rbSelf, X11DisplayPtr, self );
    (*self)->eventLoop( NUM2INT( rbTimeout ), true );
  } catch ( std::exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  };
  return rbSelf;
}

VALUE X11Display::wrapProcessEvents( VALUE rbSelf )
{
  try {
    X11DisplayPtr *self; Data_Get_Struct( rbSelf, X11DisplayPtr, self );
    (*self)->processEvents();
  } catch ( std::exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  };
  return rbSelf;
}

VALUE X11Display::wrapStatus( VALUE rbSelf )
{
  X11DisplayPtr *self; Data_Get_Struct( rbSelf, X11DisplayPtr, self );
  bool retVal = !(*self)->quit();
  VALUE rbRetVal = retVal ? Qtrue : Qfalse;
  return rbRetVal;
}

VALUE X11Display::wrapSetStatus( VALUE rbSelf, VALUE rbStatus )
{
  X11DisplayPtr *self; Data_Get_Struct( rbSelf, X11DisplayPtr, self );
  (*self)->setQuit( rbStatus == Qfalse );
  return rbSelf;
}

