#include "HDPrintscreenDialog.h"
#include "myConversions.h"

///Constructor
HDPrintscreenDialog::HDPrintscreenDialog (wxWindow* parent, mb::ComplexNum* center, mb::ZoomT* zoom, wxSize* sz, mb::IterationT* it)
    :wxDialog(parent, wxID_ANY, "Set printscreen settings"), center_(center), zoom_(zoom), sz_(sz), it_(it){
    ///Text
    wxStaticText*   ReStat  = new wxStaticText(this, wxID_ANY,                               "Real part:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                    ReCtrl_ = new wxTextCtrl  (this, wxID_ANY,                 float2str(center_->Re, 20), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT);
    wxStaticText*   ImStat  = new wxStaticText(this, wxID_ANY,                          "Imaginary part:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                    ImCtrl_ = new wxTextCtrl  (this, wxID_ANY,                 float2str(center_->Im, 20), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT);
    wxStaticText* ZoomStat  = new wxStaticText(this, wxID_ANY,                                    "Zoom:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                  ZoomCtrl_ = new wxTextCtrl  (this, wxID_ANY, mysprintf(( long double)(*zoom_), "%.8lg"), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT);
    wxStaticText*  Sz1Stat  = new wxStaticText(this, wxID_ANY,                        "Image dimensions:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                   SzXCtrl_ = new wxTextCtrl  (this, wxID_ANY, mysprintf((unsigned int)(sz_->x),    "%u"), wxDefaultPosition, wxSize( 70,27),  wxALIGN_LEFT);
    wxStaticText*  Sz2Stat  = new wxStaticText(this, wxID_ANY,                                      " x ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                   SzYCtrl_ = new wxTextCtrl  (this, wxID_ANY, mysprintf((unsigned int)(sz_->y),    "%u"), wxDefaultPosition, wxSize( 70,27),  wxALIGN_LEFT);
    wxStaticText*   ItStat  = new wxStaticText(this, wxID_ANY,                    "Number of iterations:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                    ItCtrl_ = new wxTextCtrl  (this, wxID_ANY, mysprintf((unsigned int)(  *it_),    "%u"), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT);
    ///Dimensions sizer
    wxBoxSizer* dimensionsSizer = new wxBoxSizer(wxHORIZONTAL);
    dimensionsSizer->Add(SzXCtrl_, 0, 0, 0);
    dimensionsSizer->Add(Sz2Stat , 0, 0, 0);
    dimensionsSizer->Add(SzYCtrl_, 0, 0, 0);
    ///Text sizer -> grid
    wxFlexGridSizer* gridSizer = new wxFlexGridSizer(5,2,5,5);
    gridSizer->Add(  ReStat, 0, wxEXPAND, 0); gridSizer->Add(  ReCtrl_, 0, 0, 0);
    gridSizer->Add(  ImStat, 0, wxEXPAND, 0); gridSizer->Add(  ImCtrl_, 0, 0, 0);
    gridSizer->Add(ZoomStat, 0, wxEXPAND, 0); gridSizer->Add(ZoomCtrl_, 0, 0, 0);
    gridSizer->Add( Sz1Stat, 0, wxEXPAND, 0); gridSizer->Add( dimensionsSizer, 0, 0, 0);
    gridSizer->Add(  ItStat, 0, wxEXPAND, 0); gridSizer->Add(  ItCtrl_, 0, 0, 0);
    ///Static text
    wxStaticBoxSizer* staticBoxSizer = new wxStaticBoxSizer(wxHORIZONTAL, this, "Settings");
    staticBoxSizer->Add(gridSizer, 0, wxALL, 5);
    ///Buttons
        OKButton_ = new wxButton(this, wxID_OK,         "OK");
    CancelButton_ = new wxButton(this, wxID_CANCEL, "Cancel");
    ///Buttons sizer -> wxBoxSizer(wxHORIZONTAL)
    wxBoxSizer* buttonSizer = new wxBoxSizer(wxHORIZONTAL);
    buttonSizer->Add(OKButton_, 1, wxALL, 5); buttonSizer->Add(CancelButton_, 1, wxALL, 5);
    ///Global sizer
    wxBoxSizer* globalSizer = new wxBoxSizer(wxVERTICAL);
    globalSizer->Add(staticBoxSizer, 0, wxLEFT | wxTOP | wxRIGHT, 8);
    globalSizer->Add(   buttonSizer, 0,   wxALIGN_CENTER | wxALL, 5);

    globalSizer->SetSizeHints(this);
    this->SetSizer(globalSizer);
}
void HDPrintscreenDialog::OnOK(wxCommandEvent& event){
    *center_ =       {str2float<mb::ComplexT>    (  ReCtrl_->GetLineText(0).ToStdString()),
                      str2float<mb::ComplexT>    (  ImCtrl_->GetLineText(0).ToStdString())};
    *zoom_   =        str2float<mb::ZoomT>       (ZoomCtrl_->GetLineText(0).ToStdString());
    *sz_     = wxSize(str2float<unsigned int>( SzXCtrl_->GetLineText(0).ToStdString()),
                      str2float<unsigned int>( SzYCtrl_->GetLineText(0).ToStdString()));
    *it_     =        str2float<mb::IterationT>  (  ItCtrl_->GetLineText(0).ToStdString());
    this->SetReturnCode(wxID_OK);
    this->Destroy();
}
void HDPrintscreenDialog::OnCancel(wxCommandEvent& event){
    this->SetReturnCode(wxID_CANCEL);
    this->Destroy();
}

BEGIN_EVENT_TABLE(HDPrintscreenDialog, wxDialog)
    EVT_BUTTON(wxID_OK,     HDPrintscreenDialog::OnOK    )
    EVT_BUTTON(wxID_CANCEL, HDPrintscreenDialog::OnCancel)
END_EVENT_TABLE()
