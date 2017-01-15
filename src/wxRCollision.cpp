/**
 * @file    wxRCollision.cpp
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

#include "wxRCollision.h"
#include "wxRCollisionGLCanvas.h"
#include "wxRCollisionScene.h"
#include "wxRCollisionCtrls.h"
#include "wxRCollisionInit.h"

//_____________________________________________________________________________

IMPLEMENT_APP(wxRCollisionApp)


void wxRCollisionApp::CreateFrame()
//-----------------------------------------------------------------------------
{  
  wxRCollisionFrame* m_frame = new wxRCollisionFrame((wxWindow*)NULL,-1,wxT(" wxRCollision "),wxDefaultPosition,wxSize(1000,800));
  m_frame->Centre();
	m_frame->Show(true);
	SetTopWindow(m_frame); 
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//_____________________________________________________________________________


//_____________________________________________________________________________

enum
{ ID_SHOW_SCENE = ID_RLIB_HIGHEST + 1, 
  ID_SHOW_CTRLS ,
  ID_SHOW_INIT ,
};

BEGIN_EVENT_TABLE(wxRCollisionFrame, wxRlibFrame)
  EVT_MENU( ID_SHOW_SCENE , wxRCollisionFrame::OnShowScene )
  EVT_MENU( ID_SHOW_CTRLS , wxRCollisionFrame::OnShowCtrls )
  EVT_MENU( ID_SHOW_INIT  , wxRCollisionFrame::OnShowInit )
END_EVENT_TABLE()

//_____________________________________________________________________________


wxRCollisionFrame::wxRCollisionFrame(wxWindow *pParent, wxWindowID id, const wxString& title,const wxPoint& pos,
                              const wxSize& size, long style , const wxString& name)
: wxRlibFrame(pParent, id, title, pos, size, style, name)
//-----------------------------------------------------------------------------
{  
  CreateCtrls();
}
//_____________________________________________________________________________


void wxRCollisionFrame::CreateCtrls()
//-----------------------------------------------------------------------------
{
  SetShowPane("Material" , false);  
  SetShowPane("Translation" , false);
  SetShowPane("Scaling" , false);  

  menuView->Append( ID_SHOW_CTRLS, wxT("Collision Ctrls") );
  AddPane( new wxRCollisionCtrls(this) , "Ctrls"," Collision Ctrls " , wxSize(180,110) , wxRIGHT );  

  menuView->Append( ID_SHOW_SCENE, wxT("Collision Scene") );
  AddPane( new wxRCollisionScene(this) , "Scene"," Collision Scene " , wxSize(180,100) , wxRIGHT );    

  menuView->Append( ID_SHOW_INIT, wxT("Collision Init") );
  AddPane( new wxRCollisionInit(this) , "Init"," Collision Init " , wxSize(180,130) , wxRIGHT );
  
  m_mgr.Update();
}
//_____________________________________________________________________________


void wxRCollisionFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
//-----------------------------------------------------------------------------
{
  wxFileDialog *filedlg = new wxFileDialog(this,"Choose a ply file","","","*.ply*",
                              wxFD_OPEN|wxFD_FILE_MUST_EXIST|wxFD_CHANGE_DIR,wxDefaultPosition);
  filedlg->ShowModal();

  if( !filedlg->GetFilename().IsEmpty() )
  {
    wxRCollisionGL* frameGL = new wxRCollisionGL(this);
    wxRCollisionGLCanvas* opengl = frameGL->GL();
    if( opengl )  opengl->OpenPly(filedlg->GetFilename().c_str());      
    frameGL->SetTitle(filedlg->GetFilename());
    frameGL->Show(true);
  }
  delete filedlg;
}
//_____________________________________________________________________________


void wxRCollisionFrame::OnShowScene(wxCommandEvent& WXUNUSED(event))
//-----------------------------------------------------------------------------
{
  OnShowPane("Scene");
}
//_____________________________________________________________________________


void wxRCollisionFrame::OnShowCtrls(wxCommandEvent& WXUNUSED(event))
//-----------------------------------------------------------------------------
{
  OnShowPane("Ctrls");
}
//_____________________________________________________________________________


void wxRCollisionFrame::OnShowInit(wxCommandEvent& WXUNUSED(event))
//-----------------------------------------------------------------------------
{
  OnShowPane("Init");
}
//_____________________________________________________________________________


//_____________________________________________________________________________
//_____________________________________________________________________________
