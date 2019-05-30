#ifndef INFOPANEL_H_INCLUDED
#define INFOPANEL_H_INCLUDED

class FractalFrame;

class InfoPanel: public wxPanel {
friend class FractalFrame;
private:
    FractalFrame *parent;
    wxTextCtrl *ReCtrl, *ImCtrl, *StepCtrl, *ItCtrl, *TimeCtrl, *DiamCtrl;
    InfoPanel(FractalFrame *p);
    void Update(const FractalBitmap::ComplexNum& MousePosC, const FractalBitmap::ComplexT& step      ,
                const FractalBitmap::IterationT& numIt    , const long double& secPerIt, const FractalBitmap::ComplexT& W);

    wxDECLARE_EVENT_TABLE();
};

#endif
