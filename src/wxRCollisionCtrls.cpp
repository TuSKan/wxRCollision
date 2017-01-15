/**
 * @file    wxRCollisionCtrls.cpp
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

#include "wxRCollisionCtrls.h"
#include "wxRCollisionGLCanvas.h"
#include "wxRCollision.h"

//_____________________________________________________________________________
enum 
{   
  ID_BUTTON_PLAY = wxID_HIGHEST +1 ,
  ID_BUTTON_SLOW , 
  ID_BUTTON_PAUSE,
  ID_BUTTON_STOP ,
  ID_TEXTCTRL_DT ,
  ID_TEXTCTRL_KR ,
  ID_TEXTCTRL_KF
};
//_____________________________________________________________________________


BEGIN_EVENT_TABLE(wxRCollisionCtrls, wxPanel)   
  EVT_BUTTON(ID_BUTTON_PLAY    , wxRCollisionCtrls::OnPlay ) 
  EVT_BUTTON(ID_BUTTON_SLOW    , wxRCollisionCtrls::OnSlow ) 
  EVT_BUTTON(ID_BUTTON_PAUSE   , wxRCollisionCtrls::OnPause ) 
  EVT_BUTTON(ID_BUTTON_STOP    , wxRCollisionCtrls::OnStop )   
END_EVENT_TABLE()
//_____________________________________________________________________________


void wxRCollisionCtrls::CreateCtrls()
//-----------------------------------------------------------------------------
{ 
  wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

  //--------------------------------------------------------------
  // Media

  wxStaticBox *item1 = new wxStaticBox( this , -1, wxT("Media") );
  wxStaticBoxSizer *item6 = new wxStaticBoxSizer( item1, wxHORIZONTAL );

  wxBitmapButton *item2 = new wxBitmapButton( this, ID_BUTTON_PLAY,  wxBITMAP(play)  , wxDefaultPosition, wxSize(30,30) );
  item6->Add( item2, 0, wxALIGN_CENTER, 5 );

  wxBitmapButton *item3 = new wxBitmapButton( this, ID_BUTTON_SLOW,  wxBITMAP(slow)  , wxDefaultPosition, wxSize(30,30) );
  item6->Add( item3, 0, wxALIGN_CENTER, 5 );

  wxBitmapButton *item4 = new wxBitmapButton( this, ID_BUTTON_PAUSE, wxBITMAP(pause)  , wxDefaultPosition, wxSize(30,30) );
  item6->Add( item4, 0, wxALIGN_CENTER, 5 );

  wxBitmapButton *item5 = new wxBitmapButton( this, ID_BUTTON_STOP,  wxBITMAP(stop)  , wxDefaultPosition, wxSize(30,30) );
  item6->Add( item5, 0, wxALIGN_CENTER, 5 );

  item0->Add( item6, 0, wxALIGN_CENTER, 5 );

  //--------------------------------------------------------------
  // Attribs
  wxStaticBox *item8 = new wxStaticBox( this, -1, wxT("Set Attribs") );
  wxStaticBoxSizer *item7 = new wxStaticBoxSizer( item8, wxHORIZONTAL );

  wxBoxSizer *item9 = new wxBoxSizer( wxVERTICAL );

  wxStaticText *item10 = new wxStaticText( this, wxID_ANY, wxT("KF"), wxDefaultPosition, wxDefaultSize, 0 );
  item9->Add( item10, 0, wxALIGN_CENTER, 5 );

  m_textctrl_kf = new wxTextCtrl( this, ID_TEXTCTRL_KF, wxT(""), wxDefaultPosition, wxSize(40,-1), 0 );
  item9->Add( m_textctrl_kf, 0, wxALIGN_CENTER, 5 );

  item7->Add( item9, 0, wxALIGN_CENTER, 5 );

  wxBoxSizer *item12 = new wxBoxSizer( wxVERTICAL );

  wxStaticText *item16 = new wxStaticText( this, wxID_ANY, wxT("KR"), wxDefaultPosition, wxDefaultSize, 0 );
  item12->Add( item16, 0, wxALIGN_CENTER, 5 );

  m_textctrl_kr = new wxTextCtrl( this, ID_TEXTCTRL_KR, wxT(""), wxDefaultPosition, wxSize(40,-1), 0 );
  item12->Add( m_textctrl_kr, 0, wxALIGN_CENTER, 5 );

  item7->Add( item12, 0, wxALIGN_CENTER, 5 );

  wxBoxSizer *item15 = new wxBoxSizer( wxVERTICAL );

  wxStaticText *item17 = new wxStaticText( this, wxID_ANY, wxT("DT"), wxDefaultPosition, wxDefaultSize, 0 );
  item15->Add( item17, 0, wxALIGN_CENTER, 5 );

  m_textctrl_dt = new wxTextCtrl( this, ID_TEXTCTRL_DT, wxT(""), wxDefaultPosition, wxSize(40,-1), 0 );
  item15->Add( m_textctrl_dt, 0, wxALIGN_CENTER, 5 );

  item7->Add( item15, 0, wxALIGN_CENTER, 5 );

  item0->Add( item7, 0, wxALIGN_CENTER, 5 );
  //--------------------------------------------------------------

  SetSizer( item0 ) ;

  SetAutoLayout (true);
	Layout();
}
//_____________________________________________________________________________


void wxRCollisionCtrls::OnPlay( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->SetKF( atof( m_textctrl_kf->GetValue().c_str() ) );
    opengl->SetKR( atof( m_textctrl_kr->GetValue().c_str() ) );
    opengl->SetDT( atof( m_textctrl_dt->GetValue().c_str() ) );
    opengl->Play();    
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


void wxRCollisionCtrls::OnSlow( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->SetKF( atof( m_textctrl_kf->GetValue().c_str() ) );
    opengl->SetKR( atof( m_textctrl_kr->GetValue().c_str() ) );
    opengl->SetDT( atof( m_textctrl_dt->GetValue().c_str() ) );
    opengl->StepUP();    
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


void wxRCollisionCtrls::OnPause( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->Pause();
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


void wxRCollisionCtrls::OnStop( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->Stop();
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


void wxRCollisionCtrls::SetValue( const double& kf , const double& kr , const double& dt )
//-----------------------------------------------------------------------------
{ 
  m_textctrl_kf->SetValue(wxString::Format("%G",kf) );
  m_textctrl_kr->SetValue(wxString::Format("%G",kr) );
  m_textctrl_dt->SetValue(wxString::Format("%G",dt) );
  
}
//_____________________________________________________________________________



//_____________________________________________________________________________
//_____________________________________________________________________________