/**
 * @file    chrono.cpp
 * @author  Thomas Lewiner <thomas.lewiner@polytechnique.org>
 * @version 0.2
 * @date    26/07/2004
 *
 * @brief   wxT main frame
 *
 * Class definition for wxT application and main frame
 */
//________________________________________________


#ifndef WIN32
#pragma implementation
#endif // WIN32


#include <iostream>
#include "chrono.h"

std::stack<std::string>             Chrono::_curr_clock ;
LARGE_INTEGER                       Chrono::_count      ;
LARGE_INTEGER                       Chrono::_dt         ;
std::map<std::string,LARGE_INTEGER> Chrono::_clocks     ;


//_____________________________________________________________________________
// Reinitialize clock counting
bool Chrono::ResetClocks()
//-----------------------------------------------------------------------------
{
  _clocks.clear() ;
  while( !_curr_clock.empty() ) _curr_clock.pop() ;
  _curr_clock.push("idle time") ;
  QueryPerformanceCounter(&_count) ;

  return true ;
}
//_____________________________________________________________________________


//_____________________________________________________________________________
// print the execution time of the traced functions
bool Chrono::PrintClocks()
//-----------------------------------------------------------------------------
{
  LARGE_INTEGER freq ;
	QueryPerformanceFrequency(&freq);

  float prop = 0.0 ;
  float secs = 1.0f / (float)freq.QuadPart ;
  std::map<std::string,LARGE_INTEGER>::const_iterator it, end = _clocks.end() ;
  for( it = _clocks.begin() ; it != end ; ++it ) prop += it->second.QuadPart ;

  std::cout << " time: " << prop * secs << " sec (+ idle " << _clocks["idle time"].QuadPart * secs << " sec)" << std::endl ;

  prop -= _clocks["idle time"].QuadPart ;
  if( prop == 0.0 ) return false;
  prop = 100.0f / prop ;
  for( it = _clocks.begin() ; it != end ; ++it )
    std::cout << "\t" << it->first.c_str() << "\t" << it->second.QuadPart * prop << "%\t" << it->second.QuadPart * secs << " sec" << std::endl ;

  if( _curr_clock.size() > 1 )
  {
    std::cout << "Clock error? stack size = " << _curr_clock.size() << std::endl ;
    while( !_curr_clock.empty() ) { std::cout << _curr_clock.top().c_str() << " " ; _curr_clock.pop() ; }
    return false ;
  }

  return true ;
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//_____________________________________________________________________________
