#ifndef FRACTALFRAME_H_INCLUDED
#define FRACTALFRAME_H_INCLUDED

#include <bits/stdc++.h>
#include <wx/wx.h>
#include "Mandelbrot.h"

const mb::StepT    FractalHeight = 2.5L;        ///vertical height of the fractal at zoom==1

///=========================================================
///FRACTALFRAME
class FractalFrame;
class FractalPanel;
class InfoPanel;

class FractalFrame: public wxFrame {
public:
    FractalFrame();
private:
    FractalPanel* fpanel;
    InfoPanel*    ipanel;

    std::thread *fthread;
    void Run_fthread();

    mb *f;

    wxDECLARE_EVENT_TABLE();
};

class FractalPanel: public wxPanel {
public:
    FractalPanel(FractalFrame* p, wxSize s);
private:
    FractalFrame    *parent;

    wxDECLARE_EVENT_TABLE();
};

class InfoPanel: public wxPanel {
friend class FractalFrame;
private:
    FractalFrame *parent;
    wxTextCtrl *ReCtrl, *ImCtrl, *ZoomCtrl, *ItCtrl, *TimeCtrl, *DiamCtrl;
    InfoPanel(FractalFrame *p);
    void Update(const mb::ComplexNum& MousePosC, const mb::ZoomT& zoom      ,
                const mb::IterationT& numIt    , const long double& secPerIt, const mb::StepT& W);

    wxDECLARE_EVENT_TABLE();
};

#endif
