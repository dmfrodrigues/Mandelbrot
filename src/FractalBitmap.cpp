#include "FractalBitmap.h"

FractalBitmap::FractalBitmap():wxBitmap(1,1,24){}

FractalBitmap::ComplexNum FractalBitmap::GetOriginFromCenter(ComplexNum cent, ComplexT z, wxSize s, ComplexT H){
    ComplexT st = H/z/(ComplexT)s.y;
    return cent + ComplexNum(-0.5L*(ComplexT)s.x*st, +0.5L*(ComplexT)s.y*st);
}

FractalBitmap::ComplexNum FractalBitmap::GetCenterFromOrigin(ComplexNum orig, ComplexT z, wxSize s, ComplexT H){
    ComplexT st = H/z/(ComplexT)s.y;
    return orig + ComplexNum(+0.5L*(ComplexT)s.x*st, -0.5L*(ComplexT)s.y*st);
}
