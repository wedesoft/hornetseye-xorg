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
#ifndef HORNETSEYE_TIMER_HH
#define HORNETSEYE_TIMER_HH

#include <string>
#include <boost/shared_ptr.hpp>
#include <sys/time.h>

class Timer
{
public:
  Timer(void) { reset(); }
  void reset(void) {
    gettimeofday( &m_time, NULL );
  }
  double elapsed(void) const;
protected:
  struct timeval m_time;
};
  
typedef boost::shared_ptr< Timer > TimerPtr;

#endif
