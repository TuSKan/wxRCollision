/**
 * @file    wxRCollisionCtrls.h
 * @author  Rener Pereira de Castro <rener@mat.puc-rio.br> 
 * @version 
 * @date    
 *
 * @brief
 *
 */
//________________________________________________


#ifndef _WXRCOLLISIONCTRLS_H_
#define _WXRCOLLISIONCTRLS_H_

#ifndef WIN32
#pragma interface
#endif // WIN32


#include "wxRCollision.h"

//_____________________________________________________________________________
//Class wxRCollisionCtrls

class wxRCollisionCtrls : public wxPanel
//-----------------------------------------------------------------------------
{
public:
	// Constructor & Destructor  
  wxRCollisionCtrls(wxRCollisionFrame* parent, wxWindowID id = -1, const wxPoint& pos = wxDefaultPosition,
                  const wxSize& size = wxDefaultSize, long style = wxTAB_TRAVERSAL, 
                  const wxString& name = wxT("panel") )
    : wxPanel(parent,id,pos,size,style,name) , m_parent(parent)
  {
    CreateCtrls();
  }

  ~wxRCollisionCtrls() {}
//------------------------------------------------------
  //Accessories 
protected:        
  void OnPlay( wxCommandEvent& WXUNUSED(event) );
  void OnSlow( wxCommandEvent& WXUNUSED(event) );
  void OnPause( wxCommandEvent& WXUNUSED(event) );
  void OnStop( wxCommandEvent& WXUNUSED(event) );  
  //------------------------------------------------------
  //Members
protected:
  void CreateCtrls() ;
public:
  void SetValue( const double& kf , const double& kr , const double& dt ) ;
  //------------------------------------------------------
  //Elementos
private:
  wxTextCtrl* m_textctrl_kf ;
  wxTextCtrl* m_textctrl_kr ;
  wxTextCtrl* m_textctrl_dt ;
  
  wxRCollisionFrame* m_parent ;

  //------------------------------------------------------
  DECLARE_EVENT_TABLE()
	//-----------------------------------------------------------------------------
};
//_____________________________________________________________________________
  

//_____________________________________________________________________________
#endif  //  _WXCOLLISIONCTRLS_H_
