/**
 * @file    HOctree.cpp
 * @author  Rener Pereira de Castro <rener@mat.puc-rio.br> 
 * @version 
 * @date    
 *
 * @brief
 *
 */
//________________________________________________

#ifndef WIN32
#pragma implementation
#endif // WIN32

#include "HOctree.h"
#include <iostream>
#include <cfloat>
#include <stack>
#include <algorithm>

#ifdef CLOCKS
#include "chrono.h"
#endif

//_____________________________________________________________________________
//
using std::cout ;
using std::endl ;
using std::vector ;
using std::stack ;
//_____________________________________________________________________________

//_____________________________________________________________________________
// Hash Func 
static Key hash_func = ((1<<21)-1) ;  //  (1<<x)-1   , x < 31. (Max level HOctree = 21) 64bits
//_____________________________________________________________________________


//_____________________________________________________________________________
// Aux find_neighbors 
struct KeyPosition 
{
  Key k ;
  int dx , dy , dz ;
};
//_____________________________________________________________________________

//_____________________________________________________________________________
template <>
const uint HashTable< Key,HOcnode>::convert( const Key& k ) const
//-----------------------------------------------------------------------------
{
  return (uint) ( k & hash_func );
}
//_____________________________________________________________________________


void HOctree::set_hash_func( cuint& hf )
//-----------------------------------------------------------------------------
{
  hash_func = ((1<<hf)-1);
}
//_____________________________________________________________________________


//_____________________________________________________________________________
// Class HOctree

int HOctree::inMe( const iterator& node , Key k) const 
//-----------------------------------------------------------------------------
{ 
  if( k==0 || !node.isValid() )
    return -1;  
  Key k_n(node.key());
  while( (k!=0) && (k>>3 != k_n) )
    k >>= 3;
  if( k==0 ) return -1;  
  return (int)(k & 7);
}
//_____________________________________________________________________________


#ifdef WIN32
extern "C"
#endif  // WIN32
int triBoxOverlap(float boxcenter[3],float boxhalfsize[3],float triverts[3][3]) ;
//-----------------------------------------------------------------------------
struct it_and_trigs {
  HOctree::iterator it;
  vector< int >*   trigs ;
}  ;
//-----------------------------------------------------------------------------
void HOctree::build( cBRep& surf , cuint& level )
//-----------------------------------------------------------------------------
{ 
  m_hist[0] = 0;
  // points are assumed in [0,1]^3
  uint ntrigs = surf.ntrigs;
  uint nverts = surf.nverts;
  Vertex* verts = surf.verts;
  int* trigs = surf.trigs;

  //
  int type;
  uint count[8];
  uint len ;

  stack< it_and_trigs > list;    
  it_and_trigs aux, aux_push ;

  vector<int>* vtrig = new vector<int> ;
  for( uint i = 0 ; i < ntrigs ; ++i )
    vtrig->push_back( i );

  m_tree.reserve( (uint)hash_func );
  aux.it = m_tree.insert( Key(1) , HOcnode() ) ;
  aux.trigs  = vtrig ;  

  list.push( aux );  

  while( !list.empty() )
  {
    aux  = list.top();  list.pop();    
    len = length( aux.it );
    if( m_level < len ) m_level = len;
    m_hist[0] += len;
    uint naux = aux.trigs->size() ;

    if( (len < level) && (naux >= 1) )
    {      
      (*aux.it).m_isleaf = false ;
      memset( count , 0 , 8*sizeof(int) );      
      vector<int> *  trigs_push[8] ;
      for( type = 0 ; type < 8 ; ++type )
        trigs_push[type] = new vector<int> ;

      Cube C[8], cit = cube( aux.it ) ;
      for( type = 0 ; type < 8 ; ++type )
      {        
        C[type].side     = cit.side / 2.0f ;
        C[type].center.x = cit.center.x + ((type&4)?1:-1) * C[type].side;
        C[type].center.y = cit.center.y + ((type&2)?1:-1) * C[type].side;
        C[type].center.z = cit.center.z + ((type&1)?1:-1) * C[type].side;
      }

      for( uint i = 0 ; i < naux ; ++i )
      {
        int j = aux.trigs->at(i) ;
        float triverts[3][3] ;
        cVertex &p0 = verts[ trigs[3*j+0] ] ;
        cVertex &p1 = verts[ trigs[3*j+1] ] ;
        cVertex &p2 = verts[ trigs[3*j+2] ] ;
        triverts[0][0] = p0.x ;
        triverts[0][1] = p0.y ;
        triverts[0][2] = p0.z ;
        triverts[1][0] = p1.x ;
        triverts[1][1] = p1.y ;
        triverts[1][2] = p1.z ;
        triverts[2][0] = p2.x ;
        triverts[2][1] = p2.y ;
        triverts[2][2] = p2.z ;        
        for( type = 0 ; type < 8 ; ++type )
        {
          float boxhsize[3]; 
          boxhsize[0] = (float) C[type].side + FLT_EPSILON;
          boxhsize[1] = (float) C[type].side + FLT_EPSILON;
          boxhsize[2] = (float) C[type].side + FLT_EPSILON;
          if( triBoxOverlap( C[type].center.p , boxhsize , triverts ) > 0 )
          {
            trigs_push[type]->push_back( j );
            ++count[type];
          }
        }
      }

      for( type = 0 ; type < 8 ; ++type )
      {
        //if( count[type] > 0 )
        {
          HOcnode hnode ;
          if( count[type] > 0 )
          {
            hnode.trigs().resize( trigs_push[type]->size() );
            copy(trigs_push[type]->begin(), trigs_push[type]->end(), hnode.trigs().begin() );
          }
          Key k(aux.it.key()); k <<= 3; k |= type ;
          aux_push.it = m_tree.insert(k , hnode ) ;
          aux_push.trigs = trigs_push[type] ;
          list.push( aux_push ) ;
        }
      }
      (*aux.it).trigs().clear();
    }

    aux.trigs->clear();
    delete aux.trigs ;
    aux.trigs = NULL ;
  }
  m_hist[0] /= m_tree.size() ;
  cout << " HOctree created from " << ntrigs << " triangles ";
  cout << " generate " << m_tree.size() << " nodes. " << endl;  
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//

//-----------------------------------------------------------------------------
struct it_and_keys 
{
  HOctree::iterator it;
  vector<Key>* k;
};
//-----------------------------------------------------------------------------

void HOctree::build( const vector<Key>& keys , cuint& level , cuint& npt_max )
//-----------------------------------------------------------------------------
{  
  int type;
  uint count[8];
  uint len ;
  std::stack< it_and_keys > list;
  
  it_and_keys aux, aux_push ;
  m_tree.reserve( (cuint)hash_func );
  aux.it = m_tree.insert( Key(1) , HOcnode(keys.size()) ) ;
  aux.k = const_cast< vector<Key>* > (&keys);
  list.push( aux );

  cout << " Building Octree ... " << endl ;

  while( !list.empty() )
  {
    aux  = list.top();  list.pop();    
    len = length( aux.it );
    if( m_level < len ) m_level = len;
    m_hist[0] += len;
    uint naux = aux.k->size() ;

    if( (len < level) && (naux > npt_max) )
    {
      (*aux.it).m_isleaf = false ;
      memset( count , 0 , 8*sizeof(int) );      
      vector<Key>*  k_push[8] ;
      for( uint i = 0 ; i < 8 ; ++i )
        k_push[i] = new vector<Key> ;
      
      for( uint i = 0 ; i < naux ; ++i )
      {
        if( (type=inMe( aux.it , aux.k->at(i) )) >= 0 )
        {
          k_push[type]->push_back( aux.k->at(i) );
          ++count[type];
        }
      }
      Key k( aux.it.key()<<3 );
      for( uint i = 0 ; i < 8 ; ++i )
      {
//        if( count[i] > 0 )
        {         
          aux_push.it = m_tree.insert( k|i , HOcnode(k_push[i]->size()) ) ;
          aux_push.k = k_push[i] ;
          list.push( aux_push ) ;
        }
      }
    }
    if( aux.k->size() < keys.size() )
    {
      aux.k->clear();
      delete aux.k;
      aux.k = NULL;
    }
  }
  m_hist[0] /= m_tree.size() ;
  cout << " HOctree created from " << keys.size() << " points ";
  cout << " generate " << m_tree.size() << " nodes. " << endl;  
}
//_____________________________________________________________________________



//_____________________________________________________________________________
//
void HOctree::optimize( cuint & type )
//-----------------------------------------------------------------------------
{    
#ifdef CLOCKS
  BEGIN_CLOCK 
#endif

  if( m_tree.size() == 0 ) return;

  cout << " Optimizing Octree ... " ;  
  memset( m_hist , 0 , 22*sizeof(uint) );  

  if( type == 0 )  // Search by Points
  {
    for( iterator it = m_tree.begin() , end = m_tree.end() ; it != end ; ++it ) 
    {      
      if( (*it).isleaf() ) 
      {        
        m_hist[ length(it) ] += (*it).npts();
      }
    }
  }
  else // Search by Leafs
  {
    for( iterator it = m_tree.begin() , end = m_tree.end() ; it != end ; ++it ) 
    {
      if( (*it).isleaf() ) ++m_hist[ length(it) ] ;      
    }
  }   
  
  int  left    = -1, right    = m_level + 1 ;
  uint leftsum =  0, rightsum =      0      ;
  while( left < right )
  {
    uint leftsum_old = leftsum ;
    if( leftsum <= rightsum )
    {
      ++left ;
      leftsum += m_hist[ left ] ;
    }
    if( leftsum_old >= rightsum )
    {
      --right ;
      rightsum += m_hist[ right ] ;
    }
  }  
  m_hist[0] = right ;

  cout << " OK !  Opt Type = " << type << " Opt Lev = " << m_hist[0] << endl;
}
//_____________________________________________________________________________


void HOctree::print_clocks()
//-----------------------------------------------------------------------------
{ 
  cout << endl; 
 #ifdef CLOCKS
  Chrono::PrintClocks();
  Chrono::ResetClocks();
 #endif
}
//_____________________________________________________________________________


HOctree::iterator HOctree::find_node( const Key& f , const uint& level) const
//-----------------------------------------------------------------------------
{  // Key f level  = level
#ifdef CLOCKS
  BEGIN_CLOCK 
#endif

  iterator it;
  Key k = f;
  if( !(it=m_tree.at( k )).isValid() )
  {
    k >>= 3;
    while( k!=0 && !(it=m_tree.at( k )).isValid() )  k >>= 3;
  }
  return it;
}
//_____________________________________________________________________________


HOctree::iterator HOctree::find_leaf( const Key& f ) const
//-----------------------------------------------------------------------------
{
#ifdef CLOCKS
  BEGIN_CLOCK 
#endif

  iterator it; 
  uint step = 3*(m_level-m_hist[0]) ;
  Key k = f >> step ;
  if( !(it=m_tree.at( k )).isValid() )
  {
    k >>= 3;
    while( k!=0 && !(it=m_tree.at( k )).isValid() )  k >>= 3;
  }
  else
  {
    int count(3);
    iterator valid = it;
    k = f >> ( step - count);
    while( k!=0 && (it=m_tree.at( k )).isValid() )
    {
      count +=3 ;
      valid = it;
      k = f >> ( step - count);
    }
    it = valid;
  }
  return it; 
}
//_____________________________________________________________________________


void HOctree::find_neighbors(const iterator& node,const uint& level,vector<iterator>& nlist) const
//-----------------------------------------------------------------------------
{   
#ifdef CLOCKS
  BEGIN_CLOCK 
#endif
  //------------------------------------------------------
  // Initialization  
  uint my_level = length( node ) ;
  bool biglevel(my_level <= m_hist[0]) ;    
  uint step(0);  
  vector<KeyPosition> neigh_keys ;
  KeyPosition kp;
  iterator neigh ;     
  //------------------------------------------------------
  // Generate Neigh Keys
  Key k = node.key() ;
  uint nbits_neigh = 3*my_level ;
  for( uint i = 0 ; i < 26 ; ++i ) 
  {
    kp.k = addition_locate_code( k , dir_neigh[i] ) ; 
    if( (kp.k >> nbits_neigh) != 1 ) continue ;
    kp.dx = dx[i] ; kp.dy = dy[i] ; kp.dz = dz[i] ;
    neigh_keys.push_back( kp );
  }  
  //------------------------------------------------------
  // Search Neigh
  for( vector<KeyPosition>::const_iterator k_neigh = neigh_keys.begin() , end = neigh_keys.end() ; k_neigh != end ; ++k_neigh )
  {
    kp = *k_neigh ;
    if( (neigh=m_tree.at( kp.k )).isValid() )
    {      
      if( (*neigh).isleaf() ) nlist.push_back( neigh );        
      else 
      {   
        uint id = (kp.dx+1)*9 + (kp.dy+1)*3 + (kp.dz+1);
        uint nkeys = 1 << ((kp.dx==0)+(kp.dy==0)+(kp.dz==0));
        stack <iterator> s; s.push( neigh );
        while( !s.empty() )
        {
          k = s.top().key() ; s.pop() ; k <<= 3 ;
          for( uint j = 0 ; j < nkeys ; ++j )
          {
            neigh = m_tree.at( k | l3b[id][j] );
            if( neigh.isValid() )
            {
              if( (*neigh).isleaf() ) nlist.push_back( neigh );
              else s.push( neigh );
            }
          }
        }
      }
    }        
    else
    {
      if( biglevel ) 
      {        
        kp.k >>= 3;
        while( kp.k!=0 && !(neigh=m_tree.at( kp.k )).isValid() )  kp.k >>= 3;
      }
      else 
      {
        step = 3*(my_level-m_hist[0]) ;
        k = kp.k >> step ;
        if( !(neigh=m_tree.at( k )).isValid() )
        {
          k >>= 3;
          while( k!=0 && !(neigh=m_tree.at( k )).isValid() )  k >>= 3;
        }
        else
        {
          uint count(3);
          iterator valid = neigh;
          k = kp.k >> ( step - count);
          while( k!=0 && (neigh=m_tree.at( k )).isValid() )
          {
            count +=3 ;
            valid = neigh;
            k = kp.k >> ( step - count);
          }
          neigh = valid ;
        }
      }
      if( (*neigh).isleaf() ) nlist.push_back( neigh );
    }
  }

  //std::sort( nlist.begin() , nlist.end() ) ;
  //nlist.erase( std::unique( nlist.begin() , nlist.end() ) , nlist.end() );
}
//_____________________________________________________________________________


void HOctree::find_inradius(const iterator& node,const float& radius,const uint& level,std::vector<iterator>& rlist) const
//-----------------------------------------------------------------------------
{
#ifdef CLOCKS
  BEGIN_CLOCK 
#endif   
    /*
  //------------------------------------------------------
  // Initializations
  uint my_level = length( node ) ;
  Cube C = cube(node,true); // side.x = side.y = side.z  
  //------------------------------------------------------
  // Eliminate Foo
  if( radius <= C.side.x )
  { // Find children in radius and add to rlist;
    if( !(*node).isleaf() ) 
    {
      Key k;
      iterator child;
      Cube ch;
      stack <iterator> s; s.push( node );
      while( !s.empty() )
      {
        k = s.top().key() ; s.pop() ; k <<= 3 ;
        for( uint i = 0 ; i < 8 ; ++i )
        {
          if( (*node).children(i) )
          {
            child = m_tree.at( k | i );
            if( child.isValid() )
            {
              if( (*child).isleaf() ) 
              {
                ch = cube(child);
                if( (C.center - (ch.center+ch.side)) < radius )
                  rlist.push_back(child);
              }
              else s.push( child );
            }
          }
        }
      }
    }
    return ;
  } // Adjacent  
  else if( radius <= ( C.side.x+(float)1.0f/(float)(1 << (m_level+1)) ) ) 
    return find_neighbors(node,level,rlist); 
  //------------------------------------------------------
  bool biglevel(my_level <= m_hist[0]) ;    
  uint step(0);  
  vector<KeyPosition> neigh_keys ;
  KeyPosition kp;
  iterator neigh ;     
  //------------------------------------------------------
  // Generate Neigh InRadius Keys
  Key k = node.key() ;
  uint nbits_neigh = 3*my_level ;
  for( uint i = 0 ; i < 26 ; ++i ) 
  {
    kp.k = addition_locate_code( k , dir_neigh[i] ) ; 
    if( (kp.k >> nbits_neigh) != 1 ) continue ;
    kp.dx = dx[i] ; kp.dy = dy[i] ; kp.dz = dz[i] ;
    neigh_keys.push_back( kp );
  }
  */
  //------------------------------------------------------
  // Old VErsion
  uint my_level = length( node ) ;
  Cube C = cube(node);
  if( radius <= C.side ) return ;
  else if( radius <= ( C.side+(float)1.0f/(1 << (m_level+1)) ) ) 
    return find_neighbors(node,level,rlist); // Adjacent    

  uint plevel = (uint) floor( log(radius) / log(0.5f) ) ;
  if( plevel > my_level ) plevel = my_level ;
  C = cube( m_tree.at( node.key() >> 3*(my_level-plevel) ));

  bool biglevel(plevel <= m_hist[0]) ;
  Key k(0) , c(0);  
  iterator neigh ;
  Point3D P ;  
  uint step(0);
  vector<Key> neigh_keys ;  

  neigh_keys.push_back( node.key() >> 3*(my_level-plevel) );

  float diam( 2.0f*C.side );
  for( uint i = 0 ; i < 26    ; ++i )
  {    
    P.p[0] = C.center.x + float(dx[i])*diam ;
    P.p[1] = C.center.y + float(dy[i])*diam ;
    P.p[2] = C.center.z + float(dz[i])*diam ;
    k = Point3D2Key( P , plevel ) ;
    if( k == 0 ) continue ;   
    neigh_keys.push_back( k );
  } 
  //------------------------------------------------------
  // Search Neigh  
  for( vector<Key>::const_iterator k_neigh = neigh_keys.begin() , end = neigh_keys.end() ; k_neigh != end ; ++k_neigh )
  {
    k = *k_neigh ;
    if( (neigh=m_tree.at( k )).isValid() )
    {      
      if( (*neigh).isleaf() ) rlist.push_back( neigh );        
      else 
      {         
        stack <iterator> s; s.push( neigh );
        while( !s.empty() )
        {
          k = s.top().key() ; s.pop() ; k <<= 3 ;
          for( uint j = 0 ; j < 8 ; ++j )
          {            
            neigh = m_tree.at( k | j );
            if( neigh.isValid() )
            {
              if( (*neigh).isleaf() ) rlist.push_back( neigh );
              else s.push( neigh );
            }
          }
        }
      }
    }        
    else
    {
      if( biglevel ) 
      {        
        k >>= 3;
        while( k!=0 && !(neigh=m_tree.at( k )).isValid() )  k >>= 3;
      }
      else 
      {
        step = 3*(my_level-m_hist[0]) ;
        c = k >> step ;
        if( !(neigh=m_tree.at( c )).isValid() )
        {
          c >>= 3;
          while( c!=0 && !(neigh=m_tree.at( c )).isValid() )  c >>= 3;
        }
        else
        {
          uint count(3);
          iterator valid = neigh;
          c = k >> ( step - count);
          while( c!=0 && (neigh=m_tree.at( c )).isValid() )
          {
            count +=3 ;
            valid = neigh;
            c = k >> ( step - count);
          }
          neigh = valid ;
        }
      }
      rlist.push_back( neigh );
    }
  }
  //std::sort( rlist.begin() , rlist.end() ) ;
  //rlist.erase( std::unique( rlist.begin() , rlist.end() ) , rlist.end() );
}
//_____________________________________________________________________________

//_____________________________________________________________________________
// Recursive Functions


//_____________________________________________________________________________
//
int HOctree::inMe_rec( const Cube& C , const Point3D& P ) const 
//-----------------------------------------------------------------------------
{   
  if( ( P.x <= C.center.x + C.side ) && ( P.x >= C.center.x - C.side ) &&
    ( P.y <= C.center.y + C.side ) && ( P.y >= C.center.y - C.side ) &&
    ( P.z <= C.center.z + C.side ) && ( P.z >= C.center.z - C.side ) )
  {
    bool bx = (P.x >= C.center.x);
    bool by = (P.y >= C.center.y);
    bool bz = (P.z >= C.center.z);
    return (bx<<2) + (by<<1) + (bz<<0);
  }
  return -1;
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//
HOctree::iterator HOctree::find_leaf_rec( const Point3D& P ) const
//-----------------------------------------------------------------------------
{   
#ifdef CLOCKS
  BEGIN_CLOCK 
#endif
  
  if( (m_tree.size() == 0) || P.x < 0.0f || P.x > 1.0f || P.y < 0.0f || P.y > 1.0f || P.z < 0.0f || P.z > 1.0f )
    return iterator::null();

  iterator node;
  stack<iterator> s;
  s.push( root() );

  while( !s.empty() )
  {
    node = s.top(); s.pop();
    if( inMe_rec( node , P ) >= 0 )
    {
      if( (*node).isleaf() ) return node ;
      else
      {
        Key f = node.key() << 3;
        for( uint i = 0 ; i < 8 ; ++i )
        {
          s.push( m_tree.at( f | i ) );
        }
      }
    }
  }

  return iterator::null();
}
//_____________________________________________________________________________



//_____________________________________________________________________________
//
HOctree::iterator HOctree::find_node_rec( const Point3D& P , cuint& level ) const
//-----------------------------------------------------------------------------
{   
#ifdef CLOCKS
  BEGIN_CLOCK 
#endif 

  if( (m_tree.size() == 0) || P.x < 0.0f || P.x > 1.0f || P.y < 0.0f || P.y > 1.0f || P.z < 0.0f || P.z > 1.0f )
    return iterator::null();

  uint lev = level;
  if( lev > m_level ) lev = m_level;

  iterator node;
  stack<iterator> s;
  s.push( begin() );

  while( !s.empty() )
  {
    node = s.top(); s.pop();
    if( inMe_rec( node , P ) >= 0 )
    {
      if( length(node) == lev ) return node ;
      else
      {
        Key f = node.key() << 3;
        for( uint i = 0 ; i < 8 ; ++i )
        {
          s.push( m_tree.at( f | i ) );
        }
      }
    }    
  }  

  return iterator::null();
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//
void HOctree::find_neighbors_rec( const iterator& node,std::vector<iterator>& nlist ) const
//-----------------------------------------------------------------------------
{  
#ifdef CLOCKS
  BEGIN_CLOCK 
#endif

  if( !node.isValid() ) return;

  float radius = (float)1.0f/(float)(1<<(m_level+1));

  Cube Cnode = cube( node );
  Cnode.side += radius;
  iterator neigh;
  stack<iterator> s;
  s.push( root() );
  Cube Cneigh;

  while( !s.empty() )
  {
    neigh = s.top(); s.pop();    
    Cneigh = cube( neigh );
    if( relation( Cnode , Cneigh ) > 0 )
    {
      if( (*neigh).isleaf() )
      {
        if (neigh != node) nlist.push_back( neigh ); 
      }
      else
      {
        Key f = neigh.key() << 3;
        for( uint i = 0 ; i < 8 ; ++i )
        {
          s.push( m_tree.at( f | i ) );
        }
      }
    }    
  }
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//
void HOctree::find_inradius_rec( const iterator& node , const float& radius , vector<iterator>& rlist) const
//-----------------------------------------------------------------------------
{

#ifdef CLOCKS
  BEGIN_CLOCK 
#endif

  if( !node.isValid() || (radius <= FLT_MIN) || (radius >= 1.0f ) ) return;
    
  Cube Cnode = cube( node );
  Cnode.side += radius;
  iterator neigh;
  stack<iterator> s;
  s.push( root() );
  Cube Cneigh;

  while( !s.empty() )
  {
    neigh = s.top(); s.pop();    
    Cneigh = cube( neigh );
    if( relation( Cnode , Cneigh ) > 0 )
    {
      if( (*neigh).isleaf() )
      {
        if (neigh != node) rlist.push_back( neigh ); 
      }
      else
      {
        Key f = neigh.key() << 3;
        for( uint i = 0 ; i < 8 ; ++i )
        {
          s.push( m_tree.at( f | i ) );
        }
      }
    }    
  }
}
//_____________________________________________________________________________



//_____________________________________________________________________________
//
void HOctree::leafs_list( vector<iterator>& llist ) const
//-----------------------------------------------------------------------------
{  
  iterator node;
  stack<iterator> s;
  s.push( root() );

  while( !s.empty() )
  {
    node = s.top(); s.pop();    
    if( (*node).isleaf() ) llist.push_back(node) ;
    else
    {
      Key f = node.key() << 3;
      for( uint i = 0 ; i < 8 ; ++i )
      {
        s.push( m_tree.at( f | i ) );
      }
    }
  }
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//_____________________________________________________________________________

