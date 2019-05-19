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
const mb::ColorT phi = 1.2L*pi_2;
const mb::ColorT AMP[] = {+ 27.5, +110.0, +110.0}, INIT[] = {+227.5, +110.0, +110.0}; ///RED&WHITE
//const ColorT AMP[] = {-100.0, -100.0, +  0.0}, INIT[] = {+101.0, +101.0, +  0.0}; ///YELLOW&BLACK
//const ColorT AMP[] = {-107.0, +  0.0, +  0.0}, INIT[] = {+128.0, + 10.0, + 10.0}; ///RED&BLACK
//const ColorT AMP[] = {-113.0, -113.0, -113.0}, INIT[] = {+140.0, +140.0, +140.0}; ///BLACK&WHITE


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
    for(unsigned long i = L; i < R; ++i){
        if(!Check[i]) continue;
        IterationT it;
        ComplexNum z = Z[i], c = C[i];
        for(it = 0; it < addIt; ++it){
            z = z*z + c;
            if(std::norm(z) > bailout_sqr){
                Z[i] = z; IT[i] += it;
                Check[i] = false;
                UpdatePixel(i);
                break;
            }
        }
        if(Check[i]){
            Z[i] = z;
            IT[i] += it;
        }
    }
}

inline mb::ColorT cycleFun(mb::ColorT x){
    x = remainderf(x, pi2);
    if(x < -pi_2) x = -pi-x;
    if(+pi_2 < x) x =  pi-x;
    return x/pi_2;                                ///Linear
}
void mb::UpdatePixel(const unsigned long& i){
    wxNativePixelData::Iterator p(*px);
    p.MoveTo(*px, i%sz.x, i/sz.x);

    //x = (ColorT)IT[i]-3.0*(log2(0.5*log10(Z[i].absSqr()))-log2_log10N); ///continuous/wavy pattern
    //x = (ColorT)IT[i]-1.0L*(log2(0.5*log10(Z[i].absSqr()))-log2_log10N); ///continuous pattern, modified formula
    ColorT x = (ColorT)IT[i]-(0.5L*log10(std::norm(Z[i]))/log10N-1.0L); ///continuous pattern, original formula
    //x = (ColorT)IT[i]; ///discrete pattern

    ColorT y = cycleFun(omega*x + phi);
    p.Red()   = AMP[0]*y + INIT[0];
    p.Green() = AMP[1]*y + INIT[1];
    p.Blue()  = AMP[2]*y + INIT[2];
}

mb::ComplexNum mb::GetOriginFromCenter(ComplexNum cent, ZoomT z, wxSize s, StepT H){
    StepT st = H/z/(ZoomT)s.y;
    return cent + ComplexNum(-0.5L*(ComplexT)s.x*st, +0.5L*(ComplexT)s.y*st);
}
mb::ComplexNum mb::GetCenterFromOrigin(ComplexNum orig, ZoomT z, wxSize s, StepT H){
    StepT st = H/z/(ZoomT)s.y;
    return orig + ComplexNum(+0.5L*(ComplexT)s.x*st, -0.5L*(ComplexT)s.y*st);
}
