// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


//#ifndef __BASE_H // Make sure to only declare these classes once
//#define __BASE_H
  class MainApp: public wxApp // MainApp is the class for our application
  {
  // MainApp just acts as a container for the window,
  public: // or frame in MainFrame
    virtual bool OnInit();
  };

  class MainFrame: public wxFrame // MainFrame is the class for our window,
  {
  // It contains the window and all objects in it
  public:
    MainFrame( const wxString &title, const wxPoint &pos, const wxSize &size );
    wxButton *HelloWorld;
    void OnExit( wxCommandEvent& event );

    DECLARE_EVENT_TABLE();
  };

  enum
  {
    BUTTON_Hello = wxID_HIGHEST + 1 // declares an id which will be used to call our button
  };

//#endif


  //#include "base.h"

  IMPLEMENT_APP(MainApp) // Initializes the MainApp class and tells our program
  // to run it
  bool MainApp::OnInit()
  {
    // Create an instance of our frame, or window
    MainFrame *MainWin = new MainFrame(_T("Hello World!"), wxPoint(1, 1), wxSize(300,200));
    MainWin->Show(TRUE); // show the window
    SetTopWindow(MainWin); // and finally, set it as the main window

   return TRUE;
  }

  BEGIN_EVENT_TABLE ( MainFrame, wxFrame )
    EVT_BUTTON ( BUTTON_Hello, MainFrame::OnExit ) // Tell the OS to run MainFrame::OnExit when
  END_EVENT_TABLE() // The button is pressed

  MainFrame::MainFrame(const wxString &title, const wxPoint &pos, const wxSize
    &size): wxFrame((wxFrame*)NULL,  - 1, title, pos, size)
  {
    HelloWorld = new wxButton(this, BUTTON_Hello, _T("Hello World"),
      // shows a button on this window
    wxDefaultPosition, wxDefaultSize, 0); // with the text "hello World"
  }

  void MainFrame::OnExit( wxCommandEvent& event )
  {
    Close(TRUE);
  }
