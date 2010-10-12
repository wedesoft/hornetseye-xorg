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
#define GLX_GLXEXT_PROTOTYPES
#include <GL/glu.h>
#include <GL/glx.h>
#ifndef NDEBUG
#include <iostream>
#endif
#include "openglimagepainter.hh"
#include "rubytools.hh"
#include "x11window.hh"
#include "x11output.hh"

using namespace boost;
using namespace std;

VALUE OpenGLImagePainter::cRubyClass = Qnil;

OpenGLImagePainter::~OpenGLImagePainter(void)
{
  // Seems to be deallocated by X11 or GLX.
  // if ( m_visualInfo != NULL )
  //   XFree( m_visualInfo );
}

void OpenGLImagePainter::paint( bool ) throw (Error)
{
#ifndef NDEBUG
  cerr << "OpenGLImagePainter::paint()" << endl;
#endif

  assert( m_window != NULL );

  if ( m_imageSource != NULL ) {

#ifndef NDEBUG
    cerr << "Requesting image." << endl;
#endif
    FramePtr frame = m_imageSource->frame();

    if ( frame ) {
      // Create OpenGL rendering context for on-screen rendering.
      // Direct rendering is enabled (GL_TRUE). Try GL_FALSE if there
      // are problems with your graphics card and/or driver.
      // It's also possible to disable direct rendering using an
      // environment variable "export LIBGL_ALWAYS_INDIRECT=1".
      GLXContext context =
        glXCreateContext( m_window->display()->get(),
                          m_window->visualInfo(), 0, GL_TRUE );
      // Check success.
      glWrap( context != NULL, "Error creating GLX-context" );
      
      // Attach context to the window.
      glWrap( glXMakeCurrent( m_window->display()->get(),
                              m_window->get(), context ),
              "Error switching GLX-context" );
      
      // Initialise coordinate system.
      glLoadIdentity();
      glViewport( 0, 0, m_window->width(), m_window->height() );
      glOrtho( 0, m_window->width(), m_window->height(), 0, -1.0, 1.0 );
      
      // Set up parameters for image transfer.
      glDisable( GL_DITHER );
      glPixelStorei( GL_UNPACK_ALIGNMENT, 1 );
      glRasterPos2i( 0, 0 );
      glPixelZoom(  (float)m_window->width()  / frame->width() ,
                   -(float)m_window->height() / frame->height() );
      
      // Wait for vertical retrace
      // http://www.inb.uni-luebeck.de/~boehme/xvideo_sync.html
      // The code is commented out for the moment, because it does
      // not compile on some platforms.
      // http://rubyforge.org/tracker/index.php?func=detail&aid=11164&group_id=2714&atid=10403
      
      // unsigned int retraceCount;
      // glXGetVideoSyncSGI( &retraceCount );
      // glXWaitVideoSyncSGI( 2, ( retraceCount + 1 ) % 2, &retraceCount );
      
#ifndef NDEBUG
      cerr << "Drawing " << frame->width() << 'x' << frame->height()
           << " image." << endl;
#endif
      if ( frame->rgb() ) {
        glDrawPixels( frame->width(), frame->height(),
                      GL_RGB, GL_UNSIGNED_BYTE, frame->data() );
      } else {
        glDrawPixels( frame->width(), frame->height(),
                      GL_LUMINANCE, GL_UNSIGNED_BYTE, frame->data() );
      };
      glEnable(GL_DITHER);
      
      // Finalise.
      glFinish();
      glXDestroyContext( m_window->display()->get(), context );
    };

  };
}


void OpenGLImagePainter::unregisterWindow(void)
{
  XFree( m_visualInfo );
  X11Painter::unregisterWindow();
}


XVisualInfo *OpenGLImagePainter::visualInfo( X11DisplayPtr display )
  throw (Error)
{
  assert( m_visualInfo == NULL );
  int attributes[] =
    { GLX_RGBA, GLX_RED_SIZE, 1, GLX_GREEN_SIZE, 1, GLX_BLUE_SIZE, 1,
      GLX_DEPTH_SIZE, 0, None };
  m_visualInfo =
    glXChooseVisual( display->get(), DefaultScreen( display->get() ),
                     attributes );
  ERRORMACRO( m_visualInfo != NULL, Error, ,
              "Error choosing desired X11 visual." );
  m_display = display;
  return m_visualInfo;
}

void OpenGLImagePainter::glWrap( bool condition, const char *message )
  throw (Error)
{
  if ( !condition ) {
    Error e;
    e << message;
    bool first = true;
    GLenum error;
    while ( ( error = glGetError() ) != GL_NO_ERROR ) {
      e << ( first ? ": " : "; " ) << gluErrorString( error );
      first = false;
    };
    throw e;
  };
}

VALUE OpenGLImagePainter::registerRubyClass( VALUE module,
                                             VALUE cX11Output )
{
  cRubyClass = rb_define_class_under( module, "OpenGLOutput", cX11Output );
  rb_define_singleton_method( cRubyClass, "new",
                              RUBY_METHOD_FUNC( wrapNew ), 0 );
  return cRubyClass;
}

VALUE OpenGLImagePainter::wrapNew( VALUE rbClass )
{
  VALUE retVal = Qnil;
  X11PainterPtr painter( new OpenGLImagePainter );
  X11OutputPtr ptr( new X11Output( painter ) );
  retVal = Data_Wrap_Struct( rbClass,
                             X11Output::markRubyObject,
                             X11Output::deleteRubyObject,
                             new X11OutputPtr( ptr ) );
  return retVal;
}


