#include "Mandelbrot.h"
#include <iostream>
#include <numeric>
#include <thread>

///Constants
const mb::ColorT cycle = 100.0; /*100.0*/                 ///number of colors in a cycle (for coloring)
const mb::ColorT pi = acos(-1.0);
const mb::ColorT pi_2 = pi*0.5;
const mb::ColorT pi2 = 2.0*pi;
const mb::ColorT pi2_cycle = pi2/cycle;
const mb::ColorT& omega = pi2_cycle;
const mb::ComplexT bailout = 8.0L; // 2.0 // 8.0
const mb::ComplexT bailout_sqr = bailout*bailout;
const mb::ColorT log10N = log10(bailout);
const mb::ColorT log2_log10N = std::log2(log10N);

///Functions

///Constructor

mb::mb(ComplexNum origin, ZoomT zoom, wxSize sz, StepT fractalHeight):sz(sz){
    ///Get step to make some operations quicker
    mb::StepT step = fractalHeight/zoom/(ZoomT)sz.y;
    ///Reset infoVtr and resize
    C       = std::vector<ComplexNum>(sz.x*sz.y, {0.0L,0.0L});
    Z       = std::vector<ComplexNum>(sz.x*sz.y, {0.0L,0.0L});
    IT      = std::vector<IterationT>(sz.x*sz.y,           0);
    Check   = std::vector<bool      >(sz.x*sz.y,        true);
    ///Fill infoVtr with new information
    size_t i = 0;
    ComplexNum c = origin;
    for(int ImIndex = 0; ImIndex < sz.y; ++ImIndex, c.Im -= step){
        c.Re = origin.Re;
        for(int ReIndex = 0; ReIndex < sz.x; ++ReIndex, c.Re += step, ++i){
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
