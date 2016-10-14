
#include "utilities.h"

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
//#define OUTPUT_FOLDER "output_images/"
#define OUTPUT_FOLDER "test/"

using namespace std;
using namespace utilities;

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
    wxStaticBitmap *image;


private:
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnTreeClick(wxCommandEvent& event);
    void OnNotTreeClick(wxCommandEvent& event);
    void OnHello(wxCommandEvent& event);

    int check_image;
    vector<string> images_to_show;

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

    // insert buttons
    isTree = new wxButton(panel, BUTTON_isTree, _T("It's a tree!!!!!"), wxPoint(0,275), wxSize(225,40), 0  );
    notTree = new wxButton(panel, BUTTON_notTree, _T("It's not a tree"), wxPoint(225,275), wxSize(225,40), 0  );

    // get images' names
    images_to_show = getFilesList(OUTPUT_FOLDER);
    cout << "Images " << images_to_show.size() << endl << endl;
    if(images_to_show.size() < 1)
    {
        cout << "Could not retrieve images to show. Be sure that ./squares has been executed and output_images folder has been created. "
                "Check permissions." << endl << endl;

        exit(-1);
    }

    // set images' index
    check_image = 0;

    // prepare initial image
    //wxImage* temporary_image = new wxImage("images/trees.jpg", wxBITMAP_TYPE_JPEG);
    wxImage* temporary_image = new wxImage(string(OUTPUT_FOLDER) + images_to_show[check_image], wxBITMAP_TYPE_JPEG);
    temporary_image->Rescale(100, 100);
    image = new wxStaticBitmap(panel, 0, wxBitmap(*temporary_image), wxPoint(175, 110), wxSize(100, 100));
    cout << "error" << endl;
    // delete directories from previous executions
    remove_directory(POS_SAMPLES_DIR);
    remove_directory(NEG_SAMPLES_DIR);

    // recreate directories
    mkdir(POS_SAMPLES_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);
    mkdir(NEG_SAMPLES_DIR, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH | S_IWOTH);

}

void MyFrame::OnExit(wxCommandEvent& event)
{
    Close(true);
}

void MyFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("This is a wxWidgets' Hello world sample",
                  "About Hello World", wxOK | wxICON_INFORMATION);
}

void MyFrame::OnHello(wxCommandEvent& event)
{
    wxLogMessage("Hello world from wxWidgets!");
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
