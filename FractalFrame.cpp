#include "FractalFrame.h"

#include "menuicons.h"

///Event enumeration
enum{
    ID_PRINTSCREEN    = 1,
    ID_HDPRINTSCREEN  = 2
};
///Constructor
FractalFrame::FractalFrame():wxFrame(nullptr, wxID_ANY, "Mandelbrot set plotter"){
    /**Menu*/{
        wxMenu* menuFile      = new wxMenu;
        wxMenuItem* menuItem_Printscreen   = new wxMenuItem(menuFile, ID_PRINTSCREEN  , wxT("Save printscreen\tCtrl+S"));
                    menuItem_Printscreen  ->SetBitmap(FILESAVEAS);
        wxMenuItem* menuItem_HDPrintscreen = new wxMenuItem(menuFile, ID_HDPRINTSCREEN, wxT("Save HD printscreen"));
                    menuItem_HDPrintscreen->SetBitmap(FILESAVEAS);
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
    /**Sizers*/{
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
        sizer->Add(fpanel, 1, wxALL | wxEXPAND, 5);
        sizer->Add(ipanel, 0, wxLEFT | wxBOTTOM | wxRIGHT | wxEXPAND, 7);
        sizer->SetSizeHints(this);
        this->SetSizer(sizer);
    }
    /**Create fractal*/{
        f = new mb({-1.25L,0.0L}, 1.0L, fpanel->GetSize(), FractalHeight, true);
    }
    /**Create fractal thread*/{
        fthread = new std::thread(Run_fthread, this);
    }
}

typedef std::chrono::high_resolution_clock hrclock;
void FractalFrame::Run_fthread(){
    const mb::IterationT addIt = 100;
    while(true){
        ///Update the fractal
        auto t1 = hrclock::now();
        f->UpdateMath(addIt);
        auto t2 = hrclock::now();
        ///Update the screen
        wxClientDC dc(fpanel);
        dc.DrawBitmap(f->GetBmp(), 0, 0, true);
        ///Update the InfoPanel
        auto dt = std::chrono::duration<long double>(t2-t1);
        wxPoint p = wxGetMousePosition() - fpanel->GetScreenPosition();
        mb::ComplexNum c = f->GetOrigin() + mb::ComplexNum(+(mb::StepT)p.x*f->GetStep(),-(mb::StepT)p.y*f->GetStep());
        ipanel->Update(c, f->GetZoom(), f->GetNumIt(), dt.count()/(long double)addIt, f->GetHorizontalSize());
        ///Process events
        if(!fpanel->is_mouseevt_handled){
            OnZoomEvent(fpanel->mouseevt);
            fpanel->is_mouseevt_handled = true;
        }
        if(!fpanel->is_sizeevt_handled){
            OnSizeEvent();
            fpanel->is_sizeevt_handled = true;
        }
    }
}

void FractalFrame::OnZoomEvent(const wxMouseEvent& evt){
    wxPoint p = wxGetMousePosition() - fpanel->GetScreenPosition();
    mb::ComplexNum newcenter = f->GetOrigin() + mb::ComplexNum(
        p.x*f->GetStep(), -p.y*f->GetStep()
    );
    mb::ZoomT newzoom = f->GetZoom()*std::pow(3.16227766017L, (long double)evt.GetWheelRotation()/evt.GetWheelDelta());
    delete f;
    f = new mb(newcenter, newzoom, fpanel->GetSize(), FractalHeight, true);
}

void FractalFrame::OnSizeEvent(){
    wxPoint p = wxGetMousePosition() - fpanel->GetScreenPosition();
    mb::ComplexNum newcenter = f->GetCenter();
    mb::ZoomT newzoom = f->GetZoom();
    delete f;
    f = new mb(newcenter, newzoom, fpanel->GetSize(), FractalHeight, true);
}

/*
void FractalFrame::OnPrintscreen  (wxCommandEvent& event) {fractalPanel_->OnPrintscreen  ();}
void FractalFrame::OnHDPrintscreen(wxCommandEvent& event) {fractalPanel_->OnHDPrintscreen();}
*/
///MACROS - redirect events to functions
wxBEGIN_EVENT_TABLE(FractalFrame, wxFrame)
/*
    EVT_MENU(ID_PRINTSCREEN  , FractalFrame::OnPrintscreen  )
    EVT_MENU(ID_HDPRINTSCREEN, FractalFrame::OnHDPrintscreen)
*/
wxEND_EVENT_TABLE()
