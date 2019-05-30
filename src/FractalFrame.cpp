#include "FractalFrame.h"

#include "HDPrintscreenDialog.h"

#include <chrono>
#include <fstream>

///Constants
const FractalBitmap::IterationT addIt = 200;
///Event enumeration
enum{
    ID_PRINTSCREEN    = 1,
    ID_HDPRINTSCREEN  = 2
};
///Constructor
FractalFrame::FractalFrame(FractalBitmap *p):wxFrame(nullptr, wxID_ANY, "Mandelbrot set plotter"),f(p){
    /**Menu*/{
        wxMenu* menuFile      = new wxMenu;
        wxMenuItem* menuItem_Printscreen   = new wxMenuItem(menuFile, ID_PRINTSCREEN  , wxT("Save printscreen"));
        wxMenuItem* menuItem_HDPrintscreen = new wxMenuItem(menuFile, ID_HDPRINTSCREEN, wxT("Save HD printscreen"));
        menuFile->Append(menuItem_Printscreen  );
        menuFile->Append(menuItem_HDPrintscreen);

        wxMenuBar* menuBar = new wxMenuBar;
        menuBar->Append(menuFile, "File");
        this->SetMenuBar(menuBar);
    }
    /**Panels*/{
        fpanel = new FractalPanel(this, wxSize(1150, 500));
        ipanel = new InfoPanel   (this);
    }
    /**Background color*/{
        SetBackgroundColour(ipanel->GetBackgroundColour());
    }
    /**Sizers*/{
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(fpanel, 1, wxALL | wxEXPAND, 5);
        sizer->Add(ipanel, 0, wxLEFT | wxBOTTOM | wxRIGHT | wxEXPAND, 7);
        sizer->SetSizeHints(this);
        this->SetSizer(sizer);
    }
    /**Create fractal thread*/{
        if (CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR){
            wxLogError("Could not create main thread"); return;
        }
        if (GetThread()->Run() != wxTHREAD_NO_ERROR){
            wxLogError("Could not run main thread"   ); return;
        }
    }
}

typedef std::chrono::high_resolution_clock hrclock;
wxThread::ExitCode FractalFrame::Entry(){
    /**Create fractal*/{
        f->New(FractalBitmap::ComplexNum(FractalBitmap::ComplexT(-1.375),FractalBitmap::ComplexT(0.0L)),
            FractalHeight*FractalBitmap::ComplexT(1.0L/20.0L/GetSize().y), fpanel->GetSize(), true);
    }
    while(true){
        ///Update the fractal
        auto t1 = hrclock::now();
        f->UpdateMath(addIt);
        auto t2 = hrclock::now();
        ///Update the screen
        wxClientDC dc(fpanel);
        dc.DrawBitmap(*((wxBitmap*)f), 0, 0, true);
        ///Update the InfoPanel
        auto dt = std::chrono::duration<long double>(t2-t1);
        CallAfter(&FractalFrame::UpdateInfoPanel, dt.count()/(long double)addIt);
        ///Process events
        if(!fpanel->is_mouseevt_handled  ){ OnZoomEvent(fpanel->mouseevt);          fpanel->is_mouseevt_handled   = true; }
        if(!fpanel->is_sizeevt_handled   ){ OnSizeEvent();                          fpanel->is_sizeevt_handled    = true; }
        if(!        is_prtscevt_handled  ){ OnPrintscreenEvent();                           is_prtscevt_handled   = true; }
        if(!        is_hdprtscevt_handled){ CallAfter(&FractalFrame::OnHDPrintscreenEvent); is_hdprtscevt_handled = true; }
    }
    return (wxThread::ExitCode)0;
}

void FractalFrame::UpdateInfoPanel(const long double& secPerIt){
    wxPoint p = wxGetMousePosition() - fpanel->GetScreenPosition();
    FractalBitmap::ComplexNum c = f->GetOrigin() + FractalBitmap::ComplexNum(+(FractalBitmap::ComplexT)p.x*f->GetStep(),-(FractalBitmap::ComplexT)p.y*f->GetStep());
    ipanel->Update(c, f->GetStep(), f->GetNumIt(), secPerIt, f->GetHorizontalSize());
}

void FractalFrame::OnZoomEvent(const wxMouseEvent& evt){
    wxPoint p = wxGetMousePosition() - fpanel->GetScreenPosition();
    FractalBitmap::ComplexNum newcenter = f->GetOrigin() + FractalBitmap::ComplexNum(
        FractalBitmap::ComplexT(p.x)*f->GetStep(), FractalBitmap::ComplexT(-p.y)*f->GetStep()
    );
    FractalBitmap::ComplexT newstep = f->GetStep()*FractalBitmap::ComplexT(evt.GetWheelRotation() < 0 ? 3.16227766017L : 1.0L/3.16227766017L);
    f->New(newcenter, newstep, fpanel->GetSize(), true);
}

void FractalFrame::OnSizeEvent(){
    wxPoint p = wxGetMousePosition() - fpanel->GetScreenPosition();
    f->New(f->GetCenter(), f->GetStep(), fpanel->GetSize(), true);
}


void NewImageName(const char* format, char* name){
    const unsigned long N = 100000;
    for(unsigned long i = 0; i < N; ++i){
        sprintf(name, format, i);
        if(!std::ifstream(name)) return;
    }
    throw std::logic_error("no available names");
}
void FractalFrame::OnPrintscreenEvent(){
    char new_path[256];
    NewImageName(".\\Printscreens\\Image_%04d.png", new_path);
    if(f->SaveFile(new_path, wxBITMAP_TYPE_PNG))
        wxLogMessage("Printscreen saved as " + wxString(new_path));
}
void FractalFrame::OnHDPrintscreenEvent(){
    char new_path[256];
    NewImageName(".\\Printscreens\\Image_%04d.png", new_path);

    FractalBitmap::ComplexNum center = f->GetCenter();
    FractalBitmap::ComplexT step = f->GetStep();
    wxSize sz = f->GetSize();
    FractalBitmap::IterationT numIt = f->GetNumIt();
    HDPrintscreenDialog *dialog = new HDPrintscreenDialog(this, &center, &step, &sz, &numIt);
    if(dialog->ShowModal() != wxID_OK) return;
    FractalBitmap *g = f->CreateNew(center, step, sz, true);

    numIt = (numIt/addIt)*addIt + (numIt%addIt? addIt : 0);
    g->UpdateMath(numIt);
    if(g->SaveFile(new_path, wxBITMAP_TYPE_PNG))
        wxLogMessage("Printscreen saved as " + wxString(new_path));
}

///MACROS - redirect events to functions
wxBEGIN_EVENT_TABLE(FractalFrame, wxFrame)
    EVT_MENU(ID_PRINTSCREEN  , FractalFrame::OnPrintscreenCallback  )
    EVT_MENU(ID_HDPRINTSCREEN, FractalFrame::OnHDPrintscreenCallback)
wxEND_EVENT_TABLE()
