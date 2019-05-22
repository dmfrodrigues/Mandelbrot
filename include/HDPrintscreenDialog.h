#ifndef HDPRINTSCREENDIALOG_H_INCLUDED
#define HDPRINTSCREENDIALOG_H_INCLUDED

#include <wx/wx.h>
#include "Mandelbrot.h"

class HDPrintscreenDialog: public wxDialog {
public:
    HDPrintscreenDialog(wxWindow *p, mb::ComplexNum *c, mb::ComplexT *z, wxSize *s, mb::IterationT *it);
private:
    ///================================
    ///VARIABLES
    wxTextCtrl *ReCtrl, *ImCtrl, *ZoomCtrl, *SzXCtrl, *SzYCtrl, *ItCtrl;
    mb::ComplexNum *center;
    mb::ComplexT *zoom;
    wxSize *sz;
    mb::IterationT* numIt;
    ///================================
    ///BUTTONS AND CALLBACKS
    wxButton*     OKBtn; void OnOK    (wxCommandEvent& event);
    wxButton* CancelBtn; void OnCancel(wxCommandEvent& event);
    ///================================
    ///EVENT TABLE
    DECLARE_EVENT_TABLE()
};

#endif // HDPRINTSCREENDIALOG_H_INCLUDED
