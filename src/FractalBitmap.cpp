#include "FractalBitmap.h"

FractalBitmap::FractalBitmap():wxBitmap(1,1,24){}

FractalBitmap::ComplexNum FractalBitmap::GetOriginFromCenter(ComplexNum cent, ComplexT st, wxSize s){
    return cent + ComplexNum(ComplexT(-0.5L*s.x)*st, ComplexT(+0.5L*s.y)*st);
}

FractalBitmap::ComplexNum FractalBitmap::GetCenterFromOrigin(ComplexNum orig, ComplexT st, wxSize s){
    return orig + ComplexNum(ComplexT(+0.5L*s.x)*st, ComplexT(-0.5L*s.y)*st);
}
