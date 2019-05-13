#ifndef HDPRINTSCREENDIALOG_H_INCLUDED
#define HDPRINTSCREENDIALOG_H_INCLUDED

#include <wx/wx.h>
#include "Mandelbrot.h"

class HDPrintscreenDialog: public wxDialog {
public:
    HDPrintscreenDialog(wxWindow* parent, mb::ComplexNum* center, mb::ZoomT* zoom, wxSize* sz, mb::IterationT* it);
private:
    ///================================
    ///VARIABLES
    wxTextCtrl*   ReCtrl_;
    wxTextCtrl*   ImCtrl_; mb::ComplexNum* center_;
    wxTextCtrl* ZoomCtrl_; mb::ZoomT*        zoom_;
    wxTextCtrl*  SzXCtrl_;
    wxTextCtrl*  SzYCtrl_; wxSize*         sz_;
    wxTextCtrl*   ItCtrl_; mb::IterationT*     it_;
    ///================================
    ///BUTTONS AND CALLBACKS
    wxButton*     OKButton_; void OnOK    (wxCommandEvent& event);
    wxButton* CancelButton_; void OnCancel(wxCommandEvent& event);
    ///================================
    ///EVENT TABLE
    DECLARE_EVENT_TABLE()
};

#endif // HDPRINTSCREENDIALOG_H_INCLUDED
