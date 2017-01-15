/**
 * @file    wxRCollisionGL.cpp
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

#include <vector>
#include <stack>
#include "wxRCollisionGLCanvas.h"
#include "wxRCollisionScene.h"
#include "wxRCollisionCtrls.h"
//#include "wx/msw/winundef.h"
//_____________________________________________________________________________

BEGIN_EVENT_TABLE(wxRCollisionGLCanvas, wxRlibGLCanvas)		
EVT_IDLE(wxRCollisionGLCanvas::OnIdle)  
EVT_SET_FOCUS(wxRCollisionGLCanvas::OnFocus)  
END_EVENT_TABLE()

//_____________________________________________________________________________

wxRCollisionGLCanvas::wxRCollisionGLCanvas(wxWindow *parent, const wxGLAttributes &dispAttrs, wxWindowID id,
	const wxPoint &pos, const wxSize &size, long style,
	const wxString &name, const wxPalette &palette)
	: wxRlibGLCanvas(parent, dispAttrs, id, pos, size, style, name, palette),
  m_draw_surf(true),m_draw_particles(false),m_draw_octree(false),m_draw_node(false),m_draw_monitor(false),
  m_draw_trigs(false),m_play(false),m_dt(0.01f),m_monitor(0)
//-----------------------------------------------------------------------------
{   
}
//_____________________________________________________________________________



void wxRCollisionGLCanvas::OnIdle( wxIdleEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  if(m_play)
  {
    m_sym.Solver(m_dt);
    if( m_draw_monitor ) SetMonitor( m_monitor );
    Refresh(false);
  }
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::OnFocus( wxFocusEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  UpdateCtrls();
  UpdateCtrlsCollision();
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::StepUP()
//-----------------------------------------------------------------------------
{ 
  m_play = false;  
  m_sym.Solver(m_dt);
  if( m_draw_monitor ) SetMonitor( m_monitor );
  Refresh(false);  
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::Stop()
//-----------------------------------------------------------------------------
{ 
  m_play = false;
  m_sym.ClearParticles();
  m_sym.CreateParticles(m_cx,m_cy,m_cz,m_nx,m_ny,m_nz,0.005f*m_diameter);
  Refresh(false);  
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::OpenPly(const char* plyname)
//-----------------------------------------------------------------------------
{ 
  m_sym.CreateSurfFromPly(plyname);  
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::CreateOctree(const int& level) 
//-----------------------------------------------------------------------------
{   
  m_sym.CreateOctree(level);
  m_draw_octree=true;
  if( m_draw_monitor ) SetMonitor( m_monitor );
  UpdateCtrlsCollision();
  Refresh(false);
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::CreateParticles(const real& cx , const real& cy , const real& cz ,
                                           const int& nx    , const int& ny    , const int& nz    ) 
//-----------------------------------------------------------------------------
{ 
  m_cx = (cx+1)*(m_xmax-m_xmin)*0.5f+m_xmin ;
  m_cy = (cy+1)*(m_ymax-m_ymin)*0.5f+m_ymin ;
  m_cz = (cz+1)*(m_zmax-m_zmin)*0.5f+m_zmin ;
  m_nx = nx ; m_ny = ny ; m_nz = nz ;
  m_sym.CreateParticles(m_cx,m_cy,m_cz,m_nx,m_ny,m_nz,0.005f*m_diameter);
  m_draw_particles=true;
  if( m_draw_monitor ) SetMonitor( m_monitor );
  UpdateCtrlsCollision();
  Refresh(false);
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::ClearParticles()
//-----------------------------------------------------------------------------
{ 
  m_sym.ClearParticles();
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::FlipNormal()
//-----------------------------------------------------------------------------
{ 
  BRep& surf = m_sym.Surf();
  for ( uint i = 0 ; i < surf.nverts ; ++i )
  {
    surf.verts[i].nx *= -1.0f;
    surf.verts[i].ny *= -1.0f;
    surf.verts[i].nz *= -1.0f;
  }  
  m_sym.flip_faces();
}
//_____________________________________________________________________________

void wxRCollisionGLCanvas::SetMonitor( cint& id )
//-----------------------------------------------------------------------------
{
  const PartSys::Particle* particles = m_sym.Particles();
  cuint npart = m_sym.npart();
  if( id >= 0 && id < npart )  
  {
    m_monitor = id ;
    const HOctree& tree = m_sym.Octree();    
    m_node = tree.find_leaf( particles[m_monitor].posi ) ;
  }
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::InitChild()
//-----------------------------------------------------------------------------
{
  const MType types[] = { Gold , Silver , Obsidian , Gold , Jade , Pewter };
  const wxString names[] = { wxT("Surface") , wxT("Octree") , wxT("Particles") , wxT("Monitor"), wxT("Node") ,wxT("Triangles") };
  InsertObjects( 6 , types , names );

  m_axis = false;
  ::glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
//_____________________________________________________________________________



void wxRCollisionGLCanvas::GLDraw()
//-----------------------------------------------------------------------------
{ 
  if( m_draw_surf ) DrawSurface() ;        

  if( m_draw_octree ) DrawOctree() ;

  if( m_draw_particles ) DrawParticles() ;    

  if(m_node.isValid()) 
  { 
    ::glEnable(GL_BLEND);    
    ::glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);  

    if( m_draw_trigs ) DrawStoreTrigs();    

    if( m_draw_node ) DrawNode() ;

    ::glPolygonMode(GL_FRONT_AND_BACK,m_view_mode);
    ::glDisable(GL_BLEND); 
  } 
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::DrawSurface() 
//-----------------------------------------------------------------------------
{   
  Material &M = m_objects[0].mat;
  ::glMaterialfv(GL_FRONT,GL_AMBIENT  ,M.ambient);
  ::glMaterialfv(GL_FRONT,GL_DIFFUSE  ,M.diffuse);
  ::glMaterialfv(GL_FRONT,GL_SPECULAR ,M.specular);
  ::glMateriali (GL_FRONT,GL_SHININESS,M.shininess);

  cBRep& surf = m_sym.Surf(); 
  uint ntrigsx3 = 3*surf.ntrigs;
  ::glBegin(GL_TRIANGLES);
  for( uint i = 0 ; i < ntrigsx3 ; ++i )  
  {    
    Vertex& V = surf.verts[ surf.trigs[i] ];
    ::glNormal3d( V.n[0] , V.n[1] , V.n[2] );
    ::glVertex3d( V.p[0] , V.p[1] , V.p[2] ) ;          
  }   
  ::glEnd();
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::DrawParticles() 
//-----------------------------------------------------------------------------
{
  const PartSys::Particle* particles = m_sym.Particles();  
  uint npart = m_sym.npart();

  ::glPointSize( 3.0 ) ;
  ::glBegin(GL_POINTS);

  Material &M = m_objects[3].mat;
  ::glMaterialfv(GL_FRONT,GL_AMBIENT  ,M.ambient);
  ::glMaterialfv(GL_FRONT,GL_DIFFUSE  ,M.diffuse);
  ::glMaterialfv(GL_FRONT,GL_SPECULAR ,M.specular);
  ::glMateriali (GL_FRONT,GL_SHININESS,M.shininess);

  if( m_draw_monitor &&  m_monitor < npart )
    ::glVertex3f( particles[m_monitor].posi.x , particles[m_monitor].posi.y , particles[m_monitor].posi.z ) ;      

  Material &Mp = m_objects[2].mat;
  ::glMaterialfv(GL_FRONT,GL_AMBIENT  ,Mp.ambient);
  ::glMaterialfv(GL_FRONT,GL_DIFFUSE  ,Mp.diffuse);
  ::glMaterialfv(GL_FRONT,GL_SPECULAR ,Mp.specular);
  ::glMateriali (GL_FRONT,GL_SHININESS,Mp.shininess);

  for( uint i = 0 ; i < npart ; ++i )  
    ::glVertex3f( particles[i].posi.x , particles[i].posi.y , particles[i].posi.z ) ;       

  ::glEnd();
  ::glPointSize(1.0f) ;
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::DrawOctree() 
//-----------------------------------------------------------------------------
{ 
  Material &M = m_objects[1].mat;
  ::glMaterialfv(GL_FRONT,GL_AMBIENT  ,M.ambient);
  ::glMaterialfv(GL_FRONT,GL_DIFFUSE  ,M.diffuse);
  ::glMaterialfv(GL_FRONT,GL_SPECULAR ,M.specular);
  ::glMateriali (GL_FRONT,GL_SHININESS,M.shininess);
  
  ::glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
  
  const HOctree& tree = m_sym.Octree();  
  for(HOctree::iterator it = tree.begin() , end = tree.end() ; it != end ; ++it )    
    DrawCube( tree.cube(it) );
  
  ::glPolygonMode(GL_FRONT_AND_BACK,m_view_mode);  
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::DrawNode() 
//-----------------------------------------------------------------------------
{  
  Material &M = m_objects[4].mat;
  ::glMaterialfv(GL_FRONT,GL_AMBIENT  ,M.ambient);
  ::glMaterialfv(GL_FRONT,GL_DIFFUSE  ,M.diffuse);
  ::glMaterialfv(GL_FRONT,GL_SPECULAR ,M.specular);
  ::glMateriali (GL_FRONT,GL_SHININESS,M.shininess);

  DrawCube( m_sym.Octree().cube(m_node) );     
}
//_____________________________________________________________________________


void wxRCollisionGLCanvas::DrawStoreTrigs()
//-----------------------------------------------------------------------------
{  
  Material &M = m_objects[5].mat;
  ::glMaterialfv(GL_FRONT,GL_AMBIENT  ,M.ambient);
  ::glMaterialfv(GL_FRONT,GL_DIFFUSE  ,M.diffuse);
  ::glMaterialfv(GL_FRONT,GL_SPECULAR ,M.specular);
  ::glMateriali (GL_FRONT,GL_SHININESS,M.shininess);

  cBRep& surf = m_sym.Surf();
  
  Vertex V0,V1,V2;
  const vector<int>& corners = (*m_node).trigs() ;
  ::glBegin(GL_TRIANGLES);   
  for( int j = 0 ; j < corners.size() ; ++j )
  {
    uint i = corners[j];
    V0 = surf.verts[ surf.trigs[ 3*i+0 ] ];
    ::glNormal3d( V0.n[0] , V0.n[1] , V0.n[2] );
    ::glVertex3d( V0.p[0] , V0.p[1] , V0.p[2] ) ;  

    V1 = surf.verts[ surf.trigs[ 3*i+1 ] ];
    ::glNormal3d( V1.n[0] , V1.n[1] , V1.n[2] );
    ::glVertex3d( V1.p[0] , V1.p[1] , V1.p[2] ) ;  

    V2 = surf.verts[ surf.trigs[ 3*i+2 ] ];
    ::glNormal3d( V2.n[0] , V2.n[1] , V2.n[2] );
    ::glVertex3d( V2.p[0] , V2.p[1] , V2.p[2] ) ;  
  }
  ::glEnd();  
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//
void wxRCollisionGLCanvas::DrawCube( const Cube& c ) 
//-----------------------------------------------------------------------------
{      
  ::glBegin( GL_QUADS );
  ::glNormal3f( 0.0, 0.0, 1.0);
  ::glVertex3f( c.center.x-c.side , c.center.y-c.side , c.center.z+c.side ) ;
  ::glVertex3f( c.center.x+c.side , c.center.y-c.side , c.center.z+c.side ) ;
  ::glVertex3f( c.center.x+c.side , c.center.y+c.side , c.center.z+c.side ) ;
  ::glVertex3f( c.center.x-c.side , c.center.y+c.side , c.center.z+c.side ) ;    

  ::glNormal3f( 0.0, 0.0,-1.0);
  ::glVertex3f( c.center.x-c.side , c.center.y-c.side , c.center.z-c.side ) ;
  ::glVertex3f( c.center.x+c.side , c.center.y-c.side , c.center.z-c.side ) ;
  ::glVertex3f( c.center.x+c.side , c.center.y+c.side , c.center.z-c.side ) ;
  ::glVertex3f( c.center.x-c.side , c.center.y+c.side , c.center.z-c.side ) ;    

  ::glNormal3f( 1.0, 0.0, 0.0);
  ::glVertex3f( c.center.x+c.side , c.center.y-c.side , c.center.z-c.side ) ;
  ::glVertex3f( c.center.x+c.side , c.center.y+c.side , c.center.z-c.side ) ;
  ::glVertex3f( c.center.x+c.side , c.center.y+c.side , c.center.z+c.side ) ;
  ::glVertex3f( c.center.x+c.side , c.center.y-c.side , c.center.z+c.side ) ;    

  ::glNormal3f(-1.0, 0.0, 0.0);
  ::glVertex3f( c.center.x-c.side , c.center.y-c.side , c.center.z-c.side ) ;
  ::glVertex3f( c.center.x-c.side , c.center.y+c.side , c.center.z-c.side ) ;
  ::glVertex3f( c.center.x-c.side , c.center.y+c.side , c.center.z+c.side ) ;
  ::glVertex3f( c.center.x-c.side , c.center.y-c.side , c.center.z+c.side ) ;    

  ::glNormal3f( 0.0, 1.0, 0.0);
  ::glVertex3f( c.center.x-c.side , c.center.y+c.side , c.center.z-c.side ) ;
  ::glVertex3f( c.center.x+c.side , c.center.y+c.side , c.center.z-c.side ) ;
  ::glVertex3f( c.center.x+c.side , c.center.y+c.side , c.center.z+c.side ) ;
  ::glVertex3f( c.center.x-c.side , c.center.y+c.side , c.center.z+c.side ) ;    

  ::glNormal3f( 0.0,-1.0, 0.0);
  ::glVertex3f( c.center.x-c.side , c.center.y-c.side , c.center.z-c.side ) ;
  ::glVertex3f( c.center.x+c.side , c.center.y-c.side , c.center.z-c.side ) ;
  ::glVertex3f( c.center.x+c.side , c.center.y-c.side , c.center.z+c.side ) ;
  ::glVertex3f( c.center.x-c.side , c.center.y-c.side , c.center.z+c.side ) ;    
  ::glEnd();
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//

void wxRCollisionGLCanvas::UpdateCtrlsCollision()
//-----------------------------------------------------------------------------
{
  wxRCollisionFrame* parent = (wxRCollisionFrame*)GetGrandParent();

  wxRCollisionScene* scene = (wxRCollisionScene*)parent->GetPane(wxT("Scene")) ;   
  if(scene) 
  {
    scene->SetValue(m_draw_surf,m_draw_octree,m_draw_particles,m_draw_node,m_draw_trigs);
    scene->SetMonitor( m_monitor , m_sym.npart()-1 , m_draw_monitor );
  }

  wxRCollisionCtrls* ctrls = (wxRCollisionCtrls*)parent->GetPane(wxT("Ctrls")) ;   
  if(ctrls)  ctrls->SetValue( m_sym.KF() , m_sym.KR() , m_dt );
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//_____________________________________________________________________________
