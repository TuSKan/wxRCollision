/**
 * @file    wxRCollision.h
 * @author  Rener Pereira de Castro <rener@mat.puc-rio.br> 
 * @version 
 * @date    
 *
 * @brief
 *
 */
//________________________________________________


#ifndef _WXRCOLLISION_H_
#define _WXRCOLLISION_H_

#ifndef WIN32
#pragma interface
#endif // WIN32

#include <wxRlib.h>
#include <wxRlibFrame.h>

//_____________________________________________________________________________
//Class wxExampleFrame

class wxRCollisionFrame : public wxRlibFrame
//-----------------------------------------------------------------------------
{
public:
	// Constructor & Destructor 
  wxRCollisionFrame(wxWindow *pParent, wxWindowID id, const wxString& title,const wxPoint& pos,
                 const wxSize& size, long style = wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxCLOSE_BOX |
                 wxCAPTION | wxSYSTEM_MENU | wxRESIZE_BORDER | wxCLIP_CHILDREN, const wxString& name = _T("Main Frame") );

  ~wxRCollisionFrame() {}	 
  //------------------------------------------------------
  //Virtusl Members
protected:
  void CreateCtrls();

  //------------------------------------------------------
  //Members
protected:
  void OnOpen(wxCommandEvent& WXUNUSED(event));
  void OnShowScene(wxCommandEvent& WXUNUSED(event));
  void OnShowCtrls(wxCommandEvent& WXUNUSED(event));
  void OnShowInit(wxCommandEvent& WXUNUSED(event));
  //------------------------------------------------------
  //Set and Get Data
public:

  //------------------------------------------------------
  //Elements
private:
  //m_

  //------------------------------------------------------
  DECLARE_EVENT_TABLE()
	//-----------------------------------------------------------------------------
};
//_____________________________________________________________________________


class wxRCollisionApp : public wxRlibApp 
//-----------------------------------------------------------------------------
{
protected:
  void CreateFrame();  
};
//_____________________________________________________________________________


//_____________________________________________________________________________
//_____________________________________________________________________________
#endif  //  _       _H_
