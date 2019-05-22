#ifndef INFOPANEL_H_INCLUDED
#define INFOPANEL_H_INCLUDED

class FractalFrame;

class InfoPanel: public wxPanel {
friend class FractalFrame;
private:
    FractalFrame *parent;
    wxTextCtrl *ReCtrl, *ImCtrl, *ZoomCtrl, *ItCtrl, *TimeCtrl, *DiamCtrl;
    InfoPanel(FractalFrame *p);
    void Update(const mb::ComplexNum& MousePosC, const mb::ComplexT& zoom      ,
                const mb::IterationT& numIt    , const long double& secPerIt, const mb::ComplexT& W);

    wxDECLARE_EVENT_TABLE();
};

#endif
