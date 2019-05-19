#ifndef SETTINGSDIALOG_H_INCLUDED
#define SETTINGSDIALOG_H_INCLUDED

#include <wx/wx.h>
#include "Mandelbrot.h"

class SettingsDialog: public wxDialog {
public:
    SettingsDialog(wxWindow* parent, mb::ComplexNum* center, mb::ZoomT* zoom);
private:
    wxTextCtrl*   ReCtrl_;
    wxTextCtrl*   ImCtrl_; mb::ComplexNum* center_;
    wxTextCtrl* ZoomCtrl_; mb::ZoomT*        zoom_;
    ///================================
    ///BUTTONS AND CALLBACKS
    wxButton*     OKButton_; void OnOK    (wxCommandEvent& event);
    wxButton* CancelButton_; void OnCancel(wxCommandEvent& event);
    ///================================
    ///EVENT TABLE
    DECLARE_EVENT_TABLE()
};

#endif // SETTINGSDIALOG_H_INCLUDED
