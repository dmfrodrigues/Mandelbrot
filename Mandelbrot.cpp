#include "Mandelbrot.h"

#include "myConversions.h"
#include <fstream>

///Constructor

mb::mb(ComplexNum origin, ZoomT zoom, wxSize sz, StepT fractalHeight)
        :sz(sz), C(sz.x*sz.y, {0.0L,0.0L}), Z(sz.x*sz.y, {0.0L, 0.0L}),
        IT(sz.x*sz.y, 0), Check(sz.x*sz.y, true){
    ///Get step to make some operations quicker
    mb::StepT step = fractalHeight/zoom/(ZoomT)sz.y;
    /*
    ///Reset infoVtr and resize
    C       = std::vector<ComplexNum>(sz.x*sz.y, {0.0L,0.0L});
    Z       = std::vector<ComplexNum>(sz.x*sz.y, {0.0L,0.0L});
    IT      = std::vector<IterationT>(sz.x*sz.y,           0);
    Check   = std::vector<bool      >(sz.x*sz.y,        true);
    */
    ///Fill infoVtr with new information
    size_t i = 0;
    ComplexNum c = origin;
    for(int y = 0; y < sz.y; ++y, c.Im -= step){
        c.Re = origin.Re;
        for(int x = 0; x < sz.x; ++x, c.Re += step, ++i){
            C[i] = c;
            Check[i] = !c.isCardioid_isPeriod2Bulb();
        }
    }
    ///Clear bmp and pixelData
    bmp->Create(sz,24);
    if(pixelData != nullptr) delete pixelData;
    pixelData = new wxNativePixelData(*bmp);
}

///COLOR ================================================================================
inline mb::ColorT cycleFun(mb::ColorT x){
    x = remainderf(x, pi2);
    if(x < -pi_2) x = -pi-x;
    if(+pi_2 < x) x =  pi-x;
    return x/pi_2;                                ///Linear
}

void mb::GetColor(const std::deque<unsigned long int>& ChangedPixelsDeque){
    const mb::ColorT    phi = 1.2L*pi_2; //pi_2
    ///RED&WHITE
    const mb::ColorT  Amp[] = {+ 27.5, +110.0, +110.0}; const mb::ColorT Init[] = {+227.5, +110.0, +110.0};
    ///YELLOW&BLACK
//    const mb::ColorT  Amp[] = {-100.0, -100.0, +  0.0}; const mb::ColorT Init[] = {+101.0, +101.0, +  0.0};
    ///RED&BLACK
//    const mb::ColorT  Amp[] = {-107.0, +  0.0, +  0.0}; const mb::ColorT Init[] = {+128.0, + 10.0, + 10.0};
    ///BLACK&WHITE
//    const mb::ColorT  Amp[] = {-113.0, -113.0, -113.0}; const mb::ColorT Init[] = {+140.0, +140.0, +140.0};

    mb::ColorT x, y;
    wxNativePixelData::Iterator p(*pixelData);
    for(const auto& index:ChangedPixelsDeque){
        p.MoveTo(*pixelData, index%sz.x, index/sz.x);

        ///continuous/wavy pattern
        //x = (mb::ColorT)infoVtr[index].it-3.0*(log2(0.5*log10(infoVtr[index].z.absSqr()))-log2_log10N);
        ///continuous pattern, modified formula
        //x = (mb::ColorT)infoVtr[index].it-1.0L*(log2(0.5*log10(infoVtr[index].z.absSqr()))-log2_log10N);
        ///continuous pattern, original formula
        x = (mb::ColorT)IT[index]-(0.5L*log10(Z[index].absSqr())/log10N-1.0L);
        ///discrete pattern
        //x = (mb::ColorT)infoVtr[index].it;
        ///
        y = cycleFun(omega*x + phi);
        p.Red()   = Amp[0]*y + Init[0];
        p.Green() = Amp[1]*y + Init[1];
        p.Blue()  = Amp[2]*y + Init[2];
    }
}

///CALCULATION ==========================================================================
void mb::GetPixelsThread(unsigned long l, unsigned long r, mb::IterationT numberIt){
    std::deque<unsigned long> ChangedPixelsDeque;
    mb::IterationT it;
    for(unsigned long int i = l; i < r; ++i){
        if(Check[i]){
            for (it = 0; it < numberIt; ++it){
                Z[i] = {     Z[i].Re*Z[i].Re - Z[i].Im*Z[i].Im + C[i].Re,
                        2.0L*Z[i].Re*Z[i].Im                   + C[i].Im};
                if (Z[i].absSqr() > bailout_sqr){
                    ChangedPixelsDeque.push_back(i);
                    Check[i] = false;
                    break;
                }
            }
            IT[i] += it;
        }
    }

    GetColor(ChangedPixelsDeque);

    //ret = ChangedPixelsDeque.size();
}

void mb::GetPixelsThreadPtr(mb *p, unsigned long l, unsigned long r, mb::IterationT numberIt){
    p->GetPixelsThread(l, r, numberIt);
}


unsigned long int  mb::GetPixels(mb::IterationT numIt){
    const unsigned long NThreads = 8;
    unsigned long ret = 0;
    const unsigned long N = C.size();
    std::vector<unsigned long> r(NThreads);
    std::vector<std::thread> vthreads(NThreads);
    /*
    for(unsigned long i = 0; i < NThreads; ++i){
        vthreads[i] = std::thread(mb::GetPixelsThreadPtr, this, N*i/NThreads, N*(i+1)/NThreads, numIt);
        //GetPixelsThread(pixelData, sz, numberIt, infoVtr, N*i/NThreads, N*(i+1)/NThreads);
    }
    for(auto& t:vthreads) t.join();
    return std::accumulate(r.begin(), r.end(), 0);
    */
    GetPixelsThreadPtr(this, 0, N, numIt);
}

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
