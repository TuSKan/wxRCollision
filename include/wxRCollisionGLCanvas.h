/**
 * @file    wxRCollisionGL.h
 * @author  Rener Pereira de Castro <rener@mat.puc-rio.br> 
 * @version 
 * @date    
 *
 * @brief
 *
 */
//________________________________________________


#ifndef _WXRCOLLISIONGL_H_
#define _WXRCOLLISIONGL_H_

#ifndef WIN32
#pragma interface
#endif // WIN32


#include "Utils.h"
#include "PartSys.h"
#include "wxRCollision.h"
#include <wxRlibGLCanvas.h>
//_____________________________________________________________________________
//Class wxExampleGLCanvas

class wxRCollisionGLCanvas : public wxRlibGLCanvas
//-----------------------------------------------------------------------------
{
public:
	// Constructor & Destructor  
  wxRCollisionGLCanvas(wxWindow *parent, const wxGLAttributes &dispAttrs, wxWindowID id = wxID_ANY,
	  const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = 0,
	  const wxString &name = wxGLCanvasName, const wxPalette &palette = wxNullPalette);
    
  ~wxRCollisionGLCanvas() {}
  //------------------------------------------------------
  // Event Members
protected:
  void OnIdle( wxIdleEvent& WXUNUSED(event) );  
  void OnFocus(wxFocusEvent& WXUNUSED(event) );  
  //------------------------------------------------------
  // Draw Members
protected:
  virtual void InitChild();
  virtual void GLDraw() ;
  void DrawSurface() ;
  void DrawParticles() ;
  void DrawOctree() ;
  void DrawNode() ;  
  void DrawStoreTrigs() ;    
  void DrawCube  ( const Cube& c ) ;
  //------------------------------------------------------
  //Set and Get Data
public:  
  void OpenPly(const char* plyname) ;
  void CreateOctree(const int& level = 10) ;
  inline void ClearOctree() { m_sym.ClearOctree(); }
  void CreateParticles(const real& cx,const real& cy,const real& cz,
                       const int& nx,const int& ny,const int& nz ) ;
  void ClearParticles() ;
  void FlipNormal();

//------------------------------------------------------
  //Set and Get Data
public:
  void ShowMesh(bool value) { m_draw_surf=value; }
  void ShowOctree(bool value) { m_draw_octree=value; }
  void ShowParticles(bool value) { m_draw_particles=value; }
  void ShowNode(bool value) { m_draw_node=value; }
  void ShowTrigs(bool value) { m_draw_trigs=value; }  
  void Play()  { m_play = true ; };
  void Pause() { m_play = false; };
  void StepUP() ;
  void Stop() ;
  void SetKF(const real& kf) { m_sym.KF(kf); }
  void SetKR(const real& kr) { m_sym.KR(kr); }
  void SetDT(const real& dt) { m_dt = dt; }
  void ShowMonitor(bool value) { m_draw_monitor = value; }
  void SetMonitor( const int& id ) ;

protected:
  void UpdateCtrlsCollision();
  //------------------------------------------------------
  //Elements
private:  
  bool m_draw_node,m_draw_surf,m_draw_particles,m_draw_octree,m_draw_trigs,m_play,m_draw_monitor;  
  HOctree::iterator m_node;  
  real m_dt;
  int  m_monitor ;
  real m_cx,m_cy,m_cz;
  int  m_nx,m_ny,m_nz;

  PartSys m_sym;    
	//----------------------------------------------------------------------------- 
  DECLARE_EVENT_TABLE()
};
//_____________________________________________________________________________


//_____________________________________________________________________________
// Class wxExampleGL

class wxRCollisionGL : public wxRlibGL
//-----------------------------------------------------------------------------
{
  //------------------------------------------------------
  //Constructor
public:
  wxRCollisionGL( wxRCollisionFrame* parent ) : wxRlibGL(parent) {CreateCtrl();}
  ~wxRCollisionGL() {  }    
  //------------------------------------------------------
  // Members
protected:
  void CreateCtrl()
  {
	wxGLAttributes cxtAttrs;
	cxtAttrs.PlatformDefaults().Defaults().EndList();
	if (!wxGLCanvas::IsDisplaySupported(cxtAttrs)) {
		cxtAttrs.Reset();
		cxtAttrs.PlatformDefaults().RGBA().DoubleBuffer().Depth(16).EndList();
	}
    m_opengl = new wxRCollisionGLCanvas(this, cxtAttrs);
    wxBoxSizer* main = new wxBoxSizer(wxVERTICAL);
    main->Add(m_opengl,1,wxALL|wxEXPAND|wxALIGN_CENTER);
    SetSizer(main);
    SetAutoLayout(true);
    Layout();
  }
  //------------------------------------------------------
  //Set Get Data
public:
  wxRCollisionGLCanvas* GL() { return (wxRCollisionGLCanvas*)m_opengl; }
  //------------------------------------------------------    
};
//_____________________________________________________________________________

//_____________________________________________________________________________
#endif  //  _       _H_