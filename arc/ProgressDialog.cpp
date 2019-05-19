#include "ProgressDialog.h"
#include "myConversions.h"  ///Conversions

ProgressDialog::ProgressDialog(wxWindow* parent, int maxProgress, mb::ComplexNum center, mb::ZoomT zoom, wxSize sz)
                              :wxDialog(parent, wxID_ANY, "Please wait..."){
    ///LAYOUT
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(3,1,10,0);
    /**Grid sizer*/{
        ///PROGRESS
        staticProgress_ = new wxStaticText(this, wxID_ANY, "Creating bitmap (0%)...", wxDefaultPosition, wxDefaultSize, wxALIGN_LEFT);
        gridSizer->Add(staticProgress_ , 0, wxEXPAND);
        ///GAUGE
        gauge_ = new wxGauge(this, wxID_ANY, maxProgress, wxDefaultPosition, wxSize(400,22));
        gridSizer->Add(gauge_, 0, wxEXPAND);
        ///SETTINGS
        collPane_ = new wxCollapsiblePane(this, wxID_ANY, "Settings:");
        gridSizer->Add(collPane_, 0, wxEXPAND);
        /**Settings*/{
            wxWindow* collWin = collPane_->GetPane();
            ///Stuff
            wxStaticText*   ReStat = new wxStaticText(collWin, wxID_ANY,                               "Real part:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
            wxTextCtrl*     ReCtrl = new wxTextCtrl  (collWin, wxID_ANY,                   float2str(center.Re, 20), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT); ReCtrl->SetEditable(false);
            wxStaticText*   ImStat = new wxStaticText(collWin, wxID_ANY,                          "Imaginary part:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
            wxTextCtrl*     ImCtrl = new wxTextCtrl  (collWin, wxID_ANY,                   float2str(center.Im, 20), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT); ImCtrl->SetEditable(false);
            wxStaticText* ZoomStat = new wxStaticText(collWin, wxID_ANY,                                    "Zoom:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
            wxTextCtrl*   ZoomCtrl = new wxTextCtrl  (collWin, wxID_ANY,     mysprintf(( long double)zoom, "%.8lg"), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT); ZoomCtrl->SetEditable(false);
            wxStaticText*  Sz1Stat = new wxStaticText(collWin, wxID_ANY,                              "Dimensions:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
            wxTextCtrl*    SzXCtrl = new wxTextCtrl  (collWin, wxID_ANY,     mysprintf((unsigned int)sz.x,    "%u"), wxDefaultPosition, wxSize( 70,27),  wxALIGN_LEFT); SzXCtrl->SetEditable(false);
            wxStaticText*  Sz2Stat = new wxStaticText(collWin, wxID_ANY,                                      " x ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
            wxTextCtrl*    SzYCtrl = new wxTextCtrl  (collWin, wxID_ANY,     mysprintf((unsigned int)sz.y,    "%u"), wxDefaultPosition, wxSize( 70,27),  wxALIGN_LEFT); SzYCtrl->SetEditable(false);
            wxStaticText*   ItStat = new wxStaticText(collWin, wxID_ANY,                    "Number of iterations:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
            wxTextCtrl*     ItCtrl = new wxTextCtrl  (collWin, wxID_ANY, mysprintf((unsigned int)maxProgress, "%u"), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT); ItCtrl->SetEditable(false);
            ///Dimensions sizer
            wxBoxSizer* dimSizer = new wxBoxSizer(wxHORIZONTAL);
            dimSizer->Add(SzXCtrl);
            dimSizer->Add(Sz2Stat);
            dimSizer->Add(SzYCtrl);
            ///Grid sizer
            wxFlexGridSizer* settingsGrid = new wxFlexGridSizer(5,2,5,5);
            settingsGrid->Add(  ReStat,0,wxEXPAND,0); settingsGrid->Add(  ReCtrl);
            settingsGrid->Add(  ImStat,0,wxEXPAND,0); settingsGrid->Add(  ImCtrl);
            settingsGrid->Add(ZoomStat,0,wxEXPAND,0); settingsGrid->Add(ZoomCtrl);
            settingsGrid->Add( Sz1Stat,0,wxEXPAND,0); settingsGrid->Add(dimSizer);
            settingsGrid->Add(  ItStat,0,wxEXPAND,0); settingsGrid->Add(  ItCtrl);

            settingsGrid->SetSizeHints(collWin);
            collWin->SetSizer(settingsGrid);
        }
    }
    ///
    wxBoxSizer* globalSizer = new wxBoxSizer(wxHORIZONTAL);
    globalSizer->Add(gridSizer, 0, wxALL, 20);
    globalSizer->SetSizeHints(this);
    this->SetSizer(globalSizer);
    ///START TIME
    QueryPerformanceFrequency(&freq_);
    QueryPerformanceCounter(&ti_);
}

void ProgressDialog::OnClose(wxCloseEvent& event){
    isClosed_ = true;
}

bool ProgressDialog::UpdateProgress(mb::IterationT numberIt){
    if(isClosed_ || (numberIt > gauge_->GetRange())) return false;
    staticProgress_->SetLabel(mysprintf((int)(100*numberIt/gauge_->GetRange()), "Creating bitmap (%d%%)..."));
    gauge_->SetValue(numberIt);
    this->Update();
    return true;
}

long double ProgressDialog::GetTime() const {
    LARGE_INTEGER tf_;
    QueryPerformanceCounter(&tf_);
    long double time = double(tf_.QuadPart - ti_.QuadPart)/double(freq_.QuadPart);
    return time;
}

wxBEGIN_EVENT_TABLE(ProgressDialog, wxDialog)
    EVT_CLOSE(ProgressDialog::OnClose)
wxEND_EVENT_TABLE()
