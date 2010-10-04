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
   along with this program.  If not, see <http://www.gnu.org/licenses/>.

   *  Raster graphics library
   *
   *  Copyright (c) 1997-2002 Alfredo K. Kojima
   *
   *  This library is free software; you can redistribute it and/or
   *  modify it under the terms of the GNU Library General Public
   *  License as published by the Free Software Foundation; either
   *  version 2 of the License, or (at your option) any later version.
   *
   *  This library is distributed in the hope that it will be useful,
   *  but WITHOUT ANY WARRANTY; without even the implied warranty of
   *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   *  Library General Public License for more details.
   *
   *  You should have received a copy of the GNU Library General Public
   *  License along with this library; if not, write to the Free
   *  Software Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
*/
#include <boost/shared_array.hpp>
#include <errno.h>
#ifndef NDEBUG
#include <iostream>
#endif
#include "ximagepainter.hh"
#include "rubytools.hh"
#include "x11window.hh"
#include "x11output.hh"

using namespace boost;
using namespace std;

VALUE XImagePainter::cRubyClass = Qnil;

void XImagePainter::paint( bool ) throw (Error)
{
#ifndef NDEBUG
  cerr << "XImagePainter::paint()" << endl;
#endif

  assert( m_window != NULL );

  if ( m_imageSource != NULL ) {

#ifndef NDEBUG
    cerr << "Requesting image." << endl;
#endif
    FramePtr frame = m_imageSource->frame();

    if ( frame ) {
      // Create scaled BGRA data.
      int
        newWidth  = m_window->width(),
        newHeight = m_window->height();
      boost::shared_array< unsigned char > array
        ( new unsigned char[ newWidth * newHeight * 4 ] );
      const char *p = frame->data();
      int
        fw = frame->width(),
        dx = ( frame->width()  << 16 ) / newWidth,
        dy = ( frame->height() << 16 ) / newHeight,
        py = 0;
      unsigned char *d = array.get();
      for ( int y=0; y<newHeight; y++ ) {
        int t = fw * ( py >> 16 );
        const char *s = p + ( t << 1 ) + t;/* 3 * t */
        int
          ox = 0,
          px = 0;
        for ( int x=0; x<newWidth; x++ ) {
          px += dx;
          *d++ = s[2];
          *d++ = s[1];
          *d++ = s[0];
          *d++ = '\000';
          t = ( px - ox ) >> 16;
          ox += t << 16;
          s += ( t << 1 ) + t;// s += 3 * t
        }
        py += dy;
	};

      // Display the scaled BGRA data.
      XImage *xImage = XCreateImage( m_display->get(), m_visualInfo.visual,
                                     24, ZPixmap, 0,
                                     (char *)array.get(),
                                     newWidth, newHeight,
                                     32, newWidth * 4 );
      ERRORMACRO( xImage != NULL, Error, ,
                  "Failed to create X11-image" );
      xImage->byte_order = LSBFirst;
#ifndef NDEBUG
      cerr << "Window size is " << newWidth << 'x'
           << newHeight << endl;
#endif
      XPutImage( m_display->get(), m_window->get(), m_window->gc(),
                 xImage, 0, 0, 0, 0, newWidth, newHeight );
      xImage->data = (char *)NULL;
      XDestroyImage( xImage );
    };
    
  };
}


XVisualInfo *XImagePainter::visualInfo( X11DisplayPtr display )
  throw (Error)
{
  ERRORMACRO( XMatchVisualInfo( display->get(),
                                DefaultScreen( display->get() ) != 0,
                                24, TrueColor, &m_visualInfo), Error, ,
              "Could not get 24-bit true-colour visual." );
  m_display = display;
  return &m_visualInfo;
}

VALUE XImagePainter::registerRubyClass( VALUE module,
                                        VALUE cX11Output )
{
  cRubyClass = rb_define_class_under( module, "XImageOutput", cX11Output );
  rb_define_singleton_method( cRubyClass, "new",
                              RUBY_METHOD_FUNC( wrapNew ), 0 );
  return cRubyClass;
}

VALUE XImagePainter::wrapNew( VALUE rbClass )
{
  VALUE retVal = Qnil;
  X11PainterPtr painter( new XImagePainter );
  X11OutputPtr ptr( new X11Output( painter ) );
  retVal = Data_Wrap_Struct( rbClass,
                             X11Output::markRubyObject,
                             X11Output::deleteRubyObject,
                             new X11OutputPtr( ptr ) );
  return retVal;
}

