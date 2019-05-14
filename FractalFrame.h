#ifndef FRACTALFRAME_H_INCLUDED
#define FRACTALFRAME_H_INCLUDED

#include <wx/wx.h>

#include "Mandelbrot.h"
#include "myEvents.h"

const mb::StepT    fractalHeight = 2.5L;        ///vertical height of the fractal at zoom==1

///=========================================================
///FRACTALFRAME
class FractalFrame: public wxFrame {
public:
    FractalFrame();
private:
    class           FractalPanel;
    class           InfoPanel;
    FractalPanel*   fractalPanel_;
    InfoPanel*      infoPanel_;
    mb              *mymb;
    wxDECLARE_EVENT_TABLE();
};
///=========================================================
///FRACTALPANEL
class FractalFrame::FractalPanel: public wxPanel, public wxThreadHelper {
    ///================================
    ///FRIENDS
    friend FractalFrame;
private:
    ///================================
    ///CONSTRUCTOR
    FractalPanel(FractalFrame* parent, wxSize sz);
    ///================================
    ///VARIABLES
    FractalFrame* const parent_;

    wxMenu* popupMenu_;
    ///================================
    ///FUNCTIONS
    ///Worker thread
    virtual wxThread::ExitCode Entry();
    ///Change settings
    ///Painting
    void OnPaintEvent(wxPaintEvent& evt);   ///if a wxPaintEvent is triggered, this function captures it
    void paintNow(wxPoint mousePos); ///if I want to paint, I call this function
    void render(wxDC& dc);                  ///the two above functions depend on this one
};
///=========================================================
///INFOPANEL
class FractalFrame::InfoPanel: public wxPanel {
    ///================================
    ///FRIENDS
    friend FractalFrame;
private:
    ///================================
    ///CONSTRUCTOR
    InfoPanel(FractalFrame* parent);
    ///================================
    ///VARIABLES
    FractalFrame* const parent_;

    wxTextCtrl *ReCtrl_, *ZoomCtrl_, *ItCtrl_, *TimeCtrl_;
    wxTextCtrl *ImCtrl_, *DiamCtrl_;
    ///================================
    ///FUNCTIONS
    void SetStatus(const std::vector<mb::ComplexNum>& C, wxSize panelSz, mb::ZoomT zoom, mb::StepT diam,
                   mb::IterationT totalIt, long double timeIt, wxPoint mousePos);
    ///================================
    ///EVENT TABLE
    DECLARE_EVENT_TABLE()
};

#endif // FRACTALFRAME_H_INCLUDED
