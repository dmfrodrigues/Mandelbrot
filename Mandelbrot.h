#ifndef MANDELBROT_H_INCLUDED
#define MANDELBROT_H_INCLUDED

#include <bits/stdc++.h>
#include <wx/rawbmp.h>
#include <wx/wx.h>
#include "Mandelbrot.h"

class mb{
public:
    typedef long double ComplexT;
    typedef long double ZoomT;
    typedef long double StepT;
    typedef int IterationT;
    typedef float ColorT;
    class ComplexNum{
    public:
        mb::ComplexT Re, Im;
        inline mb::ComplexT absSqr() const{ return (Re*Re + Im*Im); };
        inline bool isCardioid_isPeriod2Bulb() const{
            mb::ComplexT q = (Re-0.25)*(Re-0.25) + Im*Im;    ///isCardioid
            return (4.0*q*(q+(Re-0.25)) < Im*Im)||       ///isCardioid
                   ((Re+1.0)*(Re+1.0) + Im*Im < 0.0625); ///isPeriod2Bulb
        }
    };
    ///Constants
    static constexpr ColorT cycle = 100.0; /*100.0*/                 ///number of colors in a cycle (for coloring)
    static constexpr ColorT pi = acos(-1.0L), pi_2 = pi*0.5L, pi2 = 2.0L*pi;
    static constexpr ColorT pi2_cycle = pi2/cycle;
    static constexpr ColorT omega = pi2_cycle;
    static constexpr ComplexT bailout = 8.0L; // 2.0 // 8.0
    static constexpr ComplexT bailout_sqr = bailout*bailout;
    static constexpr ColorT log10N = log10(bailout);
    static constexpr ColorT log2_log10N = std::log2(log10N);
private:
    ///Variables
    ComplexNum              center;
    ZoomT                   zoom;
    wxSize                  sz;
    wxBitmap                *bmp;
    wxNativePixelData       *pixelData;
    std::vector<ComplexNum> C, Z;
    std::vector<IterationT> IT;
    std::vector<bool>       Check;
    //StepT                   step;
    ComplexNum              origin;
    IterationT              numIt;

public:
    /**
     * Constructor
     */
    mb(ComplexNum origin, ZoomT zoom, wxSize sz, StepT fractalHeight);

    ///CALCULATION ==========================================================================
    void GetColor(const std::deque<unsigned long int>& ChangedPixelsDeque);
    void GetPixelsThread(unsigned long l, unsigned long r, mb::IterationT numberIt);
    static void GetPixelsThreadPtr(mb *p, unsigned long l, unsigned long r, mb::IterationT numberIt);
    unsigned long int GetPixels(mb::IterationT numIt);

    ///'Get' functions
    const std::vector<ComplexNum>& getC() const{ return C; }
};

#endif // MANDELBROT_H_INCLUDED
