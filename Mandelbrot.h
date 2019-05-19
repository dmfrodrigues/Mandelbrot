#ifndef MANDELBROT_H_INCLUDED
#define MANDELBROT_H_INCLUDED

#include <bits/stdc++.h>
#include <wx/rawbmp.h>

using namespace std;

class mb{
public:
    ///Typedefs
    typedef long double ComplexT;
    typedef long double ZoomT;
    typedef long double StepT;
    typedef int IterationT;
    typedef float ColorT;
    typedef std::complex<ComplexT> ComplexNum; ///std::complex is decently fast only if compiled with flag -ffast-math
    static inline bool isCardioid_isPeriod2Bulb(const ComplexNum& c){
        mb::ComplexT q = (c.real()-0.25)*(c.real()-0.25) + c.imag()*c.imag();    ///isCardioid
        return (4.0*q*(q+(c.real()-0.25)) < c.imag()*c.imag())||       ///isCardioid
               ((c.real()+1.0)*(c.real()+1.0) + c.imag()*c.imag() < 0.0625); ///isPeriod2Bulb
    }
private:
    ///Variables
    const ZoomT                 zoom;   ///Relative measure of zoom, 1 for initial zoom
    const wxSize                sz;     ///Size of the fractal
    const unsigned long long    N;      ///Total size of the fractal (=sz.x*sz.y)
    const StepT                 step;   ///Difference between consecutive pixels
    const ComplexNum            origin; ///Upper-left corner
    const ComplexNum            center; ///Center of the fractal
    IterationT                  numIt=0;///Total number of iterations performed over the fractal
    ComplexNum                  *C, *Z; ///Point in complex space, current value of z
    IterationT                  *IT;    ///Number of iterations
    bool                        *Check; ///True if more iterations should be performed (not diverged yet)
    wxBitmap                    *bmp;   ///Bitmap
    wxNativePixelData           *px;    ///PixelData, to access bmp
public:
    /**
     * Constructor
     */
    mb(ComplexNum origin, ZoomT zoom, wxSize sz, StepT fractalHeight, bool IsCenter = false);
    ~mb();
    ///CALCULATIONS ==================================================
    /**
     * Update all pixels by making an additional addIt iterations
     */
    void UpdateMath(IterationT addIt);
    /**
     * Update pixels in [L,R) by making an additional addIt iterations
     */
    void UpdateMathLim(unsigned long L, unsigned long R, IterationT addIt);
    /**
     * Update pixels whose indexes are in q, based on the information in the variables
     */
    inline void UpdatePixel(const unsigned long& i);

    ///GET FUNCTION ==================================================
    const ComplexNum&   GetOrigin() const{ return origin; }
    const ZoomT&        GetZoom()   const{ return zoom;   }
    const wxSize&       GetSize()   const{ return sz;     }
    const wxBitmap&     GetBmp()    const{ return *bmp;   }
    const ComplexNum&   GetCenter() const{ return center; }
    const StepT&        GetStep()   const{ return step;   }
    const IterationT&   GetNumIt()  const{ return numIt;  }
    StepT       GetHorizontalSize() const{ return step*(StepT)sz.x; }

    ///STATIC FUNCTIONS ==============================================
    /**
     * Get origin of fractal from the provided center, and vice-versa
     */
    static ComplexNum GetOriginFromCenter(ComplexNum cent, ZoomT z, wxSize s, StepT H);
    static ComplexNum GetCenterFromOrigin(ComplexNum orig, ZoomT z, wxSize s, StepT H);

    /*
    void GetPixelsThread(unsigned long l, unsigned long r, mb::IterationT numberIt);
    static void GetPixelsThreadPtr(mb *p, unsigned long l, unsigned long r, mb::IterationT numberIt);
    unsigned long int GetPixels();
    void GetColor(const std::deque<unsigned long int>& ChangedPixelsDeque);
    */
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
