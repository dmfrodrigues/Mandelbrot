#ifndef MANDELBROT_H_INCLUDED
#define MANDELBROT_H_INCLUDED

#include <bits/stdc++.h>
#include <wx/rawbmp.h>

using namespace std;

class mb: public wxBitmap{
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
    const unsigned long long    N;      ///Total size of the fractal (=sz.x*sz.y)
    const StepT                 step;   ///Difference between consecutive pixels
    const ComplexNum            origin; ///Upper-left corner
    const ComplexNum            center; ///Center of the fractal
    IterationT                  numIt=0;///Total number of iterations performed over the fractal
    ComplexNum                  *C, *Z; ///Point in complex space, current value of z
    IterationT                  *IT;    ///Number of iterations
    bool                        *Check; ///True if more iterations should be performed (not diverged yet)
    wxNativePixelData           px;     ///PixelData, to access bmp
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
    const ComplexNum&   GetCenter() const{ return center; }
    const StepT&        GetStep()   const{ return step;   }
    const IterationT&   GetNumIt()  const{ return numIt;  }
    StepT       GetHorizontalSize() const{ return step*(StepT)GetSize().x; }

    ///STATIC FUNCTIONS ==============================================
    /**
     * Get origin of fractal from the provided center, and vice-versa
     */
    static ComplexNum GetOriginFromCenter(ComplexNum cent, ZoomT z, wxSize s, StepT H);
    static ComplexNum GetCenterFromOrigin(ComplexNum orig, ZoomT z, wxSize s, StepT H);
};

#endif // MANDELBROT_H_INCLUDED
