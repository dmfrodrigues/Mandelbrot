#include "FractalFrame.h"

#include "menuicons.h"

///Event enumeration
enum{
    ID_PRINTSCREEN    = 1,
    ID_HDPRINTSCREEN  = 2
};
///Constructor
FractalFrame::FractalFrame():wxFrame(nullptr, wxID_ANY, "Mandelbrot set plotter"){
    ///Menu
    wxMenu* menuFile      = new wxMenu;
    wxMenuItem* menuItem_Printscreen   = new wxMenuItem(menuFile, ID_PRINTSCREEN  , wxT("Save printscreen\tCtrl+S"));
                menuItem_Printscreen  ->SetBitmap(FILESAVEAS);
    wxMenuItem* menuItem_HDPrintscreen = new wxMenuItem(menuFile, ID_HDPRINTSCREEN, wxT("Save HD printscreen"));
                menuItem_HDPrintscreen->SetBitmap(FILESAVEAS);
    menuFile->Append(menuItem_Printscreen  );
    menuFile->Append(menuItem_HDPrintscreen);

    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "File");
    this->SetMenuBar(menuBar);
    ///Panels
    fractalPanel_ = new FractalPanel(this, wxSize(1150, 500));
    infoPanel_    = new    InfoPanel(this);
    ///Sizers
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(fractalPanel_, 1, wxALL | wxEXPAND, 5);
    sizer->Add(   infoPanel_, 0, wxLEFT | wxBOTTOM | wxRIGHT | wxEXPAND, 7);
    sizer->SetSizeHints(this);
    this->SetSizer(sizer);

    this->SetBackgroundColour(infoPanel_->GetBackgroundColour());
}
/*
void FractalFrame::OnPrintscreen  (wxCommandEvent& event) {fractalPanel_->OnPrintscreen  ();}
void FractalFrame::OnHDPrintscreen(wxCommandEvent& event) {fractalPanel_->OnHDPrintscreen();}
*/
///MACROS - redirect events to functions
wxBEGIN_EVENT_TABLE(FractalFrame, wxFrame)
/*
    EVT_MENU(ID_PRINTSCREEN  , FractalFrame::OnPrintscreen  )
    EVT_MENU(ID_HDPRINTSCREEN, FractalFrame::OnHDPrintscreen)
*/
wxEND_EVENT_TABLE()
