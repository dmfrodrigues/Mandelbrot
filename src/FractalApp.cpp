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
        f = new mb(200);
        fractalFrame = new FractalFrame(f);
        fractalFrame->Show(true);
        return true;
    }
    int OnExit(){
        if(f!=NULL) delete f;
    }
};

wxIMPLEMENT_APP(FractalApp);