/**
 * @file    PartSys.h
 * @author  Rener Pereira de Castro <rener@mat.puc-rio.br> 
 * @version 
 * @date    
 *
 * @brief
 *
 */
//________________________________________________


#ifndef _PARTSYS_H_
#define _PARTSYS_H_

#ifndef WIN32
#pragma interface
#endif // WIN32

#include "Utils.h"
#include "HOctree.h"
#include <vector>

using std::vector ;

//_____________________________________________________________________________
//Class PartSys

class PartSys
//-----------------------------------------------------------------------------
{
public:
  //______________________________________________________
  //Class Particle
  struct Particle
  {
    Point3D posi;
    Point3D velo;    
  };

  //______________________________________________________
public:
	// Constructor & Destructor  
  PartSys();    
  ~PartSys();
  //------------------------------------------------------
  //Members
public:
  void CreateOctree(cuint& level=10);

  inline void ClearOctree() { m_tree.clear(); }

  void CreateSurfFromPly( const char* plyname );

  void CreateParticles(const real& cx , const real& cy , const real& cz ,
                       const int& nx    , const int& ny    , const int& nz    ,
                       const real& mass ) ;  

  inline void ClearParticles() { delete [ ] m_particles; m_particles = NULL; m_npart = 0;}  
  //------------------------------------------------------
  // Collision Functions 
protected:
/**
 	 * \brief A recursive collision function 		
	 * \param Dt is the Step Size
	 * \param OldP is the old position (before collision)
	 * \param NewP is the new position (after  collision)
	 * \param NewV is the new velocity (after  collision)
	 * \param radius is the radius of the sphere that is colliding
	 * \param k as Collision is a recursive function, k is its depth. It should start as 0.
	 */		
  int  Collision ( real dt,Particle& Pnew , Particle& Pold , const vector<int>& trigs );

  bool  Process ( real dt,Particle& Pnew , Particle& Pold );

	// Collision. It is a simplified version of what appear in paper: 
	// Intersection Test for Collision Detection in Particle Systems
	// by  Evaggelia-Aggeliki Karabassi and Georgios Papaioannou and Theoharis Theoharis
	/**
	 * \brief Test if a point is inside of a sphere. 
	 * \param id_vertex id of the point to be tested
	 * \param Center center of sphere
	 * \param radius sphere radius
	 */
	bool VertexInsideSphere(int id_vertex, const Point3D& P);
	/**
 	 * \brief Test if a point (center of sphere) is inside a triangle
	 * \param id_face id of the triangle face
	 * \param Center center of sphere
	 */		
	bool CenterInsideTriangle(int id_face, const Point3D& P);
	/**
	 * \brief Test if a segment is inside a sphere
	 * \param id_vertex1 id of one   segment's vertice 
	 * \param id_vertex2 id of other segment's vertice 	
	 * \param Center center of sphere
	 * \param radius sphere radius
	 */	
	bool SegmentInsideSphere(int id_vertex1, int id_vertex2, const Point3D& P);
	/**
	 * \brief Test if a sphere intersect a triangle
	 * \param id_face id of the triangle (face)			
	 * \param CenterProj center of the sphere projected at triangle
	 * \param radius sphere radius
	 */			
	bool SphereIntersectTriangle(int corner, const Point3D& P);

public:
  void Solver(real dt);  
  void flip_faces() ;
  //------------------------------------------------------
  //Set and Get Data
public:
  BRep  Surf() { return m_surf; }
  const BRep Surf() const { return m_surf; }
  const HOctree& Octree() const { return m_tree; }
  const Particle * Particles () const { return m_particles; }
  uint  npart() const { return m_npart; }

  const real KF() const { return m_kf; }
  void KF( const real& kf ) { m_kf = kf; }

  const real KR() const { return m_kr; }
  void KR( const real& kr ) { m_kr = kr; }

  real Mass() const { return m_mass ; }
  void Mass(const real& m) { m_mass = m; }
  
  //------------------------------------------------------
  //Elements
private:  
  BRep m_surf;
  HOctree m_tree;
  Particle * m_particles ;
  uint m_npart;
  
  uint m_level;
  uint m_levelx3;
  Key  m_mask;
  real m_step;

  Point3D* m_faces_normals ;
  int      m_nfaces ;
  
  real m_kf , m_kr ;  //  Coeficintes de fricção e restituição entre 0 e 1 ;  
  real  m_mass;
  Point3D m_accel;
	//-----------------------------------------------------------------------------  
};
//_____________________________________________________________________________

//_____________________________________________________________________________
#endif  //  _PARTSYS_H_