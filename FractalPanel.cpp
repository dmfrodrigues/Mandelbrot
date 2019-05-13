#include "FractalFrame.h"

#include "SettingsDialog.h"
#include "wxMandelbrot.h"
#include "HDPrintscreenThread.h"

const mb::IterationT itPerUpdate =   20; /*500*/          ///no. of iterations performed before updating the window.
const mb::ZoomT        zoomRatio = 4.0L;        ///number that multiplies to zoom every time a zoom is applied

enum{
    ID_ChangeSettings = 1
};

///Constructor
FractalFrame::FractalPanel::FractalPanel(FractalFrame* parent, wxSize sz)
                                        :wxPanel(parent, wxID_ANY, wxDefaultPosition, sz, wxBORDER_RAISED),
                                         parent_(parent){
    ///Initial settings
    //this->SetCursor(wxCursor(wxCURSOR_BLANK));
    bmp_ = new wxBitmap(this->GetSize(), 24); ///depth 24bits == RGB channels, without alpha channel

    popupMenu_ = new wxMenu;
    popupMenu_->Append(ID_ChangeSettings, "Change settings");

    ///Thread stuff
    if(CreateThread(wxTHREAD_JOINABLE) != wxTHREAD_NO_ERROR) wxLogError("Could not create the worker thread.");
    if(GetThread()->Run()              != wxTHREAD_NO_ERROR) wxLogError("Could not run the worker thread.");
}
///Worker thread
inline mb::IterationT Update_itPerUpdate_dynamic(unsigned long int numberChangedPixels){
    return itPerUpdate;
}
wxThread::ExitCode FractalFrame::FractalPanel::Entry(){

    center_ = {-0.5L, 0.0L};
    zoom_   = 1.0L;

    ChangeSettings();

    LARGE_INTEGER ti, tf, freq; double timeIt;
    mb::IterationT itPerUpdate_dynamic = itPerUpdate;
    unsigned long int numberChangedPixels;
    while(true){

        QueryPerformanceFrequency(&freq);   ///Time
        QueryPerformanceCounter(&ti);       ///Time

        numberChangedPixels = parent_->mymb->GetPixels(itPerUpdate_dynamic);
        totalIterations_ += itPerUpdate_dynamic;

        QueryPerformanceCounter(&tf);       ///Time
        timeIt = (double)(tf.QuadPart - ti.QuadPart) / (double)freq.QuadPart / (double)itPerUpdate_dynamic;  ///Time

        ///Get new itPerUpdate_dynamic, adequate to needs
        itPerUpdate_dynamic = Update_itPerUpdate_dynamic(numberChangedPixels);

        wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
        parent_->infoPanel_->SetStatus(parent_->mymb->getC(), this->GetSize(), zoom_, step_*(bmp_->GetSize()).x,
                                       totalIterations_, timeIt, mousePos);
        this->paintNow(mousePos);

        if(mouseEvent_.IsValid() || sizeEvent_.IsValid())
            ProcessEvent();
    }
    return (wxThread::ExitCode)0;
}
///Painting
void FractalFrame::FractalPanel::OnPaintEvent(wxPaintEvent& event){
    wxPaintDC  dc(this);
    this->render(dc);
}
void FractalFrame::FractalPanel::paintNow(wxPoint mousePos){
    wxClientDC dc(this);
    this->render(dc);

    dc.SetPen(*wxGREY_PEN);
    wxSize   panelSz = this->GetSize();
    if(0 <= mousePos.x && mousePos.x <= panelSz.x &&
       0 <= mousePos.y && mousePos.y <= panelSz.y){
        dc.DrawLine(         0, mousePos.y,  panelSz.x, mousePos.y); ///Horizontal line
        dc.DrawLine(mousePos.x,          0, mousePos.x,  panelSz.y); ///Vertical line
    }
}
void FractalFrame::FractalPanel::render(wxDC& dc){
    dc.DrawBitmap(*bmp_, 0, 0, true);
}
///Event handling
void FractalFrame::FractalPanel::ProcessEvent(){
    if(sizeEvent_.IsValid()) OnChangeSizeEvent();
    if(mouseEvent_.IsValid() && mouseEvent_.IsInside(this)){
        if(mouseEvent_.GetEventType() == wxEVT_MOUSEWHEEL ||
           mouseEvent_.GetEventType() == wxEVT_LEFT_DOWN ||
           mouseEvent_.GetEventType() == wxEVT_RIGHT_DOWN) OnZoomEvent();
    }
}
void FractalFrame::FractalPanel::OnRightUp(wxMouseEvent& event){
    PopupMenu(popupMenu_);
}
void FractalFrame::FractalPanel::OnChangeSettings(wxCommandEvent& event){
    mb::ComplexNum center = center_;
    mb::ZoomT        zoom =   zoom_;
    SettingsDialog* dialog = new SettingsDialog(this, &center, &zoom);
    dialog->ShowModal();
    ChangeSettings(center, zoom);
}

BEGIN_EVENT_TABLE(FractalFrame::FractalPanel, wxPanel)
    EVT_PAINT     (FractalFrame::FractalPanel::OnPaintEvent)
    EVT_RIGHT_UP  (FractalFrame::FractalPanel::OnRightUp   )
    EVT_SIZE      (FractalFrame::FractalPanel::OnSizeEvent )
    EVT_MOUSEWHEEL(FractalFrame::FractalPanel::OnMouseEvent)
    EVT_MENU      (ID_ChangeSettings, FractalFrame::FractalPanel::OnChangeSettings)
END_EVENT_TABLE()
///Secondary event handlers
void FractalFrame::FractalPanel::OnChangeSizeEvent(mySizeEvent& event){
    while(wxGetMouseState().LeftIsDown()) wxMilliSleep(50);
    ChangeSettings();
    event.SetValid(false);
}
void FractalFrame::FractalPanel::OnZoomEvent(myMouseEvent& event){
    ///New center
    wxPoint mousePos = wxGetMousePosition() - this->GetScreenPosition();
    center_ = {origin_.Re + (mb::ComplexT)mousePos.x * (mb::ComplexT)step_,
               origin_.Im - (mb::ComplexT)mousePos.y * (mb::ComplexT)step_};
    ///New zoom
    zoom_ *= pow(zoomRatio, event.GetWheelRotation());
    if(event.GetEventType() == wxEVT_LEFT_DOWN) zoom_*=zoomRatio;
    if(event.GetEventType() == wxEVT_RIGHT_DOWN) zoom_/=zoomRatio;
    ///Reset fractal
    ChangeSettings(center_, zoom_);
    ///Reset evtType
    event.SetValid(false);
}
///Change settings
void FractalFrame::FractalPanel::ChangeSettings(mb::ComplexNum center, mb::ZoomT zoom){
    center_ = center;
    zoom_ = zoom;

    wxSize sz = this->GetSize();
    step_   = fractalHeight/zoom_/(mb::ZoomT)sz.y;
    origin_ = {center_.Re - step_*sz.x/2.0L,
               center_.Im + step_*sz.y/2.0L};
    ChangeSettings_origin(origin_, zoom_);
}
void FractalFrame::FractalPanel::ChangeSettings_origin(mb::ComplexNum origin, mb::ZoomT zoom){
    origin_ = origin;
    zoom_   = zoom;

    wxSize sz = this->GetSize();
    step_ = fractalHeight/zoom_/(mb::ZoomT)sz.y;
    center_ = {origin_.Re + step_*sz.x/2.0L,
               origin_.Im - step_*sz.y/2.0L};

    totalIterations_ = 0;

    parent_->mymb = new mb(origin_, zoom_, this->GetSize(), fractalHeight);
}

void FractalFrame::FractalPanel::OnPrintscreen(){
    SaveBitmapFcn(bmp_, 0.0L, center_, zoom_, totalIterations_);
}
void FractalFrame::FractalPanel::OnHDPrintscreen(){
    ///Start the thread
    HDPrintscreenThread* hdprintscreenThread =
        new HDPrintscreenThread(this->center_, this->zoom_, this->GetSize(), this->totalIterations_, fractalHeight);
    if(hdprintscreenThread->Run() != wxTHREAD_NO_ERROR){
        delete hdprintscreenThread;
        hdprintscreenThread = nullptr;
        wxLogError("Could not create HDPrintscreenThread");
    }
}

