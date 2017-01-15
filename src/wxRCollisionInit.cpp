/**
 * @file    wxRCollisionInit.cpp
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

#include "wxRCollisionInit.h"
#include "wxRCollisionGLCanvas.h"
#include "wxRCollision.h"

//_____________________________________________________________________________
enum 
{ 
  ID_TEXTCTRL_CX = wxID_HIGHEST +1 ,
  ID_TEXTCTRL_CY  ,
  ID_TEXTCTRL_CZ  ,
  ID_TEXTCTRL_NX  ,
  ID_TEXTCTRL_NY  ,
  ID_TEXTCTRL_NZ  ,
  ID_TEXTCTRL_LEV ,
  ID_BUTTON_PARTICLE_CREATE,
  ID_BUTTON_PARTICLE_CLEAR,
  ID_BUTTON_OCTREE_CREATE ,
  ID_BUTTON_OCTREE_CLEAR
};
//_____________________________________________________________________________


BEGIN_EVENT_TABLE(wxRCollisionInit, wxPanel)
  EVT_BUTTON( ID_BUTTON_PARTICLE_CREATE , wxRCollisionInit::OnParticlesCreate )
  EVT_BUTTON( ID_BUTTON_PARTICLE_CLEAR  , wxRCollisionInit::OnParticlesClear )
  EVT_BUTTON( ID_BUTTON_OCTREE_CREATE   , wxRCollisionInit::OnOctreeCreate )
  EVT_BUTTON( ID_BUTTON_OCTREE_CLEAR   , wxRCollisionInit::OnOctreeClear )
END_EVENT_TABLE()
//_____________________________________________________________________________


void wxRCollisionInit::CreateCtrls()
//-----------------------------------------------------------------------------
{ 
  wxBoxSizer *item0 = new wxBoxSizer( wxHORIZONTAL );

  //--------------------------------------------------------------

  wxStaticBox *item2 = new wxStaticBox( this, -1, wxT("Particles") );
  wxStaticBoxSizer *item1 = new wxStaticBoxSizer( item2, wxVERTICAL );

  wxBoxSizer *item3 = new wxBoxSizer( wxHORIZONTAL );

  wxBoxSizer *item4 = new wxBoxSizer( wxVERTICAL );

  wxStaticText *item5 = new wxStaticText( this, wxID_ANY, wxT("x"), wxDefaultPosition, wxDefaultSize, 0 );
  item4->Add( item5, 0, wxALIGN_CENTER, 5 );

  m_textctrl_cx = new wxTextCtrl( this, ID_TEXTCTRL_CX, wxT("0.0"), wxDefaultPosition, wxSize(40,-1), 0 );
  item4->Add( m_textctrl_cx, 0, wxALIGN_CENTER, 5 );

  item3->Add( item4, 0, wxALIGN_CENTER, 5 );

  wxBoxSizer *item7 = new wxBoxSizer( wxVERTICAL );

  wxStaticText *item8 = new wxStaticText( this, wxID_ANY, wxT("y"), wxDefaultPosition, wxDefaultSize, 0 );
  item7->Add( item8, 0, wxALIGN_CENTER, 5 );

  m_textctrl_cy = new wxTextCtrl( this, ID_TEXTCTRL_CY, wxT("0.0"), wxDefaultPosition, wxSize(40,-1), 0 );
  item7->Add( m_textctrl_cy, 0, wxALIGN_CENTER, 5 );

  item3->Add( item7, 0, wxALIGN_CENTER, 5 );

  wxBoxSizer *item10 = new wxBoxSizer( wxVERTICAL );

  wxStaticText *item11 = new wxStaticText( this, wxID_ANY, wxT("z"), wxDefaultPosition, wxDefaultSize, 0 );
  item10->Add( item11, 0, wxALIGN_CENTER, 5 );

  m_textctrl_cz = new wxTextCtrl( this, ID_TEXTCTRL_CZ, wxT("0.0"), wxDefaultPosition, wxSize(40,-1), 0 );
  item10->Add( m_textctrl_cz, 0, wxALIGN_CENTER, 5 );

  item3->Add( item10, 0, wxALIGN_CENTER, 5 );

  item1->Add( item3, 0, wxALIGN_CENTER, 5 );

  wxBoxSizer *item13 = new wxBoxSizer( wxHORIZONTAL );

  wxBoxSizer *item14 = new wxBoxSizer( wxVERTICAL );

  wxStaticText *item15 = new wxStaticText( this, wxID_ANY, wxT("nx"), wxDefaultPosition, wxDefaultSize, 0 );
  item14->Add( item15, 0, wxALIGN_CENTER, 5 );

  m_textctrl_nx = new wxTextCtrl( this, ID_TEXTCTRL_NX, wxT("50"), wxDefaultPosition, wxSize(40,-1), 0 );
  item14->Add( m_textctrl_nx, 0, wxALIGN_CENTER, 5 );

  item13->Add( item14, 0, wxALIGN_CENTER, 5 );

  wxBoxSizer *item17 = new wxBoxSizer( wxVERTICAL );

  wxStaticText *item18 = new wxStaticText( this, wxID_ANY, wxT("ny"), wxDefaultPosition, wxDefaultSize, 0 );
  item17->Add( item18, 0, wxALIGN_CENTER, 5 );

  m_textctrl_ny = new wxTextCtrl( this, ID_TEXTCTRL_NY, wxT("50"), wxDefaultPosition, wxSize(40,-1), 0 );
  item17->Add( m_textctrl_ny, 0, wxALIGN_CENTER, 5 );

  item13->Add( item17, 0, wxALIGN_CENTER, 5 );

  wxBoxSizer *item20 = new wxBoxSizer( wxVERTICAL );

  wxStaticText *item21 = new wxStaticText( this, wxID_ANY, wxT("nz"), wxDefaultPosition, wxDefaultSize, 0 );
  item20->Add( item21, 0, wxALIGN_CENTER, 5 );

  m_textctrl_nz = new wxTextCtrl( this, ID_TEXTCTRL_NZ, wxT("50"), wxDefaultPosition, wxSize(40,-1), 0 );
  item20->Add( m_textctrl_nz, 0, wxALIGN_CENTER, 5 );

  item13->Add( item20, 0, wxALIGN_CENTER, 5 );

  item1->Add( item13, 0, wxALIGN_CENTER, 5 );

  wxBoxSizer *item23 = new wxBoxSizer( wxHORIZONTAL );

  wxButton *item24 = new wxButton( this, ID_BUTTON_PARTICLE_CREATE, wxT("Create"), wxDefaultPosition, wxSize(50,-1), 0 );
  item23->Add( item24, 0, wxALIGN_CENTER, 5 );

  wxButton *item25 = new wxButton( this, ID_BUTTON_PARTICLE_CLEAR, wxT("Clear"), wxDefaultPosition, wxSize(50,-1), 0 );
  item23->Add( item25, 0, wxALIGN_CENTER, 5 );

  item1->Add( item23, 0, wxALIGN_CENTER|wxTOP, 5 );

  item0->Add( item1, 0, wxALIGN_CENTER, 5 );

  wxStaticBox *item27 = new wxStaticBox( this, -1, wxT("HOctree") );
  wxStaticBoxSizer *item26 = new wxStaticBoxSizer( item27, wxVERTICAL );

  wxStaticText *item28 = new wxStaticText( this, wxID_ANY, wxT("Level "), wxDefaultPosition, wxDefaultSize, 0 );
  item26->Add( item28, 0, wxALIGN_CENTER, 5 );

  m_textctrl_lev = new wxTextCtrl( this, ID_TEXTCTRL_LEV, wxT("8"), wxDefaultPosition, wxSize(30,-1), 0 );
  item26->Add( m_textctrl_lev, 0, wxALIGN_CENTER, 5 );

  wxButton *item30 = new wxButton( this, ID_BUTTON_OCTREE_CREATE, wxT("Create"), wxDefaultPosition, wxSize(50,-1), 0 );
  item26->Add( item30, 0, wxALIGN_CENTER, 5 );

  wxButton *item31 = new wxButton( this, ID_BUTTON_OCTREE_CLEAR, wxT("Clear"), wxDefaultPosition, wxSize(50,-1), 0 );
  item26->Add( item31, 0, wxALIGN_CENTER, 5 );

  item0->Add( item26, 0, wxALIGN_CENTER, 5 );
  
  //--------------------------------------------------------------
  
  SetSizer( item0 ) ;

  SetAutoLayout (true);
	Layout();
}
//_____________________________________________________________________________


void wxRCollisionInit::OnParticlesCreate( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->CreateParticles( atof( m_textctrl_cx->GetValue().c_str() ) ,
                             atof( m_textctrl_cy->GetValue().c_str() ) ,
                             atof( m_textctrl_cz->GetValue().c_str() ) ,
                             atoi( m_textctrl_nx->GetValue().c_str() ) ,
                             atoi( m_textctrl_ny->GetValue().c_str() ) ,
                             atoi( m_textctrl_nz->GetValue().c_str() ) ) ;
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


void wxRCollisionInit::OnParticlesClear( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->ClearParticles();
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


void wxRCollisionInit::OnOctreeCreate( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->CreateOctree( atoi( m_textctrl_lev->GetValue().c_str() ) );
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


void wxRCollisionInit::OnOctreeClear( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->ClearOctree( );
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//_____________________________________________________________________________