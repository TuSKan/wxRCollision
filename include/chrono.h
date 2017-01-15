/**
 * @file    chrono.h
 * @author  Thomas Lewiner <thomas.lewiner@polytechnique.org>
 * @version 0.2
 * @date    26/07/2004
 *
 * @brief   simple class to get detailed execution times
 *
 * simple class to get detailed execution times
 */
//________________________________________________


#ifndef _CHRONO_H_
#define _CHRONO_H_

#ifndef WIN32
#pragma interface
#endif // WIN32

//_____________________________________________________________________________

#include <map>
#include <stack>
#include <string>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <winbase.h>


//_____________________________________________________________________________
/// simple class to get detailed execution times
class Chrono
//-----------------------------------------------------------------------------
{
// Constructors
public :
  /// Constructor: start execution time
  Chrono( const std::string name )
  {
    QueryPerformanceCounter(&_dt) ;
    _clocks[_curr_clock.top()].QuadPart += _dt.QuadPart - _count.QuadPart ;
    _curr_clock.push(name) ;
    QueryPerformanceCounter(&_count) ;
  }

  /// Destructor: save the execution time
  ~Chrono()
  {
    QueryPerformanceCounter(&_dt) ;
    _clocks[_curr_clock.top()].QuadPart += _dt.QuadPart - _count.QuadPart ;
    _curr_clock.pop() ;
    QueryPerformanceCounter(&_count) ;
  }

//-----------------------------------------------------------------------------
// Operations
public :
  /// Reinitialize clock counting
  static bool ResetClocks() ;

  /// Print clocks to log
  static bool PrintClocks() ;

//-----------------------------------------------------------------------------
// Elements
protected :
  static std::stack<std::string>             _curr_clock ;
  static LARGE_INTEGER                       _count      ;
  static LARGE_INTEGER                       _dt         ;
  static std::map<std::string,LARGE_INTEGER> _clocks     ;
};
//_____________________________________________________________________________


//_____________________________________________________________________________

#define BEGIN_CLOCK    Chrono chrono_( __FUNCTION__ ) ;

//_____________________________________________________________________________
//_____________________________________________________________________________


#endif // _CHRONO_H_

