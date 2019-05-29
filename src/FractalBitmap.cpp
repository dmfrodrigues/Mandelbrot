#include "FractalBitmap.h"

FractalBitmap::FractalBitmap():wxBitmap(1,1,24){}

FractalBitmap::ComplexNum FractalBitmap::GetOriginFromCenter(ComplexNum cent, ComplexT st, wxSize s){
    return cent + ComplexNum(-0.5L*(ComplexT)s.x*st, +0.5L*(ComplexT)s.y*st);
}

FractalBitmap::ComplexNum FractalBitmap::GetCenterFromOrigin(ComplexNum orig, ComplexT st, wxSize s){
    return orig + ComplexNum(+0.5L*(ComplexT)s.x*st, -0.5L*(ComplexT)s.y*st);
}
