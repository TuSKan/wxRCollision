/**
 * @file    PartSys.cpp
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


#include "PartSys.h"
#include <cfloat>
#include <cmath>
#include <set>
#include <stack>
#include <vector>

using std::vector ;
using std::set ;
using std::stack ;

//_____________________________________________________________________________

//_____________________________________________________________________________

PartSys::PartSys() : m_kf(1.0) , m_kr(1.0) , m_faces_normals(NULL) , m_mass(0.0) , m_particles(NULL) ,
m_npart(0) , m_level(0) , m_levelx3(0) , m_mask(0) , m_step(0.0f) , m_surf(BRepNULL)
//-----------------------------------------------------------------------------
{  
  m_accel.x = 0.0 ;
  m_accel.y =-4.0 ;
  m_accel.z = 0.0 ;  
}
//_____________________________________________________________________________


PartSys::~PartSys()
//-----------------------------------------------------------------------------
{ 
  delete [] m_particles;  
  delete [] m_faces_normals ;
  delete [] m_surf.verts;
  delete [] m_surf.trigs;
}
//_____________________________________________________________________________


void PartSys::CreateOctree(cuint& level)
//-----------------------------------------------------------------------------
{
  if( m_tree.size() > 0 || m_surf.nverts == 0 ) return ;  
  m_tree.build( m_surf , level );  
  m_tree.optimize(1);
  m_level = m_tree.level();
  m_levelx3 = 3*m_level;
  m_mask  = ((Key)1 << (3*m_level+1)) -1 ;
  m_step = 1.0f / (float)((uint)1<<m_level);
}
//_____________________________________________________________________________


void PartSys::CreateSurfFromPly( const char* plyname )
//-----------------------------------------------------------------------------
{  
  if( !OpenPLY( plyname , m_surf ) )  { m_surf = BRepNULL; return ; }
  m_nfaces = m_surf.ntrigs ;
  m_faces_normals = new Point3D[ m_nfaces ];   

  for( int i = 0 ; i < m_nfaces ; ++i )
  { 
    cPoint3D &V0 = asPoint3D(m_surf.verts[ m_surf.trigs[3*i+0] ].p);
    cPoint3D &V1 = asPoint3D(m_surf.verts[ m_surf.trigs[3*i+1] ].p);
    cPoint3D &V2 = asPoint3D(m_surf.verts[ m_surf.trigs[3*i+2] ].p);
    m_faces_normals[i] = (V1-V0)^(V2-V0) ;
    real nrm = norm( m_faces_normals[i] ) ;
    if( nrm > FLT_EPSILON ) m_faces_normals[i] = (1.0f/nrm)*m_faces_normals[i] ;
  }  
 
  Point3D N = asPoint3D(m_surf.verts[0].n);  
  if( N*m_faces_normals[0] < 0 ) flip_faces();
}
//_____________________________________________________________________________


void PartSys::CreateParticles(const real& cx , const real& cy , const real& cz ,
                              const int& nx    , const int& ny    , const int& nz    ,
                              const real& mass )
//-----------------------------------------------------------------------------
{ 
  m_mass = mass ;
  real EPS = mass*1e-05f ;
  real radius = 2.0f*(m_mass+EPS) ;
  Point3D min ;

  min.x = cx - ((real)nx*0.5f)*radius ;
  min.y = cy - ((real)ny*0.5f)*radius ;
  min.z = cz - ((real)nz*0.5f)*radius ;

  Particle P;
  P.velo.x = P.velo.y = P.velo.z = 0.0 ;

  m_npart = nx*ny*nz;
  m_particles = new Particle[nx*ny*nz];
  for( int k = 0 ; k < nz ; ++k )
  for( int j = 0 ; j < ny ; ++j )
  for( int i = 0 ; i < nx ; ++i )
  {
    P.posi.x = min.x + i*radius + ( (rand()%2==0)?-1.0f:1.0f)*1e-05f;
    P.posi.y = min.y + j*radius + ( (rand()%2==0)?-1.0f:1.0f)*1e-05f;
    P.posi.z = min.z + k*radius + ( (rand()%2==0)?-1.0f:1.0f)*1e-05f;
    m_particles[ i + j*nx + k*nx*ny ] = P;
  }  
}
//_____________________________________________________________________________


void PartSys::Solver( real dt )
//-----------------------------------------------------------------------------
{
  static const uint MAXINTER = 10000;
  if( m_tree.size() > 0  && m_surf.nverts > 0 ) 
  {    
    Particle Pold , Poldin;
    for( uint i = 0 ; i < m_npart ; ++i )
    {// Euler Method
      uint iter(0);
      Pold = Poldin = m_particles[i];
      Particle& Pnew = m_particles[i] ;
      Pnew.velo += m_accel*dt ;
      Pnew.posi += Pnew.velo*dt ;
      while( iter < MAXINTER && Process( dt , Pnew , Pold ) ) ++iter;
      if( iter == MAXINTER ) { Pnew = Pold = Poldin ; }
    }
  }
}
//_____________________________________________________________________________


void PartSys::flip_faces()
//-----------------------------------------------------------------------------
{
  for( int i = 0 ; i < m_nfaces ; ++i )
  {
    m_faces_normals[i].x *= -1 ;
    m_faces_normals[i].y *= -1 ;
    m_faces_normals[i].z *= -1 ;
  }
}
//_____________________________________________________________________________


//_____________________________________________________________________________
// Collision Function


bool PartSys::VertexInsideSphere(int id_vertex, const Point3D& P)
//-----------------------------------------------------------------------------
{ 
  Point3D V = asPoint3D(m_surf.verts[ id_vertex ].p);  
  return ( ((V-P)*(V-P)) < (m_mass*m_mass) );
}
//_____________________________________________________________________________


bool PartSys::CenterInsideTriangle(int corner, const Point3D& P)
//-----------------------------------------------------------------------------
{   
  cPoint3D& V0 = asPoint3D(m_surf.verts[ m_surf.trigs[3*corner+0] ].p);
  cPoint3D& V1 = asPoint3D(m_surf.verts[ m_surf.trigs[3*corner+1] ].p);
  cPoint3D& V2 = asPoint3D(m_surf.verts[ m_surf.trigs[3*corner+2] ].p);

  cPoint3D u = V1 - V0;
  cPoint3D v = V2 - V0;
  cPoint3D w = P - V0;

  real aux_uv = u*v;
	real aux_uu = u*u;
	real aux_vv = v*v;
	real aux_wv = w*v;
	real aux_wu = w*u;
	
	real D = (aux_uv)*(aux_uv) -(aux_uu)*(aux_vv);

  if( fabs( D ) < FLT_EPSILON ) return false ;

	real s = (  (aux_uv)*(aux_wv)-(aux_vv)*(aux_wu)  ) / ( D );
	real t = (  (aux_uv)*(aux_wu)-(aux_uu)*(aux_wv)  ) / ( D );	
	
	if( (fabs(s) < FLT_EPSILON)  || (fabs(t) < FLT_EPSILON)  ||  (fabs(s+t-1) < FLT_EPSILON) )
		return true;  // Point at segment
		
	if( (s > 0)  && (t > 0)  &&  (s+t < 1) )
		return true;  // Point  inside
	
	return false;
}
//_____________________________________________________________________________


bool PartSys::SegmentInsideSphere(int id_vertex1, int id_vertex2, const Point3D& P)
//-----------------------------------------------------------------------------
{   
  cPoint3D &V1 = asPoint3D(m_surf.verts[ id_vertex1 ].p);
  cPoint3D &V2 = asPoint3D(m_surf.verts[ id_vertex2 ].p);

  cPoint3D N = V2 - V1 ;

  creal t = ( N*P - N*V1 ) / (N*N);

  cPoint3D Q = V1 + t*N;
  cPoint3D Dist = Q - P;
	creal d2 = Dist*Dist;

  if ( d2 >= m_mass*m_mass ) return false;
	else if ( (t >= 0)  && (t <= 1) ) return true;
	else if ( (t < 0) && VertexInsideSphere( id_vertex1, P ) ) 	return true;
	else if ( (t > 1) && VertexInsideSphere( id_vertex2, P ) ) 	return true;	 
	
  return false;
}
//_____________________________________________________________________________


bool PartSys::SphereIntersectTriangle(int corner, const Point3D& proj)
//-----------------------------------------------------------------------------
{ 
  int verts0 = m_surf.trigs[3*corner+0];
  int verts1 = m_surf.trigs[3*corner+1];
  int verts2 = m_surf.trigs[3*corner+2];
	
	if( VertexInsideSphere(verts0 , proj) ) return true;
  if( VertexInsideSphere(verts1 , proj) ) return true;
  if( VertexInsideSphere(verts2 , proj) ) return true;
		
	if( CenterInsideTriangle(corner, proj) ) return true;
		
	if( SegmentInsideSphere(verts0,verts1, proj ) )  return true;
	if( SegmentInsideSphere(verts0,verts2, proj ) )  return true;
	if( SegmentInsideSphere(verts1,verts2, proj ) )  return true;
	
	return false;
}
//_____________________________________________________________________________


bool PartSys::Process ( real dt, Particle& Pnew , Particle& Pold )
//-----------------------------------------------------------------------------
{
  Key PoldI(0) , PoldJ(0) , PoldK(0) ;
  XYZ2Index( Pold.posi.x , Pold.posi.y , Pold.posi.z , m_level , PoldI , PoldJ , PoldK );  
  if( (PoldI == 0) && (PoldJ == 0) && (PoldK == 0) ) return false;
  
  Key key_next(Index2Key(PoldI,PoldJ,PoldK,m_level));
  HOctree::iterator node = m_tree.at(key_next);
  
  if( node.isValid() && Collision( dt , Pnew , Pold , (*node).trigs() ) ) return true;   

  Key PnewI = ( ( Pnew.posi.x > 0.0f ) ? 1 : -1 ) * (Key)floor( abs(Pnew.posi.x) / m_step ) ;
  Key PnewJ = ( ( Pnew.posi.y > 0.0f ) ? 1 : -1 ) * (Key)floor( abs(Pnew.posi.y) / m_step ) ;
  Key PnewK = ( ( Pnew.posi.z > 0.0f ) ? 1 : -1 ) * (Key)floor( abs(Pnew.posi.z) / m_step ) ;  

  if( (PoldI == PnewI) && (PoldJ == PnewJ) && (PoldK == PnewK) ) return false;
  //////////////////////////////////////////////////////////////////////////  
  // Direction (orthogonal)
  int pi((int)PoldI) , pj((int)PoldJ) , pk((int)PoldK) ;
  int qi((int)PnewI) , qj((int)PnewJ) , qk((int)PnewK) ;

  // Bresenham's line from P to Q
  // starting point of line
  int x = pi, y = pj, z = pk;
  // direction of line
  int dx = qi-x, dy = qj-y, dz = qk-z;
  // increment or decrement depending on direction of line
  int sx = (dx > 0 ? 1 : (dx < 0 ? -1 : 0));
  int sy = (dy > 0 ? 1 : (dy < 0 ? -1 : 0));
  int sz = (dz > 0 ? 1 : (dz < 0 ? -1 : 0));
  // decision parameters for voxel selection
  dx = abs(dx); dy = abs(dy); dz = abs(dz);
  int ax = 2*dx, ay = 2*dy, az = 2*dz;
  int decx, decy, decz;
  // determine largest direction component, single-step related variable
  int max = dx, var = 0;
  if ( dy > max ) { max = dy; var = 1; }
  if ( dz > max ) { max = dz; var = 2; }
  // Define directions
  uint dnx = ( (sx == 1) ? 18 : ( (sx == -1) ? 0  : 9));
  uint dny = ( (sy == 1) ? 6  : ( (sy == -1) ? 0  : 3));
  uint dnz = ( (sz == 1) ? 2  : ( (sz == -1) ? 0  : 1)); 

  // traverse Bresenham line  
  switch ( var ) 
  {
  case 0:  // single-step in x-direction
    for (decy=ay-dx, decz=az-dx; ; /*x += sx, decy += ay, decz += az*/) 
    {
      if ( x == (int)qi ) break;
      //take Bresenham step
      //uint d = dnx;      
      //if ( decy >= 0 ) { decy -= ax; y += sy; d += dny; } else d += 3;
      //if ( decz >= 0 ) { decz -= ax; z += sz; d += dnz; } else d += 1;
      ///*  
      uint d = 0 ;
      if ( decy >= 0 )
      {
        if ( decz >= decy )
        { decz -= ax; z += sz; d = dnz + 12 ; }
        else
        { decy -= ax; y += sy; d = dny + 10 ; }
      }
      else if ( decz >= 0 )
      {
        decz -= ax; z += sz; d = dnz + 12 ;
      }
      else
      {
        x += sx ;  decy += ay ;  decz += az ;  d = dnx + 4 ;
      }
      //*/
      Key temp = addition_locate_code( key_next , dir_neigh [d] & m_mask ) & m_mask;
      if( ( temp >> (m_levelx3) ) != 1 ) return false;
      key_next = temp;
      node = m_tree.at( key_next );      
      if( node.isValid() && Collision( dt , Pnew , Pold , (*node).trigs() ) ) return true;
    }
  break;
  case 1:  // single-step in y-direction
    for (decx=ax-dy, decz=az-dy; ; /*y += sy, decx += ax, decz += az*/) 
    { 
      if ( y == (int)qj ) break;
      //take Bresenham step
      
      //uint d = dny;
      //if ( decx >= 0 ) { decx -= ay; x += sx; d += dnx; } else d += 9;
      //if ( decz >= 0 ) { decz -= ay; z += sz; d += dnz; } else d += 1;
      ///*
      uint d = 0 ;
      if ( decx >= 0 )
      {
        if ( decz >= decx )
        { decz -= ay; z += sz; d += dnz + 12 ; }
        else
        { decx -= ay; x += sx; d += dnx + 4 ; }
      }
      else if ( decz >= 0 )
      {
        decz -= ay; z += sz; d += dnz + 12 ;
      }
      else
      {
        y += sy ;  decx += ax ;  decz += az ;  d = dny + 10 ;
      }
      //*/
      Key temp = addition_locate_code( key_next , dir_neigh [d] & m_mask ) & m_mask;
      if( ( temp >> (m_levelx3) ) != 1 ) return false;
      key_next = temp;
      node = m_tree.at( key_next );      
      if( node.isValid() && Collision( dt , Pnew , Pold , (*node).trigs() ) ) return true;
    }    
  break;
  case 2:  // single-step in z-direction
    for (decx=ax-dz, decy=ay-dz; ; /*z += sz, decx += ax, decy += ay*/) 
    { 
      if ( z == (int)qk ) break;
      //take Bresenham step
      //uint d = dnz;      
      //if ( decx >= 0 ) { decx -= az; x += sx; d += dnx; } else d += 9;
      //if ( decy >= 0 ) { decy -= az; y += sy; d += dny; } else d += 3;
      ///*
      uint d = 0 ;
      if ( decx >= 0 )
      {
        if ( decy >= decx )
        { decy -= az; y += sy; d = dny + 10 ; }
        else
        { decx -= az; x += sx; d = dnx +  4 ; }
      }
      else if ( decy >= 0 )
      {
        decy -= az; y += sy; d = dny + 10 ;
      }
      else
      {
        z += sz ;  decx += ax ;  decy += ay ;  d = dnz + 12 ;
      }
      //*/
      Key temp = addition_locate_code( key_next , dir_neigh [d] & m_mask ) & m_mask;
      if( ( temp >> (m_levelx3) ) != 1 ) return false;
      key_next = temp;
      node = m_tree.at( key_next );      
      if( node.isValid() && Collision( dt , Pnew , Pold , (*node).trigs() ) ) return true;
    }
  break;
  }
  return false;
}
//_____________________________________________________________________________



//_____________________________________________________________________________
//
int  PartSys::Collision ( real dt,Particle& Pnew , Particle& Pold , const vector<int>& trigs )
//-----------------------------------------------------------------------------
{ 
  cint nfaces = trigs.size() ;
  if( nfaces <= 0 ) return 0;
  // Variables for Intersect Triangle
  int face_intersect(-1) ;
  bool has_intersect(false) ;
  real time_intersect(FLT_MAX) ;

  // Variables for distance and time for contact with planes  
	real* distance  = new real[nfaces];
	real* time      = new real[nfaces];
	bool* intersect = new bool[nfaces];

  Point3D Proj , Fwd ;
  real side, dVProj , dist , timet;  

  for( int f = 0 ; f < nfaces ; ++f )
  {    
    cPoint3D& V = asPoint3D(m_surf.verts[m_surf.trigs[3*trigs[f]]].p);
    cPoint3D& N = m_faces_normals[ trigs[f] ];

    // First: discard planes with no chance
    dVProj = Pnew.velo * N ;

    side = ( Pold.posi - V ) * N;
		if( side > 0 ) 
			distance[f] =   side - m_mass ;  //Positive distance
		else
			distance[f] =  -side - m_mass ;  //Positive distance

		// Is it getting close
		if( dVProj < 0 )
    {	
			time[f] = -distance[f]/dVProj ;
      //Not time enough to touch
      intersect[f] = ( time[f] <= dt )?( true ):( false );; 
		}
    else
    {
      intersect[f] = false ;
      time[f] = FLT_MAX ;
    }    

    if( intersect[f] ) // Here I know time <= RestDt
    { 
      dist = ( Pold.posi - V ) * N ; // side
			timet =  -dist/dVProj;
      Fwd = Pold.posi + Pnew.velo * timet;  // Put Sphere with the centre at triangle
      Proj = Fwd - ( (Fwd-V) * N ) * N; // Projection in Plane
			// Now I test intersection sphere/triangle
			if (!SphereIntersectTriangle( trigs[f] , Proj ) ) 
        intersect[f] = false;	// Will not touch			
    }

		// Here I choose the near intersect plane, could be more then one
		if( intersect[f] && (time_intersect > time[f]) )
    {
			time_intersect = time[f];
			face_intersect = trigs[f];
			has_intersect = true;			
		}
  }

  delete [] distance;
	delete [] time;
	delete [] intersect;  

  // Now, if intersect, I know the near plane intersection
	if( has_intersect )
  {
		// Avoid intersection with the triangle
		if( fabs(time_intersect) < 1e03f ) time_intersect = 0.0f;

    Pold.posi += ( Pnew.velo  * time_intersect ); 

		cPoint3D& N = m_faces_normals[ face_intersect ];
		Point3D& Vn = (Pnew.velo*N) * N ;
		Point3D& Vt = Pnew.velo-Vn ;
		
		Vt = Vt * m_kf;
		Vn = Vn * m_kr;

		Pnew.velo = Vt - Vn;

		dt -= time_intersect;

    Pnew.posi = Pold.posi + Pnew.velo * dt;

    return 1;
	}

  return 0;
}
//_____________________________________________________________________________



//_____________________________________________________________________________
//_____________________________________________________________________________
