#ifndef HDPRINTSCREENTHREAD_H_INCLUDED
#define HDPRINTSCREENTHREAD_H_INCLUDED

#include "ProgressDialog.h"
///=========================================================
///HDPRINTSCREENTHREAD
class HDPrintscreenThread: public wxThread {
public:
    ///================================
    ///CONSTRUCTOR
    HDPrintscreenThread(mb::ComplexNum center, mb::ZoomT zoom, wxSize sz, mb::IterationT maxIt, mb::StepT fractalHeight);
private:
    ///================================
    ///VARIABLES
    /*
    mb::ComplexNum center_;
    mb::ZoomT        zoom_;
    wxSize         sz_;
    mb::IterationT  maxIt_;
    wxBitmap* bmp_;
    */
    bool CancelEverything_ = false;
    /*
    wxNativePixelData* pixelData_;
    std::vector<mb::Info> infoVtr_;
    mb::StepT step_;
    mb::ComplexNum origin_;
    */
    ProgressDialog* progressDialog_;
    ///================================
    ///FUNCTIONS
    virtual wxThread::ExitCode Entry();
};

#endif // HDPRINTSCREENTHREAD_H_INCLUDED
