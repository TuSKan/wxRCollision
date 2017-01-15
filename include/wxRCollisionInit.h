/**
 * @file    wxRCollisionInit.h
 * @author  Rener Pereira de Castro <rener@mat.puc-rio.br> 
 * @version 
 * @date    
 *
 * @brief
 *
 */
//________________________________________________


#ifndef _WXRCOLLISIONINIT_H_
#define _WXRCOLLISIONINIT_H_

#ifndef WIN32
#pragma interface
#endif // WIN32


#include "wxRCollision.h"


//_____________________________________________________________________________
//Class wxRCollisionInit

class wxRCollisionInit : public wxPanel
//-----------------------------------------------------------------------------
{
public:
	// Constructor & Destructor  
  wxRCollisionInit(wxRCollisionFrame* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, 
                  const wxString& name = wxT("panel") )
    : wxPanel(parent,id,pos,size,style,name) , m_parent(parent)
  {
    CreateCtrls();
  }

  ~wxRCollisionInit() {}
//------------------------------------------------------
  //Accessories 
protected:      
  void OnParticlesCreate( wxCommandEvent& WXUNUSED(event));
  void OnParticlesClear( wxCommandEvent& WXUNUSED(event));
  void OnOctreeCreate( wxCommandEvent& WXUNUSED(event));
  void OnOctreeClear( wxCommandEvent& WXUNUSED(event));
  void OnFlipNormal( wxCommandEvent& WXUNUSED(event) );
  //------------------------------------------------------
  //Members
protected:
  void CreateCtrls() ;
public:
  void SetValue( bool mesh , bool octree , bool particles , 
                 bool node , bool neigh , bool trigs );
  void SetMonitor( const int& id , const int& max , bool show );  
  //------------------------------------------------------
  //Elements
private:
  wxTextCtrl* m_textctrl_cx ;
  wxTextCtrl* m_textctrl_cy ;
  wxTextCtrl* m_textctrl_cz ;
  wxTextCtrl* m_textctrl_nx ;
  wxTextCtrl* m_textctrl_ny ;
  wxTextCtrl* m_textctrl_nz ;

  wxTextCtrl* m_textctrl_lev ;

  wxRCollisionFrame* m_parent ;

  //------------------------------------------------------
  DECLARE_EVENT_TABLE()
	//-----------------------------------------------------------------------------
};
//_____________________________________________________________________________
  

//_____________________________________________________________________________
#endif  //  _WXCOLLISIONINIT_H_
