#include "wxMandelbrot.h"

#include "myConversions.h"
#include <fstream>

/*
void CreateAllStuff(wxBitmap*& bmp, wxNativePixelData*& pixelData, std::vector<mb::Info>& infoVtr,
                    mb::ComplexNum origin, mb::ZoomT zoom, wxSize sz, mb::StepT fractalHeight){
    ///Get step to make some operations quicker
    mb::StepT step = fractalHeight/zoom/(mb::ZoomT)sz.y;
    ///Reset infoVtr and resize
    for(auto& i:infoVtr) i.Reset();
    infoVtr.resize(sz.x*sz.y);
    ///Fill infoVtr with new information
    long int i=0;
    mb::ComplexNum c = origin;
    for(int ImIndex = 0; ImIndex < sz.y; ++ImIndex, c.Im -= step){
        c.Re = origin.Re;
        for(int ReIndex = 0; ReIndex < sz.x; ++ReIndex, c.Re += step, ++i){
            infoVtr[i].c = c;
            infoVtr[i].Check = !c.isCardioid_isPeriod2Bulb();
        }
    }
    ///Clear bmp and pixelData
    bmp->Create(sz,24);
    if(pixelData != nullptr) delete pixelData;
    pixelData = new wxNativePixelData(*bmp);
}
*/

const std::string formatStr = ".png";
const wxBitmapType bitmapType = wxBITMAP_TYPE_PNG;
const std::string folderStr = ".\\Printscreens";

void SaveBitmapFcn(const wxBitmap* const bmp, long double time,
                   mb::ComplexNum center, mb::ZoomT zoom, mb::IterationT it){

    ///Create directory
    CreateDirectoryA(folderStr.c_str(), nullptr);
    ///Find name
    std::string outFileStr;
    std::string outFileStr_format;
    unsigned int i = 0;
    do{
        outFileStr = folderStr + "\\Image_" + mysprintf(i++, "%04d");
        outFileStr_format = outFileStr + formatStr;
    }while(std::ifstream(outFileStr_format.c_str()) && (i < 10000));
    if(i >= 10000){wxLogError("No suitable name for printscreen"); return;}
    ///Save file
    if(bmp->SaveFile(outFileStr_format, bitmapType)){
        std::ofstream outFileStream((outFileStr + "_log.txt").c_str(), std::ios::out);
        outFileStream << wxDateTime::Now().Format("Time and date: %d-%b-%Y %H:%M:%S").c_str() << std::endl
                      << mysprintf(time,          "Time elapsed: %.8f seconds"              ) << std::endl
                      << mysprintf(center.Re,     "Re(c) = %.20Lf"                          ) << std::endl
                      << mysprintf(center.Im,     "Im(c) = %.20Lf"                          ) << std::endl
                      << mysprintf(zoom,          "Zoom: %.20Lf"                            ) << std::endl
                      << "Image dimensions: "     << bmp->GetSize().x<<"x"<<bmp->GetSize().y  << std::endl
                      << "Number of iterations: " << it                                       << std::endl;
        outFileStream.close();
        wxLogMessage(std::string("Printscreen saved as " + outFileStr_format).c_str());
    }else{
        wxLogWarning("Could not save printscreen");
    }
}
