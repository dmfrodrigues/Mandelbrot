#include <wx/wx.h>
#include "FractalFrame.h"
#include "Mandelbrot.h"

class FractalApp: public wxApp{
private:
    FractalFrame *fractalFrame;
    mb *f;
public:
    virtual bool OnInit(){
        wxInitAllImageHandlers();
        f = new mb(10);
        fractalFrame = new FractalFrame(f);
        fractalFrame->Show(true);
        return true;
    }
    int OnExit(){
        if(f!=NULL) delete f;
        return wxApp::OnExit();
    }
};

wxIMPLEMENT_APP(FractalApp);
