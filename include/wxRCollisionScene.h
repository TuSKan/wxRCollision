/**
 * @file    wxRCollisionScene.h
 * @author  Rener Pereira de Castro <rener@mat.puc-rio.br> 
 * @version 
 * @date    
 *
 * @brief
 *
 */
//________________________________________________


#ifndef _WXRCOLLISIONSCENE_H_
#define _WXRCOLLISIONSCENE_H_

#ifndef WIN32
#pragma interface
#endif // WIN32


#include "wxRCollision.h"
#include <wx/spinctrl.h>

//_____________________________________________________________________________
//Class wxCollisionScene

class wxRCollisionScene : public wxPanel
//-----------------------------------------------------------------------------
{
public:
	// Constructor & Desctructor  
  wxRCollisionScene(wxRCollisionFrame* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, 
                  const wxString& name = wxT("panel") )
    : wxPanel(parent,id,pos,size,style,name) , m_parent(parent)
  {
    CreateCtrls();
  }

  ~wxRCollisionScene() {}
//------------------------------------------------------
  //Accessories 
protected:      
  void OnShowMesh( wxCommandEvent& WXUNUSED(event));
  void OnShowOctree( wxCommandEvent& WXUNUSED(event) );
  void OnShowParticles( wxCommandEvent& WXUNUSED(event) );
  void OnShowNode( wxCommandEvent& WXUNUSED(event) );  
  void OnShowTrigs( wxCommandEvent& WXUNUSED(event) );  
  void OnCreateOctree( wxCommandEvent& WXUNUSED(event) );
  void OnCreateParticles( wxCommandEvent& WXUNUSED(event) );
  void OnFlipNormal( wxCommandEvent& WXUNUSED(event) );
  void OnMonitor( wxSpinEvent& WXUNUSED(event) );
  void OnShowMonitor( wxCommandEvent& WXUNUSED(event) );
  void OnFlipNormals( wxCommandEvent& WXUNUSED(event) );
  //------------------------------------------------------
  //Members
protected:
  void CreateCtrls() ;
public:
  void SetValue( bool mesh , bool octree , bool particles , 
                 bool node , bool trigs );
  void SetMonitor( const int& id , const int& max , bool show );  
  //------------------------------------------------------
  //Elementos
private:
  wxCheckBox* m_check_mesh;
  wxCheckBox* m_check_octree;
  wxCheckBox* m_check_particles;
  wxCheckBox* m_check_node;
  wxCheckBox* m_check_trigs; 

  wxSpinCtrl* m_spin_monitor;
  wxCheckBox* m_check_monitor;

  wxRCollisionFrame* m_parent ;

  //------------------------------------------------------
  DECLARE_EVENT_TABLE()
	//-----------------------------------------------------------------------------
};
//_____________________________________________________________________________
  

//_____________________________________________________________________________
#endif  //  _WXCOLLISIONSCENE_H_
