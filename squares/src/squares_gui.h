#ifndef SQUARES_GUI_H
#define SQUARES_GUI_H

#include <wx/wxprec.h>
#include <wx/image.h>
#include <wx/sizer.h>

// For compilers that support precompilation, includes "wx/wx.h"
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
    wxStaticBitmap *image;


private:
    void OnExit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    void OnTreeClick(wxCommandEvent& event);
    void OnNotTreeClick(wxCommandEvent& event);
    void OnHello(wxCommandEvent& event);
    void OnClose(wxCloseEvent &event);

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



#endif // SQUARES_GUI_H

