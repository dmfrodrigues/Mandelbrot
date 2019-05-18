#include "Mandelbrot.h"
#include <bits/stdc++.h>

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

mb::mb(ComplexNum o, ZoomT z, wxSize s, StepT H, bool IsCenter):
        zoom(z), sz(s), N(s.x*s.y), step(H/zoom/(ZoomT)sz.y),
        origin(IsCenter? GetOriginFromCenter(o, zoom, sz, H) : o),
        center(GetCenterFromOrigin(origin, zoom, sz, H)){
    C     = new ComplexNum[N];
    Z     = new ComplexNum[N]; std::fill(Z,Z+N,ComplexNum(0.0L,0.0L));
    IT    = new IterationT[N]; std::fill(IT,IT+N,0);
    Check = new bool[N]; std::fill(Check,Check+N,true);
    ///Fill 'C', 'Check' with new information
    unsigned long i = 0;
    ComplexNum c = origin;
    for(unsigned y = 0; y < sz.y; ++y, c.imag(c.imag()-step)){
        c.real(origin.real());
        for(unsigned x = 0; x < sz.x; ++x, c.real(c.real()+step), ++i){
            C[i] = c;
            Check[i] = !isCardioid_isPeriod2Bulb(c);
        }
    }
    ///Clear bmp and px
    bmp = new wxBitmap(sz, 24);
    px  = new wxNativePixelData(*bmp);
}
mb::~mb(){
    delete[] C;
    delete[] Z;
    delete[] IT;
    delete[] Check;
    delete bmp;
    delete px;
}

const unsigned NThreads = 8;
void mb::UpdateMath(IterationT addIt){
    std::thread *ArrThreads[NThreads];
    for(unsigned long L, R, i = 0; i < NThreads; ++i){
        L =  i   *N/NThreads;
        R = (i+1)*N/NThreads;
        ArrThreads[i] = new std::thread(mb::UpdateMathLim, this, L, R, addIt);
    }
    for(unsigned long i = 0; i < NThreads; ++i){
        ArrThreads[i]->join();
    }
    numIt += addIt;
}

void mb::UpdateMathLim(unsigned long L, unsigned long R, IterationT addIt){
    std::deque<unsigned long> changed;
    for(unsigned long i = L; i < R; ++i){
        if(!Check[i]) continue;
        IterationT it;
        ComplexNum z = Z[i], c = C[i];
        for(it = 0; it < addIt; ++it){
            z = z*z + c;
            if(std::norm(z) > bailout_sqr){
                changed.push_back(i);
                Check[i] = false;
                break;
            }
        }
        Z[i] = z; C[i] = c;
        IT[i] += it;
    }
    UpdatePixels(changed);
}

inline mb::ColorT cycleFun(mb::ColorT x){
    x = remainderf(x, pi2);
    if(x < -pi_2) x = -pi-x;
    if(+pi_2 < x) x =  pi-x;
    return x/pi_2;                                ///Linear
}
void mb::UpdatePixels(const std::deque<unsigned long>& q){
    const ColorT phi = 1.2L*pi_2;
    const ColorT AMP[] = {+ 27.5, +110.0, +110.0}, INIT[] = {+227.5, +110.0, +110.0}; ///RED&WHITE
    //const ColorT AMP[] = {-100.0, -100.0, +  0.0}, INIT[] = {+101.0, +101.0, +  0.0}; ///YELLOW&BLACK
    //const ColorT AMP[] = {-107.0, +  0.0, +  0.0}, INIT[] = {+128.0, + 10.0, + 10.0}; ///RED&BLACK
    //const ColorT AMP[] = {-113.0, -113.0, -113.0}, INIT[] = {+140.0, +140.0, +140.0}; ///BLACK&WHITE
    ColorT x, y;
    wxNativePixelData::Iterator p(*px);
    for(const auto& i:q){
        p.MoveTo(*px, i%sz.x, i/sz.x);

        //x = (ColorT)IT[i]-3.0*(log2(0.5*log10(Z[i].absSqr()))-log2_log10N); ///continuous/wavy pattern
        //x = (ColorT)IT[i]-1.0L*(log2(0.5*log10(Z[i].absSqr()))-log2_log10N); ///continuous pattern, modified formula
        x = (ColorT)IT[i]-(0.5L*log10(std::norm(Z[i]))/log10N-1.0L); ///continuous pattern, original formula
        //x = (ColorT)IT[i]; ///discrete pattern

        y = cycleFun(omega*x + phi);
        p.Red()   = AMP[0]*y + INIT[0];
        p.Green() = AMP[1]*y + INIT[1];
        p.Blue()  = AMP[2]*y + INIT[2];
    }
}

mb::ComplexNum mb::GetOriginFromCenter(ComplexNum cent, ZoomT z, wxSize s, StepT H){
    StepT st = H/z/(ZoomT)s.y;
    return cent + ComplexNum(-0.5L*(ComplexT)s.x*st, +0.5L*(ComplexT)s.y*st);
}
mb::ComplexNum mb::GetCenterFromOrigin(ComplexNum orig, ZoomT z, wxSize s, StepT H){
    StepT st = H/z/(ZoomT)s.y;
    return orig + ComplexNum(+0.5L*(ComplexT)s.x*st, -0.5L*(ComplexT)s.y*st);
}

/*
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
*/
/*
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
*/
