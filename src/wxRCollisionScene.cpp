/**
 * @file    wxRCollisionScene.cpp
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

#include "wxRCollisionScene.h"
#include "wxRCollisionGLCanvas.h"
#include "wxRCollision.h"
#include <wx/statline.h>

//_____________________________________________________________________________
enum 
{ 
  ID_CHECKBOX_MESH = wxID_HIGHEST +1 ,
  ID_CHECKBOX_OCTREE ,
  ID_CHECKBOX_PARTICLES,  
  ID_CHECKBOX_NODE ,
  ID_CHECKBOX_TRIGS ,    
  ID_SPINCTRL_MONITOR,
  ID_CHECKBOX_PID ,
  ID_BUTTON_FLIP

};
//_____________________________________________________________________________


BEGIN_EVENT_TABLE(wxRCollisionScene, wxPanel)
  EVT_CHECKBOX(ID_CHECKBOX_MESH     , wxRCollisionScene::OnShowMesh ) 
  EVT_CHECKBOX(ID_CHECKBOX_OCTREE   , wxRCollisionScene::OnShowOctree ) 
  EVT_CHECKBOX(ID_CHECKBOX_PARTICLES, wxRCollisionScene::OnShowParticles ) 
  EVT_CHECKBOX(ID_CHECKBOX_NODE     , wxRCollisionScene::OnShowNode ) 
  EVT_CHECKBOX(ID_CHECKBOX_TRIGS    , wxRCollisionScene::OnShowTrigs )     
  EVT_SPINCTRL(ID_SPINCTRL_MONITOR  , wxRCollisionScene::OnMonitor ) 
  EVT_CHECKBOX(ID_CHECKBOX_PID      , wxRCollisionScene::OnShowMonitor )   
  EVT_BUTTON(ID_BUTTON_FLIP         , wxRCollisionScene::OnFlipNormals )   
END_EVENT_TABLE()
//_____________________________________________________________________________


void wxRCollisionScene::CreateCtrls()
//-----------------------------------------------------------------------------
{ 
  wxBoxSizer *item0 = new wxBoxSizer( wxVERTICAL );

  //--------------------------------------------------------------

  wxStaticBox *item12 = new wxStaticBox( this, -1, wxT("Show") );
  wxStaticBoxSizer *item13 = new wxStaticBoxSizer( item12, wxVERTICAL );

  wxFlexGridSizer *item14 = new wxFlexGridSizer( 2, 0, 0, 0 );

  m_check_mesh = new wxCheckBox( this, ID_CHECKBOX_MESH, wxT("Mesh"), wxDefaultPosition, wxDefaultSize, 0 );
  item14->Add( m_check_mesh, 0, wxALIGN_LEFT, 5 );

  m_check_octree = new wxCheckBox( this, ID_CHECKBOX_OCTREE, wxT("HOctree"), wxDefaultPosition, wxDefaultSize, 0 );
  item14->Add( m_check_octree, 0, wxALIGN_LEFT, 5 );

  m_check_particles = new wxCheckBox( this, ID_CHECKBOX_PARTICLES, wxT("Particles"), wxDefaultPosition, wxDefaultSize, 0 );
  item14->Add( m_check_particles, 0, wxALIGN_LEFT, 5 );

  m_check_node = new wxCheckBox( this, ID_CHECKBOX_NODE, wxT("Node"), wxDefaultPosition, wxDefaultSize, 0 );
  item14->Add( m_check_node, 0, wxALIGN_LEFT, 5 ); 

  m_check_trigs = new wxCheckBox( this, ID_CHECKBOX_TRIGS, wxT("Triangles"), wxDefaultPosition, wxDefaultSize, 0 );
  item14->Add( m_check_trigs, 0, wxALIGN_LEFT, 5 ); 

  item13->Add( item14, 0, wxALIGN_CENTER, 5 );

  item0->Add( item13, 0, wxALIGN_CENTER, 5 );
  //--------------------------------------------------------------

  wxStaticBox *item9 = new wxStaticBox( this, -1, wxT("Monitor") );
  wxStaticBoxSizer *item10 = new wxStaticBoxSizer( item9, wxHORIZONTAL );

  m_check_monitor = new wxCheckBox( this, ID_CHECKBOX_PID, wxT("Particle ID"), wxDefaultPosition, wxDefaultSize, 0 );
  item10->Add( m_check_monitor, 0, wxALIGN_CENTER, 5 );

  m_spin_monitor = new wxSpinCtrl( this, ID_SPINCTRL_MONITOR, wxT("0"), wxDefaultPosition, wxSize(60,-1), 0, 0, 100, 0 );
  item10->Add( m_spin_monitor, 0, wxALIGN_CENTER, 5 );

  item0->Add( item10, 0, wxALIGN_CENTER, 5 );

  wxButton *item30 = new wxButton( this, ID_BUTTON_FLIP, wxT("Flip Normals"), wxDefaultPosition, wxDefaultSize , 0 );
  item0->Add( item30, 0, wxALIGN_CENTER, 5 );
  //--------------------------------------------------------------
  
  SetSizer( item0 ) ;

  SetAutoLayout (true);
	Layout();
}
//_____________________________________________________________________________


void wxRCollisionScene::OnShowMesh( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->ShowMesh( m_check_mesh->GetValue() );
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


void wxRCollisionScene::OnShowOctree( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->ShowOctree( m_check_octree->GetValue() );
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


void wxRCollisionScene::OnShowParticles( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->ShowParticles( m_check_particles->GetValue() );
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


void wxRCollisionScene::OnShowNode( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->ShowNode( m_check_node->GetValue() );
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


void wxRCollisionScene::OnShowTrigs( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->ShowTrigs( m_check_trigs->GetValue() );
    opengl->Redraw();
  }
}
//_____________________________________________________________________________

void wxRCollisionScene::SetValue( bool mesh , bool octree , bool particles , 
                                 bool node , bool trigs )
//-----------------------------------------------------------------------------
{ 
  m_check_mesh->SetValue(mesh);
  m_check_octree->SetValue(octree);
  m_check_particles->SetValue(particles);
  m_check_node->SetValue(node);
  m_check_trigs->SetValue(trigs);
}
//_____________________________________________________________________________




void wxRCollisionScene::SetMonitor( const int& id , const int& max , bool show )
//-----------------------------------------------------------------------------
{ 
  if( max >= 0 ) m_spin_monitor->SetRange( 0 , max );
  else m_spin_monitor->SetRange( 0 , 0 );
  m_spin_monitor->SetValue( id );
  m_check_monitor->SetValue( show );
}
//_____________________________________________________________________________


void wxRCollisionScene::OnMonitor( wxSpinEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->SetMonitor( m_spin_monitor->GetValue() );
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


void wxRCollisionScene::OnShowMonitor( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->ShowMonitor(m_check_monitor->GetValue() ) ;
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


void wxRCollisionScene::OnFlipNormals( wxCommandEvent& WXUNUSED(event) )
//-----------------------------------------------------------------------------
{ 
  wxRCollisionGLCanvas* opengl = (wxRCollisionGLCanvas*)(m_parent->GetActiveGLCanvas());  
  if( opengl )
  {
    opengl->FlipNormal();
    opengl->Redraw();
  }
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//_____________________________________________________________________________