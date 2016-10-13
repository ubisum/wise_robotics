

// For compilers that support precompilation, includes "wx/wx.h".
#include <wx/wxprec.h>
#include <wx/image.h>
#include <wx/sizer.h>

#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


class MyApp: public wxApp
{
public:
    virtual bool OnInit();
};


class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
    wxButton *isTree;
    wxButton *notTree;
    wxPanel* panel;
    wxImage frame_image;


private:
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnTreeClick(wxCommandEvent& event);
    void OnNotTreeClick(wxCommandEvent& event);
    void OnHello(wxCommandEvent& event);

    wxDECLARE_EVENT_TABLE();
};

enum
{
    ID_Hello = 1,
    BUTTON_isTree = wxID_HIGHEST + 1,
    BUTTON_notTree = wxID_HIGHEST + 2,
};


wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Hello,   MyFrame::OnHello)
    EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_BUTTON(BUTTON_isTree, MyFrame::OnTreeClick)
    EVT_BUTTON(BUTTON_notTree, MyFrame::OnNotTreeClick)
    //EVT_PAINT(wxImagePanel::paintEvent)

wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);




bool MyApp::OnInit()
{
     wxInitAllImageHandlers();
     wxBoxSizer* sizer = new wxBoxSizer(wxHORIZONTAL);

    MyFrame *frame = new MyFrame( "Hello World", wxPoint(50, 50), wxSize(450, 340));
    frame->Show( true );
    return true;
}


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size)
{
    // create file menu
    wxMenu *menuFile = new wxMenu;

    // add a hello item
    menuFile->Append(ID_Hello, "&Hello...\tCtrl-H",
                     "Help string shown in status bar for this menu item");

    // add a separator
    menuFile->AppendSeparator();

    // add an exit item
    menuFile->Append(wxID_EXIT);

    // create another menu
    wxMenu *menuHelp = new wxMenu;

    // add an exit item
    menuHelp->Append(wxID_ABOUT);

    // create a menu bar and append menus
    wxMenuBar *menuBar = new wxMenuBar;
    menuBar->Append( menuFile, "&File" );
    menuBar->Append( menuHelp, "&Help" );

    // insert menu bar
    SetMenuBar( menuBar );

    // create and set menu bar
    CreateStatusBar();
    SetStatusText( "Welcome to wxWidgets!" );

    // initialize panel
    panel = new wxPanel(this, wxID_ANY);

    isTree = new wxButton(panel, BUTTON_isTree, _T("It's a tree!!!!!"), wxPoint(0,275), wxSize(225,40), 0  );
    notTree = new wxButton(panel, BUTTON_notTree, _T("It's not a tree"), wxPoint(225,275), wxSize(225,40), 0  );
    //frame_image.AddHandler(new wxJPEGHandler);
    //frame_image.LoadFile("images/image.jpg", wxBITMAP_TYPE_JPEG);
}
void MyFrame::OnExit(wxCommandEvent& event)
{
    Close( true );
}
void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox( "This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION );
}
void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
}

void MyFrame::OnTreeClick(wxCommandEvent& event)
{

}

void MyFrame::OnNotTreeClick(wxCommandEvent& event)
{

}
