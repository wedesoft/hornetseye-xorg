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
#ifdef HAVE_CONFIG_H
#include "config.h"
#endif
#include <boost/shared_array.hpp>
#include <errno.h>
#ifndef NDEBUG
#include <iostream>
#endif
extern "C" {
#ifndef HAVE_LIBSWSCALE_INCDIR
  #include <ffmpeg/swscale.h>
#else
  #include <libswscale/swscale.h>
#endif
}
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
      boost::shared_array< unsigned char > array
        ( new unsigned char[ m_window->width() * m_window->height() * 4 ] );
      SwsContext *swsContext =
        sws_getContext( frame->width(), frame->height(), PIX_FMT_RGB24,
                        m_window->width(), m_window->height(), PIX_FMT_BGRA,
                        SWS_FAST_BILINEAR, 0, 0, 0 );
      uint8_t *sourceData[4];
      int sourceLineSize[4];
      sourceData[0] = (uint8_t *)frame->data();
      sourceLineSize[0] = 3 * frame->width();
      uint8_t *destData[4];
      int destLineSize[4];
      destData[0] = (uint8_t *)array.get();
      destLineSize[0] = 4 * m_window->width();
      sws_scale( swsContext, sourceData, sourceLineSize, 0,
                 frame->height(), destData, destLineSize );
      sws_freeContext( swsContext );
      XImage *xImage = XCreateImage( m_display->get(), m_visualInfo.visual,
                                     24, ZPixmap, 0, (char *)array.get(),
                                     m_window->width(), m_window->height(),
                                     32, m_window->width() * 4 );
      ERRORMACRO( xImage != NULL, Error, ,
                  "Failed to create X11-image" );
      xImage->byte_order = LSBFirst;
#ifndef NDEBUG
      cerr << "Window size is " << m_window->width() << 'x'
           << m_window->height() << endl;
#endif
      XPutImage( m_display->get(), m_window->get(), m_window->gc(),
                 xImage, 0, 0, 0, 0, m_window->width(), m_window->height() );
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

