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

///Function
///Constructor

mb::mb(ComplexNum origin, ZoomT zoom, wxSize sz, StepT fractalHeight):
    origin_(origin), zoom_(zoom), sz_(sz){
    ///Get step to make some operations quicker
    mb::StepT step = fractalHeight/zoom_/(ZoomT)sz_.y;
    ///Reset infoVtr and resize
    for(auto& i:infoVtr_) i.Reset();
    infoVtr_.resize(sz.x*sz.y);
    ///Fill infoVtr with new information
    long int i=0;
    mb::ComplexNum c = origin_;
    for(int ImIndex = 0; ImIndex < sz.y; ++ImIndex, c.Im -= step){
        c.Re = origin.Re;
        for(int ReIndex = 0; ReIndex < sz.x; ++ReIndex, c.Re += step, ++i){
            infoVtr_[i].c = c;
            infoVtr_[i].Check = !c.isCardioid_isPeriod2Bulb();
        }
    }
    ///Clear bmp and pixelData
    bmp_->Create(sz,24);
    if(pixelData_ != nullptr) delete pixelData_;
    pixelData_ = new wxNativePixelData(*bmp_);
}

mb::Info::Info(mb::ComplexNum c_, mb::ComplexNum z_, mb::IterationT it_, bool Check_):c(c_), z(z_), it(it_), Check(Check_){}
void mb::Info::Reset(){
    c = {0.0L,0.0L};
    z = {0.0L,0.0L};
    it = 0;
    Check = true;
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
    const mb::ColorT  Amp[] = {+ 27.5, +110.0, +110.0};
    const mb::ColorT Init[] = {+227.5, +110.0, +110.0};
    ///YELLOW&BLACK
//    const mb::ColorT  Amp[] = {-100.0, -100.0, +  0.0};
//    const mb::ColorT Init[] = {+101.0, +101.0, +  0.0};
    ///RED&BLACK
//    const mb::ColorT  Amp[] = {-107.0, +  0.0, +  0.0};
//    const mb::ColorT Init[] = {+128.0, + 10.0, + 10.0};
    ///BLACK&WHITE
//    const mb::ColorT  Amp[] = {-113.0, -113.0, -113.0};
//    const mb::ColorT Init[] = {+140.0, +140.0, +140.0};

    mb::ColorT x, y;
    wxNativePixelData::Iterator p(pixelData_);
    for(const auto& index:ChangedPixelsDeque){
        p.MoveTo(pixelData, index%sz.x, index/sz.x);

        ///continuous/wavy pattern
        //x = (mb::ColorT)infoVtr[index].it-3.0*(log2(0.5*log10(infoVtr[index].z.absSqr()))-log2_log10N);
        ///continuous pattern, modified formula
        //x = (mb::ColorT)infoVtr[index].it-1.0L*(log2(0.5*log10(infoVtr[index].z.absSqr()))-log2_log10N);
        ///continuous pattern, original formula
        x = (mb::ColorT)infoVtr[index].it-(0.5L*log10(infoVtr[index].z.absSqr())/log10N-1.0L);
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
        auto& info = infoVtr[i];
        if(info.Check){
            for (it = 0; it < numberIt; ++it){
                info.z = {     info.z.Re*info.z.Re - info.z.Im*info.z.Im + info.c.Re,
                          2.0L*info.z.Re*info.z.Im                       + info.c.Im};
                if (info.z.absSqr() > bailout_sqr){
                    ChangedPixelsDeque.push_back(i);
                    info.Check = false;
                    break;
                }
            }
            info.it += it;
        }
    }

    GetColor(ChangedPixelsDeque);

    //ret = ChangedPixelsDeque.size();
}

void mb::GetPixelsThreadPtr(mb *p, unsigned long l, unsigned long r, mb::IterationT numberIt){
    p->GetPixelsThread(l, r, numberIt);
}


unsigned long int  mb::GetPixels(){
    const unsigned long NThreads = 8;
    unsigned long ret = 0;
    const unsigned long N = infoVtr.size();
    std::vector<unsigned long> r(NThreads);
    std::vector<std::thread> vthreads(NThreads);
    for(unsigned long i = 0; i < NThreads; ++i){
        vthreads[i] = std::thread(mb::GetPixelsThreadPtr, this, N*i/NThreads, N*(i+1)/NThreads, numberIt);
        //GetPixelsThread(pixelData, sz, numberIt, infoVtr, N*i/NThreads, N*(i+1)/NThreads);
    }
    for(auto& t:vthreads) t.join();
    return std::accumulate(r.begin(), r.end(), 0);
}
