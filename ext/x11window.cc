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
#include <X11/keysym.h>
#include <X11/xpm.h>
#include <stdint.h>
#include "rubytools.hh"
#include "x11display.hh"
#include "x11output.hh"
#include "x11window.hh"

using namespace std;

// created from favicon.ico
static const char *hornetseye_xpm[] = {
  "16 16 152 2",
  "   c black",
  ".  c #6FB062",
  "X  c #2DD1B8",
  "o  c #37D7C6",
  "O  c #5B9091",
  "+  c #5A9092",
  "@  c #5A9192",
  "#  c #5A9290",
  "$  c #5C9192",
  "%  c #5D9393",
  "&  c #659187",
  "*  c #7FBF86",
  "=  c #40C8A6",
  "-  c #46D4BE",
  ";  c #59CDAB",
  ":  c #7BC79F",
  ">  c #64CAA2",
  ",  c #64CCA8",
  "<  c #67CDAA",
  "1  c #6ACEA9",
  "2  c #69D1B3",
  "3  c #5DDFD5",
  "4  c #4CE4DF",
  "5  c #66DEC9",
  "6  c #73DBC6",
  "7  c #79E5DC",
  "8  c #6CE8EA",
  "9  c #79EBED",
  "0  c #87B35F",
  "q  c #89B258",
  "w  c #8ABE7A",
  "e  c #AFBB5D",
  "r  c #AFBA62",
  "t  c #BEC062",
  "y  c #B8C36F",
  "u  c #B8F149",
  "i  c #B9F24A",
  "p  c #B8F04C",
  "a  c #B9F14D",
  "s  c #BDF04E",
  "d  c #FF962F",
  "f  c #FF9B39",
  "g  c #DCB642",
  "h  c #DABA48",
  "j  c #E3BD55",
  "k  c #E4BE58",
  "l  c #FFB369",
  "z  c #C1C268",
  "x  c #D6C774",
  "c  c #D1C879",
  "v  c #E7C363",
  "b  c #EAC86C",
  "n  c #8ACA9B",
  "m  c #96C587",
  "M  c #99C589",
  "N  c #81CFA6",
  "B  c #8CCEA3",
  "V  c #80D0A9",
  "C  c #9CD6B5",
  "Z  c #A5C787",
  "A  c #A4C890",
  "S  c #A8C0B8",
  "D  c #84E0D0",
  "F  c #8FE3D2",
  "G  c #90E0C8",
  "H  c #99E5D7",
  "J  c #9EE4D4",
  "K  c #85E9E1",
  "L  c #86EEEB",
  "P  c #86EDEE",
  "I  c #8BEEEF",
  "U  c #8FEDEC",
  "Y  c #96EAE1",
  "T  c #95EFEE",
  "R  c #94EFF0",
  "E  c #98EFF0",
  "W  c #90F0EF",
  "Q  c #95F1F0",
  "!  c #A0C0C0",
  "~  c #A1C1C1",
  "^  c #A3C1C1",
  "/  c #A2C2C0",
  "(  c #B0E0C8",
  ")  c #B5E1CA",
  "_  c #B0EADB",
  "`  c #B3EBDE",
  "'  c #AAF2EF",
  "]  c #B5F4F5",
  "[  c #B5F5F5",
  "{  c #DDCD81",
  "}  c #D9D18D",
  "|  c #C4DCB4",
  " . c #D1DCB2",
  ".. c #D1DFBB",
  "X. c #D6F698",
  "o. c #D7F69A",
  "O. c #D9F797",
  "+. c #D8F79B",
  "@. c #D9F69A",
  "#. c #E4CC82",
  "$. c #EAD99E",
  "%. c #EBDA9F",
  "&. c #EDD99D",
  "*. c #FFC083",
  "=. c #FFC68F",
  "-. c #FFC995",
  ";. c #EBDBA3",
  ":. c #F1DEA9",
  ">. c #F2DFAE",
  ",. c #FFDFBF",
  "<. c #EEE1B2",
  "1. c #EEE4B8",
  "2. c #F4E1B2",
  "3. c #FFE0BF",
  "4. c #D3DFDF",
  "5. c #D7DFDB",
  "6. c #D3DFE1",
  "7. c #D8E5C3",
  "8. c #DAE6C8",
  "9. c #CDF9F7",
  "0. c #CDF8F9",
  "q. c #D3E1E1",
  "w. c #D2E0E2",
  "e. c #D2E2E2",
  "r. c #D4E2E2",
  "t. c #D0F8F5",
  "y. c #D2FAF7",
  "u. c #D5F8F9",
  "i. c #D6FAF8",
  "p. c #D9F9F9",
  "a. c #ECFBCF",
  "s. c #EDFACE",
  "d. c #EDFBCF",
  "f. c #ECFAD2",
  "g. c #F7E9C7",
  "h. c #F7EDCF",
  "j. c #F8EAC8",
  "k. c #FAEED2",
  "l. c #FAF0D6",
  "z. c #FAF1DA",
  "x. c #E3FBFB",
  "c. c #E7FBFB",
  "v. c #E4FCFA",
  "b. c #ECFBFC",
  "n. c #EDFCFD",
  "m. c #F5FDFD",
  "M. c #FAFEFE",
  "N. c #FBFFFF",
  "B. c #FFFDFB",
  "V. c #FEFEFE",
  "C. c gray100",
  "Z. c None",
  "Z.Z.,.,.,.,.,.,.,.a.a.a.a.f.s.a.",
  "Z.-.*.*.*.*.*.*.*.X.@.@.@.+.O.X.",
  "=.l f d d d d d d i i a s s i i ",
  "5.S & 7.8._ 1.V.V.V.v.)  ...J } ",
  "r.^ % p.i.' &.V.V.V.v.C t.9.T { ",
  "e.^ % V.V.x.( >.V.V.H <.V.V.n.F ",
  "e.^ + V.V.V.E B n N A z.V.V.V.Q ",
  "e.^ + V.V.m.: h.V.N.K b V.V.V.2 ",
  "4.! O ;.;.D c V.V.V.[ Z #.;.G r ",
  "4.! # Q I 3 k V.V.V.c.= L L 4 h ",
  "r.^ % V.V.[ M :.V.V.7 x V.V.t.V ",
  "r.^ % V.V.V.8 0 w * . l.V.V.V.9 ",
  "e.^ + V.V.V.; | i.9.6 v V.V.V.5 ",
  "e.! + g.h.Y e V.V.V.W z 2.h.` q ",
  "4.! + < < - j V.V.V.n.X > , o g ",
  "e.^ % V.V.Q y j.V.V.U t V.V.[ M "
};

VALUE X11Window::cRubyClass = Qnil;

X11Window::X11Window( X11DisplayPtr display, X11PainterPtr painter,
                      int width, int height, Window parent ):
  m_display(display), m_painter(painter), m_width(width), m_height(height)
{
  // state = true;
  ERRORMACRO( width > 0 && height > 0, Error, ,
              width << 'x' << height << " is an illegal window size." );

  m_visualInfo = painter->visualInfo( display );
  /* // Request true-colour X11 visual.
     if ( !XMatchVisualInfo( display->get(),
     DefaultScreen( display->get() ),
     24, TrueColor, &m_visualInfo) ) {
     if ( !XMatchVisualInfo( display->get(),
     DefaultScreen( display->get() ),
     16, TrueColor, &m_visualInfo ) ) {
     if ( !XMatchVisualInfo( display->get(),
     DefaultScreen( display->get() ),
     15, TrueColor, &m_visualInfo ) ) {
     ERRORMACRO( false, Error, ,
     "Could not get X11 visual for true-colour display." );
     };
     };
     }; */

  // Create a color map.
  m_colourMap = XCreateColormap( m_display->get(),
                                 DefaultRootWindow( display->get() ),
                                 m_visualInfo->visual, AllocNone );
  ERRORMACRO( m_colourMap != 0, Error, ,
              "Couldn't allocate X11 colormap." );
      
  try {
        
    // http://space-out.net/readarticle.php?article=1
    XSetWindowAttributes attributes;
    attributes.colormap = m_colourMap;
    attributes.event_mask = KeyPressMask | ExposureMask | StructureNotifyMask;

#ifndef NDEBUG
    cerr << "Creating window of size " << width << 'x' << height << endl;
#endif

    if ( parent == 0 )
      parent = RootWindow( display->get(),
                           m_visualInfo->screen );

    m_window = XCreateWindow( display->get(),
                              parent,
                              0, 0, width, height,
                              0, m_visualInfo->depth, InputOutput,
                              m_visualInfo->visual,
                              CWColormap | CWEventMask,
                              &attributes );
    ERRORMACRO( m_window != 0, Error, , "Error creating X11 window." );

    wmProtocols = XInternAtom( display->get(), "WM_PROTOCOLS", False );
    wmDeleteWindow = XInternAtom( display->get(), "WM_DELETE_WINDOW", False );
    XSetWMProtocols( display->get(), m_window, &wmDeleteWindow, 1 );
    XWMHints wmHints;
    XpmCreatePixmapFromData( display->get(), m_window,
                             (char **)hornetseye_xpm,
                             &wmHints.icon_pixmap,
                             &wmHints.icon_mask,
                             0 );
    wmHints.flags = IconPixmapHint | IconMaskHint;
    XSetWMHints( display->get(), m_window, &wmHints );

    try {

      painter->registerWindow( this );   
          
      // Add window to list of display-class.
      display->addWindow( this );

      // Create graphics context.
      XGCValues xgcv;
      m_gc = XCreateGC( display->get(), m_window, 0L, &xgcv );

    } catch ( Error &e ) {

      display->removeWindow( this );
      XDestroyWindow( display->get(), m_window );
      painter->unregisterWindow();
      throw e;
      
    };

  } catch ( Error &e ) {
    
    XFreeColormap( display->get(), m_colourMap );
    throw e;
    
  };
      
}

X11Window::~X11Window(void)
{
#ifndef NDEBUG
  cerr << "Destroying X11-window." << endl;
#endif
  XFreeGC( m_display->get(), m_gc );
  m_display->removeWindow( this );
  XDestroyWindow( m_display->get(), m_window );
  m_painter->unregisterWindow();
  XFreeColormap( m_display->get(), m_colourMap );
}

void X11Window::show(void) throw (Error)
{
  XEvent event;
  XMapWindow( m_display->get(), m_window );
  // Wait for X-server.
  XIfEvent( m_display->get(), &event, waitForNotify, (char *)m_window );
  repaint( true );
}

void X11Window::close(void)
{
  XEvent event;
  XUnmapWindow( m_display->get(), m_window );
  // Wait for X-server.
  XIfEvent( m_display->get(), &event, waitForNotify, (char *)m_window );
}

void X11Window::setTitle( const char *_title )
{
  char *title = strdup( _title );
  XStoreName( m_display->get(), m_window, title );
  free( title );
}

Bool X11Window::waitForNotify( Display *, XEvent *e, char *arg )
{
  return ( e->type == MapNotify || e->type == UnmapNotify ) &&
    ( e->xmap.window == (Window)arg );
}

void X11Window::resize( int width, int height ) throw (Error)
{
  ERRORMACRO( width > 0 && height > 0, Error, ,
              "Cannot resize window to " << width << 'x' << height );
  if ( m_width != width || m_height != height ) {
#ifndef NDEBUG
    cerr << "Resizing to " << width << "x" << height << endl;
#endif
    XResizeWindow( m_display->get(), m_window, width, height );
    m_width = width;
    m_height = height;
    XFlush( m_display->get() );
  };
}

void X11Window::repaint( bool x11Event ) throw (Error)
{
  paintEvent( x11Event );
}

void X11Window::handleEvent( XEvent &event ) throw (Error)
{
#ifndef NDEBUG
  cerr << "X11Window::handleEvent()" << endl;
#endif
  switch ( event.type ) {
  case ConfigureNotify:
#ifndef NDEBUG
    cerr << "ConfigureNotify event" << endl;
#endif
    while ( XCheckTypedWindowEvent( m_display->get(), m_window,
                                    ConfigureNotify, &event ) );
    m_width = event.xconfigure.width;
    m_height = event.xconfigure.height;
    paintEvent( true );
    break;
  case ClientMessage:
    if ( ( event.xclient.message_type == wmProtocols ) &&
         ( (Atom)event.xclient.data.l[0] == wmDeleteWindow ) ) {
#ifndef NDEBUG
      cerr << "Delete message" << endl;
#endif
      m_display->setQuit( true );
    };
    break;
  case Expose:
#ifndef NDEBUG
    cerr << "Expose event" << endl;
#endif
    while ( XCheckTypedWindowEvent( m_display->get(), m_window,
                                    Expose, &event ) );
    paintEvent( true );
    break;
  case KeyPress:
#ifndef NDEBUG
    cerr << "Key event" << endl;
#endif
    keyEvent( event.xkey );
    break;
  default:
    break;
  };
}

void X11Window::paintEvent( bool x11Event ) throw (Error)
{
#ifndef NDEBUG
  cerr << "X11Window::paintEvent( " << x11Event << " )" << endl;
#endif
  m_painter->paint( x11Event );
}

void X11Window::keyEvent( XKeyEvent &xkey ) throw (Error)
{
  switch ( xkey.keycode ) {
  case 0x09: // escape
  case 0x41: // space
#ifndef NDEBUG
    cerr << "Escape/Space -> Quit!" << endl;
#endif
    m_display->setQuit( true );
    break;
  default:
    break;
  };
}

VALUE X11Window::registerRubyClass( VALUE module )
{
  cRubyClass = rb_define_class_under( module, "X11Window", rb_cObject );
  rb_define_singleton_method( cRubyClass, "new",
                              RUBY_METHOD_FUNC( wrapNew ), 4 );
  rb_define_method( cRubyClass, "title=",
                    RUBY_METHOD_FUNC( wrapSetTitle ), 1 );
  rb_define_method( cRubyClass, "width",
                    RUBY_METHOD_FUNC( wrapWidth ), 0 );
  rb_define_method( cRubyClass, "height",
                    RUBY_METHOD_FUNC( wrapHeight ), 0 );
  rb_define_method( cRubyClass, "resize",
                    RUBY_METHOD_FUNC( wrapResize ), 2 );
  rb_define_method( cRubyClass, "show",
                    RUBY_METHOD_FUNC( wrapShow ), 0 );
  rb_define_method( cRubyClass, "close",
                    RUBY_METHOD_FUNC( wrapClose ), 0 );
  return cRubyClass;
}

void X11Window::deleteRubyObject( void *ptr )
{
  delete (X11WindowPtr *)ptr;
}

VALUE X11Window::wrapNew( VALUE rbClass, VALUE rbDisplay, VALUE rbX11Output,
                          VALUE rbWidth, VALUE rbHeight )
{
  VALUE retVal = Qnil;
  try {
    checkStruct( rbDisplay, X11Display::cRubyClass );
    X11DisplayPtr *display;
    dataGetStruct( rbDisplay, X11Display::cRubyClass, X11DisplayPtr,
                   display );
    checkStruct( rbX11Output, X11Output::cRubyClass );
    X11OutputPtr *x11Output;
    dataGetStruct( rbX11Output, X11Output::cRubyClass, X11OutputPtr,
                   x11Output );
    X11WindowPtr ptr
      ( new X11Window( *display, (*x11Output)->painter(),
                       NUM2INT( rbWidth ), NUM2INT( rbHeight ) ) );
    retVal = Data_Wrap_Struct( rbClass, 0, X11Window::deleteRubyObject,
                               new X11WindowPtr( ptr ) );
  } catch ( std::exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  };
  return retVal;
}

VALUE X11Window::wrapSetTitle( VALUE rbSelf, VALUE rbTitle )
{
  X11WindowPtr *self; Data_Get_Struct( rbSelf, X11WindowPtr, self );
  (*self)->setTitle( StringValuePtr( rbTitle ) );
  return rbTitle;
}

VALUE X11Window::wrapWidth( VALUE rbSelf )
{
  X11WindowPtr *self; Data_Get_Struct( rbSelf, X11WindowPtr, self );
  return INT2NUM( (*self)->width() );
}

VALUE X11Window::wrapHeight( VALUE rbSelf )
{
  X11WindowPtr *self; Data_Get_Struct( rbSelf, X11WindowPtr, self );
  return INT2NUM( (*self)->height() );
}

VALUE X11Window::wrapResize( VALUE rbSelf, VALUE rbWidth, VALUE rbHeight )
{
  try {
    X11WindowPtr *self; Data_Get_Struct( rbSelf, X11WindowPtr, self );
    (*self)->resize( NUM2INT( rbWidth ), NUM2INT( rbHeight ) );
  } catch ( std::exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  };
  return rbSelf;
}

VALUE X11Window::wrapShow( VALUE rbSelf )
{
  try {
    X11WindowPtr *self; Data_Get_Struct( rbSelf, X11WindowPtr, self );
    (*self)->show();
  } catch ( std::exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  };
  return rbSelf;
}

VALUE X11Window::wrapClose( VALUE rbSelf )
{
  X11WindowPtr *self; Data_Get_Struct( rbSelf, X11WindowPtr, self );
  (*self)->close();
  return rbSelf;
}

