#ifndef HDPRINTSCREENDIALOG_H_INCLUDED
#define HDPRINTSCREENDIALOG_H_INCLUDED

#include <wx/wx.h>
#include "Mandelbrot.h"

class HDPrintscreenDialog: public wxDialog {
public:
    HDPrintscreenDialog(wxWindow *p, FractalBitmap::ComplexNum *c, FractalBitmap::ComplexT *z, wxSize *s, FractalBitmap::IterationT *it);
private:
    ///================================
    ///VARIABLES
    wxTextCtrl *ReCtrl, *ImCtrl, *StepCtrl, *SzXCtrl, *SzYCtrl, *ItCtrl;
    FractalBitmap::ComplexNum *center;
    FractalBitmap::ComplexT *step;
    wxSize *sz;
    FractalBitmap::IterationT* numIt;
    ///================================
    ///BUTTONS AND CALLBACKS
    wxButton*     OKBtn; void OnOK    (wxCommandEvent& event);
    wxButton* CancelBtn; void OnCancel(wxCommandEvent& event);
    ///================================
    ///EVENT TABLE
    DECLARE_EVENT_TABLE()
};

#endif // HDPRINTSCREENDIALOG_H_INCLUDED
