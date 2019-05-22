#include "HDPrintscreenThread.h"
#include "HDPrintscreenDialog.h"
#include "Mandelbrot.h"
#include "wxMandelbrot.h"

//wxDEFINE_EVENT(wxEVT_COMMAND_HDPRINTSCREENTHREAD_COMPLETED, wxThreadEvent);

const mb::IterationT itPerUpdate_Printscreen = 100;          ///no. of iterations performed before updating progress bar

HDPrintscreenThread::HDPrintscreenThread(mb::ComplexNum center, mb::ZoomT zoom, wxSize sz, mb::IterationT maxIt, mb::StepT fractalHeight)
                                    :wxThread(wxTHREAD_DETACHED), center_(center), zoom_ (zoom), sz_ (sz), maxIt_ (maxIt){
    HDPrintscreenDialog* dialog = new HDPrintscreenDialog(NULL, &center_, &zoom_, &sz_, &maxIt_);
    if(dialog->ShowModal() != wxID_OK){
        CancelEverything_ = true;
        return;
    }
    ///ProgressDialog
    progressDialog_ = new ProgressDialog(NULL, maxIt_, center_, zoom_, sz_);
    progressDialog_->Show(true);
    progressDialog_->Update();
    ///Create stuff
    //bmp_       = new wxBitmap(sz_, 24);
    //pixelData_ = nullptr;
    //infoVtr_   = std::vector<mb::Info>(0);
    step_      = fractalHeight/zoom_/(mb::ZoomT)sz_.y;
    origin_    = {center_.Re - step_*(mb::StepT)sz_.x/2.0L,
                  center_.Im + step_*(mb::StepT)sz_.y/2.0L};
    CreateAllStuff(bmp_, pixelData_, infoVtr_,
                   origin_, zoom_, sz_, fractalHeight);
    ///Adjust maxIt to make it a multiple of itPerUpdate_Printscreenr
    maxIt_ = (mb::IterationT)ceil((float)maxIt_/(float)itPerUpdate_Printscreen) * itPerUpdate_Printscreen;
    step_      = fractalHeight/zoom_/(mb::ZoomT)sz_.y;
}

wxThread::ExitCode HDPrintscreenThread::Entry(){
    if(CancelEverything_) return (wxThread::ExitCode)0;

    for(mb::IterationT numberIt = itPerUpdate_Printscreen; numberIt <= maxIt_; numberIt += itPerUpdate_Printscreen){
        mb::GetPixels(*pixelData_, sz_, itPerUpdate_Printscreen, infoVtr_);
        if(!progressDialog_->UpdateProgress(numberIt)){
            progressDialog_->Destroy();
            wxLogError("Computation interrupted. Could not save file.");
            return (wxThread::ExitCode)0;
        }
    }

    long double time = progressDialog_->GetTime();

    SaveBitmapFcn(this->bmp_, time, this->center_, this->zoom_, this->maxIt_);
    progressDialog_->Destroy();
    std::cout << "End of HDPrintscreenThread::Entry" << std::endl;
    return (wxThread::ExitCode)0;
}