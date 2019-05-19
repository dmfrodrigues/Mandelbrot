#include <wx/wx.h>
#include "FractalFrame.h"

///MyApp will be our inherited wxApp
class MyApp: public wxApp{
private:
    FractalFrame* fractalFrame;
public:
    virtual bool OnInit(){
        wxInitAllImageHandlers();
        fractalFrame = new FractalFrame();
        fractalFrame->Show(true);
        return true;
    }
};
///MACRO - "main" function
///creates an instance of MyApp and starts the program
wxIMPLEMENT_APP(MyApp);
