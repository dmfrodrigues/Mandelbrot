#include "FractalFrame.h"

#include "myConversions.h"

FractalFrame::InfoPanel::InfoPanel(FractalFrame* parent):wxPanel(parent), parent_(parent){
    ///
    wxStaticText* ReStat   = new wxStaticText(this, wxID_ANY,                         "Re(c) = ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
    ReCtrl_                = new wxTextCtrl  (this, wxID_ANY,     "+0.11223344556677889900e-123", wxDefaultPosition, wxSize(250,27), wxALIGN_RIGHT);
    ReCtrl_                ->SetEditable(false);
    wxStaticText* ImStat   = new wxStaticText(this, wxID_ANY,                         "Im(c) = ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
    ImCtrl_                = new wxTextCtrl  (this, wxID_ANY,     "-0.00998877665544332211e+321", wxDefaultPosition, wxSize(250,27), wxALIGN_RIGHT);
    ImCtrl_                ->SetEditable(false);

    wxStaticText* ZoomStat = new wxStaticText(this, wxID_ANY,                           "Zoom: ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
    ZoomCtrl_              = new wxTextCtrl  (this, wxID_ANY,                   "0.1234567e-123", wxDefaultPosition, wxSize(120,27), wxALIGN_RIGHT);
    ZoomCtrl_              ->SetEditable(false);

    wxStaticText* ItStat   = new wxStaticText(this, wxID_ANY,           "Total no. iterations: ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
    ItCtrl_                = new wxTextCtrl  (this, wxID_ANY,                           "123456", wxDefaultPosition, wxSize(100,27), wxALIGN_RIGHT);
    ItCtrl_                ->SetEditable(false);

    wxStaticText* TimeStat = new wxStaticText(this, wxID_ANY, "Time elapsed per iteration (s): ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
    TimeCtrl_              = new wxTextCtrl  (this, wxID_ANY,                   "0.112233445566", wxDefaultPosition, wxSize(130,27), wxALIGN_RIGHT);
    TimeCtrl_              ->SetEditable(false);

    wxStaticText* DiamStat = new wxStaticText(this, wxID_ANY,   "Horizontal size of the image: ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
    DiamCtrl_              = new wxTextCtrl  (this, wxID_ANY,                 "0.11223344556677", wxDefaultPosition, wxSize(160,27), wxALIGN_RIGHT);
    DiamCtrl_              ->SetEditable(false);
/**
    "Re(c) = %s; Zoom: %g; Total iterations: %d;\n"             "Time elapsed per iteration: %.12f s"
    "Im(c) = %s; Horizontal diameter of the image: %g"
*/
    ///Static box sizer (creates rectangle with title)
    wxStaticBoxSizer* StatBoxSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Settings");
    ///1st line
    wxBoxSizer* sizerLine1 = new wxBoxSizer(wxHORIZONTAL);
    sizerLine1->Add(  ReStat, 0, wxLEFT,  3); sizerLine1->Add(  ReCtrl_, 0, 0, 0);
    sizerLine1->Add(ZoomStat, 0, wxLEFT, 20); sizerLine1->Add(ZoomCtrl_, 0, 0, 0);
    sizerLine1->Add(  ItStat, 0, wxLEFT, 20); sizerLine1->Add(  ItCtrl_, 0, 0, 0);
    sizerLine1->Add(TimeStat, 0, wxLEFT, 20); sizerLine1->Add(TimeCtrl_, 0, 0, 0);
    ///2nd line
    wxBoxSizer* sizerLine2 = new wxBoxSizer(wxHORIZONTAL);
    sizerLine2->Add(  ImStat, 0, wxLEFT,  3); sizerLine2->Add(  ImCtrl_, 0, 0, 0);
    sizerLine2->Add(DiamStat, 0, wxLEFT, 20); sizerLine2->Add(DiamCtrl_, 0, 0, 0);
    ///Add stuff together
    StatBoxSizer->Add(sizerLine1, 1, wxLEFT | wxBOTTOM | wxRIGHT, 6);
    StatBoxSizer->Add(sizerLine2, 1, wxLEFT | wxBOTTOM | wxRIGHT, 6);
    StatBoxSizer->SetSizeHints(this);
    this->SetSizer(StatBoxSizer);
}
void FractalFrame::InfoPanel::SetStatus(const std::vector<mb::ComplexNum>& C, wxSize panelSz, mb::ZoomT zoom, mb::StepT diam,
                                        mb::IterationT totalIt, long double timeIt, wxPoint mousePos){
    ///MousePosition
    if(0 <= mousePos.x && mousePos.x <= panelSz.x &&
       0 <= mousePos.y && mousePos.y <= panelSz.y){
        mb::ComplexNum mouseComplex = C[mousePos.x + mousePos.y * panelSz.x];
        ReCtrl_->ChangeValue(float2str(mouseComplex.Re, 20));
        ImCtrl_->ChangeValue(float2str(mouseComplex.Im, 20));
    }
    ZoomCtrl_->ChangeValue(mysprintf( (long double)   zoom,  "%.8lg"));
    ItCtrl_  ->ChangeValue(mysprintf((unsigned int)totalIt,     "%u"));
    DiamCtrl_->ChangeValue(mysprintf( (long double)   diam,  "%.6lg"));
    TimeCtrl_->ChangeValue(mysprintf( (long double) timeIt, "%.12lf"));
}
BEGIN_EVENT_TABLE(FractalFrame::InfoPanel, wxPanel)

END_EVENT_TABLE()
