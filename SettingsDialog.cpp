#include "SettingsDialog.h"
#include "myConversions.h"

///Constructor
SettingsDialog::SettingsDialog(wxWindow* parent, mb::ComplexNum* center, mb::ZoomT* zoom)
                                            :wxDialog(parent, wxID_ANY, "Change Settings"),
                                             center_(center), zoom_(zoom){
    ///Text
    wxStaticText*   ReStat  = new wxStaticText(this, wxID_ANY,               "Real part:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                    ReCtrl_ = new wxTextCtrl  (this, wxID_ANY, float2str(center_->Re, 20), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT);
    wxStaticText*   ImStat  = new wxStaticText(this, wxID_ANY,          "Imaginary part:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                    ImCtrl_ = new wxTextCtrl  (this, wxID_ANY, float2str(center_->Im, 20), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT);
    wxStaticText* ZoomStat  = new wxStaticText(this, wxID_ANY,                    "Zoom:", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
                  ZoomCtrl_ = new wxTextCtrl  (this, wxID_ANY,      float2str(*zoom_,  8), wxDefaultPosition, wxSize(250,27),  wxALIGN_LEFT);
    ///Text sizer -> grid
    wxFlexGridSizer* txtSizer = new wxFlexGridSizer(3,2,5,5);
    txtSizer->Add(  ReStat, 0, wxEXPAND, 0); txtSizer->Add(  ReCtrl_, 0, 0, 0);
    txtSizer->Add(  ImStat, 0, wxEXPAND, 0); txtSizer->Add(  ImCtrl_, 0, 0, 0);
    txtSizer->Add(ZoomStat, 0, wxEXPAND, 0); txtSizer->Add(ZoomCtrl_, 0, 0, 0);
    ///Buttons
        OKButton_ = new wxButton(this, wxID_OK,         "OK");
    CancelButton_ = new wxButton(this, wxID_CANCEL, "Cancel");
    ///Buttons sizer -> wxBoxSizer(wxHORIZONTAL)
    wxBoxSizer* btnSizer = new wxBoxSizer(wxHORIZONTAL);
    btnSizer->Add(OKButton_, 1, wxALL, 5); btnSizer->Add(CancelButton_, 1, wxALL, 5);
    ///Global sizer
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
    sizer->Add(txtSizer, 0, wxLEFT | wxTOP | wxRIGHT, 15);
    sizer->Add(btnSizer, 0,   wxALIGN_CENTER | wxALL, 5);

    sizer->SetSizeHints(this);
    this->SetSizer(sizer);
}
void SettingsDialog::OnOK(wxCommandEvent& event){
    *center_ = {str2float<mb::ComplexT>(  ReCtrl_->GetLineText(0).ToStdString()),
                str2float<mb::ComplexT>(  ImCtrl_->GetLineText(0).ToStdString())};
    *zoom_   =  str2float<mb::ZoomT>   (ZoomCtrl_->GetLineText(0).ToStdString());
    this->Destroy();
}
void SettingsDialog::OnCancel(wxCommandEvent& event){
    this->Destroy();
}

BEGIN_EVENT_TABLE(SettingsDialog, wxDialog)
    EVT_BUTTON(wxID_OK,     SettingsDialog::OnOK    )
    EVT_BUTTON(wxID_CANCEL, SettingsDialog::OnCancel)
END_EVENT_TABLE()
