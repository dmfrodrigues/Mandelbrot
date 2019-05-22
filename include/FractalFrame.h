#ifndef FRACTALFRAME_H_INCLUDED
#define FRACTALFRAME_H_INCLUDED

#include <wx/wx.h>
#include "Mandelbrot.h"

#include "FractalPanel.h"
#include "InfoPanel.h"

const mb::ComplexT    FractalHeight = 2.5L;        ///vertical height of the fractal at zoom==1

///=========================================================
///FRACTALFRAME
class FractalFrame: public wxFrame, public wxThreadHelper {
public:
    FractalFrame();
private:
    FractalPanel* fpanel;
    InfoPanel*    ipanel;

    wxThread::ExitCode Entry();

    mb *f;

    void OnZoomEvent(const wxMouseEvent& evt);
    void OnSizeEvent();

    bool is_prtscevt_handled = true;
    void OnPrintscreenCallback(wxCommandEvent& event){ is_prtscevt_handled = false; }
    void OnPrintscreenEvent() const;
    bool is_hdprtscevt_handled = true;
    void OnHDPrintscreenCallback(wxCommandEvent& event){ is_hdprtscevt_handled = false; }
    void OnHDPrintscreenEvent();

    wxDECLARE_EVENT_TABLE();
};


#endif
