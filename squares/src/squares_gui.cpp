
#include "utilities.h"
#include "squares_gui.h"

#include <string>
#include <cstring>
#include <vector>
#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>

#include <wx/wxprec.h>
#include <wx/image.h>
#include <wx/sizer.h>

// For compilers that support precompilation, includes "wx/wx.h"
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif


#define POS_SAMPLES_DIR "positive_samples/"
#define NEG_SAMPLES_DIR "negative_samples/"
#define OUTPUT_FOLDER "output_images/"

using namespace std;
using namespace utilities;



wxBEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(ID_Hello,   MyFrame::OnHello)
    EVT_MENU(wxID_EXIT,  MyFrame::OnExit)
    EVT_MENU(wxID_ABOUT, MyFrame::OnAbout)
    EVT_BUTTON(BUTTON_isTree, MyFrame::OnTreeClick)
    EVT_BUTTON(BUTTON_notTree, MyFrame::OnNotTreeClick)
    EVT_CLOSE(MyFrame::OnClose)

wxEND_EVENT_TABLE()
wxIMPLEMENT_APP(MyApp);

bool MyApp::OnInit()
{
     wxInitAllImageHandlers();

    MyFrame *frame = new MyFrame( "Samples selection", wxPoint(50, 50), wxSize(450, 340));
    frame->Show( true );
    return true;
}


MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
        : wxFrame(NULL, wxID_ANY, title, pos, size, wxMINIMIZE_BOX | wxMAXIMIZE_BOX | wxSYSTEM_MENU | wxCAPTION | wxCLOSE_BOX | wxCLIP_CHILDREN)
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
    SetStatusText( "Welcome to Samples Selection!" );

    // initialize panel
    panel = new wxPanel(this, wxID_ANY);

    // insert buttons
    isTree = new wxButton(panel, BUTTON_isTree, _T("It's a tree!!!!!"), wxPoint(0,275), wxSize(225,40), 0  );
    notTree = new wxButton(panel, BUTTON_notTree, _T("It's not a tree"), wxPoint(225,275), wxSize(225,40), 0  );

    // get images' names
    images_to_show = getFilesList(OUTPUT_FOLDER);
    if(images_to_show.size() < 1)
    {
        cout << "Could not retrieve images to show. Be sure that ./squares has been executed and output_images folder has been created. "
                "Check permissions." << endl << endl;

        exit(-1);
    }

    // set images' index
    check_image = 0;

    // prepare initial image
    wxImage* temporary_image = new wxImage(string(OUTPUT_FOLDER) + images_to_show[check_image], wxBITMAP_TYPE_JPEG);
    temporary_image->Rescale(100, 100);
    image = new wxStaticBitmap(panel, 0, wxBitmap(*temporary_image), wxPoint(175, 110), wxSize(100, 100));

    // delete directories from previous executions
    remove_directory(POS_SAMPLES_DIR);
    remove_directory(NEG_SAMPLES_DIR);

    // recreate directories
    mkdir(POS_SAMPLES_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
    mkdir(NEG_SAMPLES_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

}

void MyFrame::OnExit(wxCommandEvent& event)
{
    wxMessageDialog *dial = new wxMessageDialog(NULL,
          wxT("Are you sure to quit? Process is not complete"), wxT("Attention"),
          wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

      if(dial->ShowModal() == wxID_YES)
         exit(-1);
}

void MyFrame::OnClose(wxCloseEvent &event)
{
    wxMessageDialog *dial = new wxMessageDialog(NULL,
          wxT("Are you sure to quit? Process is not complete"), wxT("Attention"),
          wxYES_NO | wxNO_DEFAULT | wxICON_QUESTION);

      if(dial->ShowModal() == wxID_YES)
          exit(-1);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Click left button if image contains a tree. Click right image, otherwise.",
                  "Samples Selection", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello from Sample Selection!");
}

void MyFrame::OnTreeClick(wxCommandEvent& event)
{
    // open input stream
    string concat_input = OUTPUT_FOLDER + images_to_show[check_image];
    ifstream source(concat_input.c_str(), ios::binary);

    // open output stream
    string concat_output = POS_SAMPLES_DIR + images_to_show[check_image];
    ofstream dest(concat_output.c_str(), ios::binary);

    // declare iterators
    istreambuf_iterator<char> begin_source(source);
    istreambuf_iterator<char> end_source;
    ostreambuf_iterator<char> begin_dest(dest);

    // copy file
    copy(begin_source, end_source, begin_dest);

    // close streams
    source.close();
    dest.close();

    // check whether image needs to be updated
    if(check_image < images_to_show.size() - 1)
    {
        // update image
        wxImage* next_image = new wxImage(string(OUTPUT_FOLDER) + images_to_show[check_image+1], wxBITMAP_TYPE_JPEG);
        next_image->Rescale(100, 100);
        image = new wxStaticBitmap(panel, 0, wxBitmap(*next_image), wxPoint(175, 110), wxSize(100, 100));
        check_image++;
    }

    else
    {
        // close interface
        cout << "Images written to correspondent folders" << endl << endl;
        Close(true);
    }
}

void MyFrame::OnNotTreeClick(wxCommandEvent& event)
{
    // open input stream
    string concat_input = OUTPUT_FOLDER + images_to_show[check_image];
    ifstream source(concat_input.c_str(), ios::binary);

    // open output stream
    string concat_output = NEG_SAMPLES_DIR + images_to_show[check_image];
    ofstream dest(concat_output.c_str(), ios::binary);

    // declare iterators
    istreambuf_iterator<char> begin_source(source);
    istreambuf_iterator<char> end_source;
    ostreambuf_iterator<char> begin_dest(dest);

    // copy file
    copy(begin_source, end_source, begin_dest);

    // close streams
    source.close();
    dest.close();

    // check whether image needs to be updated
    if(check_image < images_to_show.size() - 1)
    {
        // update image
        wxImage* next_image = new wxImage(string(OUTPUT_FOLDER) + images_to_show[check_image+1], wxBITMAP_TYPE_JPEG);
        next_image->Rescale(100, 100);
        image = new wxStaticBitmap(panel, 0, wxBitmap(*next_image), wxPoint(175, 110), wxSize(100, 100));
        check_image++;
    }

    else
    {
        // close interface
        cout << "Images written to correspondent folders" << endl << endl;
        Close(true);
    }
}
