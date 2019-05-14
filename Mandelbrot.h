#ifndef MANDELBROT_H_INCLUDED
#define MANDELBROT_H_INCLUDED

#include <vector>
#include <deque>
#include <wx/rawbmp.h>

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
private:

    //ComplexNum              center_;
    //ZoomT                   zoom_;
    wxSize                  sz;
    IterationT              maxIt;
    wxBitmap                *bmp;
    wxNativePixelData       *pixelData;
    std::vector<ComplexNum> C, Z;
    std::vector<IterationT> IT;
    std::vector<bool>       Check;
    //StepT                   step_;
    //ComplexNum              origin_;
    //IterationT              numberIt;

public:

    mb(ComplexNum origin, ZoomT zoom, wxSize sz, StepT fractalHeight);
    ///CALCULATION ==========================================================================
    void GetPixelsThread(unsigned long l, unsigned long r, mb::IterationT numberIt);
    static void GetPixelsThreadPtr(mb *p, unsigned long l, unsigned long r, mb::IterationT numberIt);
    unsigned long int GetPixels(mb::IterationT numIt);
    void GetColor(const std::deque<unsigned long int>& ChangedPixelsDeque);

    ///'Get' functions
    const std::vector<ComplexNum>& getC() const{ return C; }
};

#endif // MANDELBROT_H_INCLUDED
