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
#include "x11output.hh"
#include "x11window.hh"

using namespace std;

VALUE X11Output::cRubyClass = Qnil;

X11Output::X11Output( X11PainterPtr painter ):
  m_painter(painter)
{
  m_painter->registerImageSource( this );
}

X11Output::~X11Output(void)
{
  m_painter->unregisterImageSource();
}

void X11Output::write( FramePtr frame ) throw (Error)
{
  m_frame = frame;
  X11Window *window = m_painter->window();
  if ( window ) window->repaint();
}

bool X11Output::status(void) const
{
  bool retVal;
  if ( m_painter->window() != NULL ) {
    if ( m_painter->window()->display()->quit() )
      retVal = false;
    else
      retVal = true;
  } else
    retVal = false;
  return retVal;
}

VALUE X11Output::registerRubyClass( VALUE module )
{
  cRubyClass = rb_define_class_under( module, "X11Output", rb_cObject );
  rb_define_method( cRubyClass, "status?",
                    RUBY_METHOD_FUNC( wrapStatus ), 0 );
  rb_define_method( cRubyClass, "write",
                    RUBY_METHOD_FUNC( wrapWrite ), 1 );
  return cRubyClass;
}

void X11Output::deleteRubyObject( void *ptr )
{
  delete (X11OutputPtr *)ptr;
}

void X11Output::markRubyObject( void *ptr )
{
  FramePtr frame( (*(X11OutputPtr *)ptr)->frame() );
  if ( frame ) frame->markRubyMember();
}

VALUE X11Output::wrapStatus( VALUE rbSelf )
{
  X11OutputPtr *self; Data_Get_Struct( rbSelf, X11OutputPtr, self );
  return (*self)->status() ? Qtrue : Qfalse;
}

VALUE X11Output::wrapWrite( VALUE rbSelf, VALUE rbFrame )
{
  try {
    X11OutputPtr *self; Data_Get_Struct( rbSelf, X11OutputPtr, self );
    FramePtr frame( new Frame( rbFrame ) );
    (*self)->write( frame );
  } catch ( std::exception &e ) {
    rb_raise( rb_eRuntimeError, "%s", e.what() );
  }
  return rbFrame;
}

