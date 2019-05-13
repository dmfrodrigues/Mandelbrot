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
    class Info{
    public:
        mb::ComplexNum c, z;
        mb::IterationT it;
        bool Check;
        Info(mb::ComplexNum c_ = {0.0L,0.0L}, mb::ComplexNum z_ = {0.0L,0.0L}, mb::IterationT it_ = 0, bool Check_ = true);
        void Reset();
    };
private:
    ComplexNum          center_;
    ZoomT               zoom_;
    wxSize              sz_;
    IterationT          maxIt_;
    wxBitmap            *bmp_;
    wxNativePixelData   *pixelData_;
    std::vector<Info>   infoVtr_;
    StepT               step_;
    ComplexNum          origin_;
    IterationT numberIt;

public:

    mb(ComplexNum origin, ZoomT zoom, wxSize sz, StepT fractalHeight);
    ///CALCULATION ==========================================================================
    void GetPixelsThread(unsigned long l, unsigned long r, mb::IterationT numberIt);
    static void GetPixelsThreadPtr(mb *p, unsigned long l, unsigned long r, mb::IterationT numberIt);
    unsigned long int GetPixels();
    void GetColor(const std::deque<unsigned long int>& ChangedPixelsDeque);

};
/*
namespace mb{
    typedef long double ComplexT;
    typedef long double ZoomT;
    typedef long double StepT;
    typedef int IterationT;
    typedef float ColorT;

    class ComplexNum{
    public:
        mb::ComplexT Re;
        mb::ComplexT Im;
        inline mb::ComplexT absSqr() const{
            return (Re*Re + Im*Im);
        };
        inline bool isCardioid_isPeriod2Bulb() const{
            mb::ComplexT q = (Re-0.25)*(Re-0.25) + Im*Im;    ///isCardioid
            return (4.0*q*(q+(Re-0.25)) < Im*Im)||       ///isCardioid
                   ((Re+1.0)*(Re+1.0) + Im*Im < 0.0625); ///isPeriod2Bulb
        }
    };

    class Info{
    public:
        mb::ComplexNum c;
        mb::ComplexNum z;
        mb::IterationT it;
        bool Check;
        Info(mb::ComplexNum c_ = {0.0L,0.0L}, mb::ComplexNum z_ = {0.0L,0.0L},
             mb::IterationT it_ = 0, bool Check_ = true);
        void Reset();
    };

    ///CALCULATION ==========================================================================
    unsigned long int GetPixels(wxNativePixelData& pixelData, wxSize sz,
                                mb::IterationT numberIt, std::vector<mb::Info>& infoVtr);
}
*/
#endif // MANDELBROT_H_INCLUDED
