#ifndef WXMANDELBROT_H_INCLUDED
#define WXMANDELBROT_H_INCLUDED

#include <wx/wx.h>
#include "Mandelbrot.h"

void SaveBitmapFcn(const wxBitmap* const bmp, long double time,
                   mb::ComplexNum center, mb::ZoomT zoom, mb::IterationT it);

#endif // WXMANDELBROT_H_INCLUDED
