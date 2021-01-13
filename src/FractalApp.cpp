#include "FractalFrame.h"
#include "Mandelbrot.h"
#include <iostream>

class FractalApp: public wxApp{
private:
    FractalFrame *fractalFrame;
    mb *f;
public:
    virtual bool OnInit(){
        std::cerr << "+FractalApp::OnInit" << std::endl;
        wxInitAllImageHandlers();
        f = new mb(200);
        fractalFrame = new FractalFrame(f);
        fractalFrame->Show(true);
        std::cerr << "-FractalApp::OnInit" << std::endl;
        return true;
    }
    int OnExit(){
        std::cerr << "+FractalApp::OnExit" << std::endl;
        delete f;
        return wxApp::OnExit();
    }
};

wxIMPLEMENT_APP(FractalApp);
