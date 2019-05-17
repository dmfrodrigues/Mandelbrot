#include "FractalFrame.h"
#include "CURSOR_CROSS.h"

//#include "SettingsDialog.h"
//#include "wxMandelbrot.h"
//#include "HDPrintscreenThread.h"

//const mb::IterationT itPerUpdate =   20; /*500*/          ///no. of iterations performed before updating the window.
//const mb::ZoomT        zoomRatio = 4.0L;        ///number that multiplies to zoom every time a zoom is applied

enum{
    ID_ChangeSettings = 1
};

///Constructor
FractalPanel::FractalPanel(FractalFrame* p, wxSize s)
    :wxPanel(p, wxID_ANY, wxDefaultPosition, s, wxBORDER_RAISED),
     parent(p){
    ///Initial settings
    SetCursor(CURSOR_CROSS);
    //popupMenu_ = new wxMenu;
    //popupMenu_->Append(ID_ChangeSettings, "Change settings");

    ///Thread stuff
    /*
    if(CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) wxLogError("Could not create the worker thread.");
    if(GetThread()->Run()              != wxTHREAD_NO_ERROR) wxLogError("Could not run the worker thread.");
    */
}

BEGIN_EVENT_TABLE(FractalPanel, wxPanel)
/*
    EVT_PAINT     (FractalFrame::FractalPanel::OnPaintEvent)
    EVT_RIGHT_UP  (FractalFrame::FractalPanel::OnRightUp   )
    EVT_SIZE      (FractalFrame::FractalPanel::OnSizeEvent )
    EVT_MOUSEWHEEL(FractalFrame::FractalPanel::OnMouseEvent)
    EVT_MENU      (ID_ChangeSettings, FractalFrame::FractalPanel::OnChangeSettings)
*/
END_EVENT_TABLE()
