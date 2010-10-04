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

/// Class for measuring time.    
class Timer
{
public:
  /** Constructor starting timer.
      The constructor will call \c reset() to. */
  Timer(void) { reset(); }
  /** Restart timer.
      Reset the time elapsed to zero again. */
  void reset(void) {
    gettimeofday( &time, NULL );
  }
  /** Get elapsed time.
      The time elapsed (in seconds) is returned.
      @return Time elapsed in seconds since last call of \c timer or
      \c reset(). */
  double elapsed(void) const;
protected:
  /// Time of construction of this object.
  struct timeval time;
};
  
///
typedef boost::shared_ptr< Timer > TimerPtr;

#endif
