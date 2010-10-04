/* HornetsEye - Computer Vision with Ruby
   Copyright (C) 2006, 2007   Jan Wedekind
   
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
#ifndef HORNETSEYE_XVIDEOIMAGEPAINTER_HH
#define HORNETSEYE_XVIDEOIMAGEPAINTER_HH

#include <X11/extensions/Xv.h>
#include <X11/extensions/Xvlib.h>
#include <set>
#include "x11painter.hh"
#if 0
class X11Window;
class XVideoOutput;

///
class XVideoImagePainter: public X11Painter
{
public:
  ///
  XVideoImagePainter(void);
  ///
  ~XVideoImagePainter(void);
  ///
  virtual void paint( bool x11Event ) throw (Error);
  ///
  virtual void unregisterWindow(void);
  ///
  virtual XVisualInfo *visualInfo( X11DisplayPtr display ) throw (Error);
  ///
  static VALUE cRubyClass;
  ///
  static VALUE registerRubyClass( VALUE module, VALUE cX11Output );
  ///
  static VALUE wrapNew( VALUE rbClass );
protected:
  ///
  Atom findAtom( const char *name ) throw (Error);
  ///
  int selectFormat( const int preferredUID ) throw (Error);
  ///
  FramePtr alignYV12( FramePtr frame );
  ///
  static int typecodeToUID( std::string typecode );
  ///
  static std::string uidToTypecode( int uid );
  ///
  XVisualInfo m_visualInfo;
  ///
  XvPortID m_port;
  ///
  bool m_requireColourKey;
  ///
  int m_colourKey;
  ///
  X11DisplayPtr m_display;
  ///
  XvImage *m_xvImage;
  ///
  static std::set< XvPortID > grabbedPorts;
};

typedef boost::shared_ptr< XVideoImagePainter > XVideoImagePainterPtr;

#endif
#endif
