#include "Mandelbrot.h"

#include <fstream>
#include <iomanip>
#include <iostream>
#include <wx/log.h>

///Constructor
FractalBitmap::FractalBitmap(ComplexNum o, ComplexT z, wxSize s, ComplexT H, bool IsCenter):
        wxBitmap(s, 24), px(*((wxBitmap*)this)),
        zoom(z), N(GetSize().x*GetSize().y), step(H/zoom/(ComplexT)GetSize().y),
        origin(IsCenter? GetOriginFromCenter(o, zoom, GetSize(), H) : o),
        center(GetCenterFromOrigin(origin, zoom, GetSize(), H)){
    C     = new ComplexNum[N];
    Z     = new ComplexNum[N]; std::fill(Z,Z+N,ComplexNum(0.0L,0.0L));
    IT    = new IterationT[N]; std::fill(IT,IT+N,0);
    Check = new bool[N]; std::fill(Check,Check+N,true);
    ///Fill 'C', 'Check' with new information
    unsigned long i = 0;
    ComplexNum c = origin;
    for(unsigned y = 0; y < GetSize().y; ++y, c.imag(c.imag()-step)){
        c.real(origin.real());
        for(unsigned x = 0; x < GetSize().x; ++x, c.real(c.real()+step), ++i){
            C[i] = c;
            Check[i] = !isCardioid_isPeriod2Bulb(c);
        }
    }
}
FractalBitmap::~FractalBitmap(){
    delete[] C;
    delete[] Z;
    delete[] IT;
    delete[] Check;
}

FractalBitmap::ColorT FractalBitmap::CycleFun(FractalBitmap::ColorT x){
    x = remainderf(x, pi2);
    if(x < -pi_2) x = -pi-x;
    if(+pi_2 < x) x =  pi-x;
    return x/pi_2;                                ///Linear
}
void FractalBitmap::UpdatePixel(const unsigned long& i){
    wxNativePixelData::Iterator p(px);
    p.MoveTo(px, i%GetSize().x, i/GetSize().x);

    //x = (ColorT)IT[i]-3.0*(log2(0.5*log10(Z[i].absSqr()))-log2_log10N); ///continuous/wavy pattern
    //x = (ColorT)IT[i]-1.0L*(log2(0.5*log10(Z[i].absSqr()))-log2_log10N); ///continuous pattern, modified formula
    ColorT x = (ColorT)IT[i]-(0.5L*log10(std::norm(Z[i]))/log10N-1.0L); ///continuous pattern, original formula
    //x = (ColorT)IT[i]; ///discrete pattern

    ColorT y = CycleFun(omega*x + phi);
    p.Red()   = AMP[0]*y + INIT[0];
    p.Green() = AMP[1]*y + INIT[1];
    p.Blue()  = AMP[2]*y + INIT[2];
}

FractalBitmap::ComplexNum FractalBitmap::GetOriginFromCenter(ComplexNum cent, ComplexT z, wxSize s, ComplexT H){
    ComplexT st = H/z/(ComplexT)s.y;
    return cent + ComplexNum(-0.5L*(ComplexT)s.x*st, +0.5L*(ComplexT)s.y*st);
}
FractalBitmap::ComplexNum FractalBitmap::GetCenterFromOrigin(ComplexNum orig, ComplexT z, wxSize s, ComplexT H){
    ComplexT st = H/z/(ComplexT)s.y;
    return orig + ComplexNum(+0.5L*(ComplexT)s.x*st, -0.5L*(ComplexT)s.y*st);
}

bool FractalBitmap::SaveFile(const wxString& name, wxBitmapType type, const wxPalette *palette) const{
    wxBitmap::SaveFile(name, type, palette);
    std::ofstream ostrm(name.ToStdString() + ".txt");
    ostrm << "timedate\t"    << wxDateTime::Now().Format("%d-%b-%Y %H:%M:%S").c_str() << "\n"
          << "timeelapsed\t" << std::setprecision( 8) << 0.0                          << "\n"
          << "re(c)\t"       << std::setprecision(20) << center.real()                << "\n"
          << "im(c)\t"       << std::setprecision(20) << center.imag()                << "\n"
          << "zoom\t"        << std::setprecision(20) << zoom                         << "\n"
          << "size.x\t"      << GetSize().x                                           << "\n"
          << "size.y\t"      << GetSize().y                                           << "\n"
          << "NumIt\t"       << numIt                                                 << "\n" << std::flush;
    ostrm.close();
    return true;
}
