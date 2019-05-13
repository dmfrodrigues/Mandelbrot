#ifndef PROGRESSDIALOG_H_INCLUDE
#define PROGRESSDIALOG_H_INCLUDE

#include <wx/wx.h>
#include <wx/collpane.h>    ///Collapsible pane
#include "Mandelbrot.h"

class ProgressDialog: public wxDialog {
public:
    ProgressDialog(wxWindow* parent, int maxProgress, mb::ComplexNum center, mb::ZoomT zoom, wxSize sz);
    bool UpdateProgress(mb::IterationT numberIt);
    long double GetTime() const;
private:
    ///================================
    ///VARIABLES

    bool isClosed_ = false;

    wxStaticText* staticProgress_; ///Static text. Shows progress in percentage
    wxGauge*               gauge_; ///Gauge. Shows progress graphically
    wxCollapsiblePane*  collPane_; ///Collapsible pane. Shows settings when expanded

    LARGE_INTEGER ti_, freq_;      ///Time
    ///================================
    ///CALLBACKS
    void OnClose(wxCloseEvent& event);
    ///================================
    ///EVENT TABLE
    DECLARE_EVENT_TABLE()
};

#endif // PROGRESSDIALOG_H_INCLUDE
