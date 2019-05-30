#include "FractalFrame.h"

#include "myConversions.h"

InfoPanel::InfoPanel(FractalFrame* p):wxPanel(p), parent(p){
    ///

    wxStaticText* ReStat   = new wxStaticText(this, wxID_ANY,                         "Re(c) = ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
    ReCtrl                 = new wxTextCtrl  (this, wxID_ANY,     "+0.11223344556677889900e-123", wxDefaultPosition, wxSize(250,27), wxALIGN_RIGHT);
    ReCtrl                 ->SetEditable(false);
    wxStaticText* ImStat   = new wxStaticText(this, wxID_ANY,                         "Im(c) = ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
    ImCtrl                 = new wxTextCtrl  (this, wxID_ANY,     "-0.00998877665544332211e+321", wxDefaultPosition, wxSize(250,27), wxALIGN_RIGHT);
    ImCtrl                 ->SetEditable(false);

    wxStaticText* StepStat = new wxStaticText(this, wxID_ANY,                           "Step: ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
    StepCtrl               = new wxTextCtrl  (this, wxID_ANY,                   "0.1234567e-123", wxDefaultPosition, wxSize(120,27), wxALIGN_RIGHT);
    StepCtrl               ->SetEditable(false);

    wxStaticText* ItStat   = new wxStaticText(this, wxID_ANY,           "Total no. iterations: ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
    ItCtrl                 = new wxTextCtrl  (this, wxID_ANY,                           "123456", wxDefaultPosition, wxSize(100,27), wxALIGN_RIGHT);
    ItCtrl                 ->SetEditable(false);

    wxStaticText* TimeStat = new wxStaticText(this, wxID_ANY, "Time elapsed per iteration (s): ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
    TimeCtrl               = new wxTextCtrl  (this, wxID_ANY,                   "0.112233445566", wxDefaultPosition, wxSize(130,27), wxALIGN_RIGHT);
    TimeCtrl               ->SetEditable(false);

    wxStaticText* DiamStat = new wxStaticText(this, wxID_ANY,   "Horizontal size of the image: ", wxDefaultPosition,  wxDefaultSize, wxALIGN_RIGHT);
    DiamCtrl               = new wxTextCtrl  (this, wxID_ANY,                 "0.11223344556677", wxDefaultPosition, wxSize(160,27), wxALIGN_RIGHT);
    DiamCtrl               ->SetEditable(false);
/**
    "Re(c) = %s; Zoom: %g; Total iterations: %d;\n"             "Time elapsed per iteration: %.12f s"
    "Im(c) = %s; Horizontal diameter of the image: %g"
*/
    ///Static box sizer (creates rectangle with title)
    wxStaticBoxSizer* StatBoxSizer = new wxStaticBoxSizer(wxVERTICAL, this, "Settings");
    ///1st line
    wxBoxSizer* sizerLine1 = new wxBoxSizer(wxHORIZONTAL);
    sizerLine1->Add(  ReStat, 0, wxLEFT,  3); sizerLine1->Add(  ReCtrl, 0, 0, 0);

    sizerLine1->Add(  ItStat, 0, wxLEFT, 20); sizerLine1->Add(  ItCtrl, 0, 0, 0);
    sizerLine1->Add(TimeStat, 0, wxLEFT, 20); sizerLine1->Add(TimeCtrl, 0, 0, 0);
    ///2nd line
    wxBoxSizer* sizerLine2 = new wxBoxSizer(wxHORIZONTAL);
    sizerLine2->Add(  ImStat, 0, wxLEFT,  3); sizerLine2->Add(  ImCtrl, 0, 0, 0);
    sizerLine2->Add(StepStat, 0, wxLEFT, 20); sizerLine2->Add(StepCtrl, 0, 0, 0);
    sizerLine2->Add(DiamStat, 0, wxLEFT, 20); sizerLine2->Add(DiamCtrl, 0, 0, 0);
    ///Add stuff together
    StatBoxSizer->Add(sizerLine1, 1, wxLEFT | wxBOTTOM | wxRIGHT, 6);
    StatBoxSizer->Add(sizerLine2, 1, wxLEFT | wxBOTTOM | wxRIGHT, 6);
    StatBoxSizer->SetSizeHints(this);
    this->SetSizer(StatBoxSizer);
}

void InfoPanel::Update(const FractalBitmap::ComplexNum& MousePosC, const FractalBitmap::ComplexT& step,
                       const FractalBitmap::IterationT& numIt    , const long double& secPerIt, const FractalBitmap::ComplexT& W){
    ReCtrl  ->ChangeValue(float2str(MousePosC.real(), 20));
    ImCtrl  ->ChangeValue(float2str(MousePosC.imag(), 20));
    StepCtrl->ChangeValue(float2str(step            ,  4));
    ItCtrl  ->ChangeValue(std::to_string(numIt));
    TimeCtrl->ChangeValue(mysprintf((double) secPerIt, "%.12lf"));
    DiamCtrl->ChangeValue(mysprintf((double) W       , "%.6lg" ));
}

BEGIN_EVENT_TABLE(InfoPanel, wxPanel)
END_EVENT_TABLE()
