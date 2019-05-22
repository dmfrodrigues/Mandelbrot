#ifndef MANDELBROT_H_INCLUDED
#define MANDELBROT_H_INCLUDED

#include <bits/stdc++.h>
#include <wx/rawbmp.h>

using namespace std;

class mb: public wxBitmap{
public:
    ///PUBLIC TYPEDEFS
    typedef long double ComplexT;
    typedef int IterationT;
    typedef std::complex<ComplexT> ComplexNum; ///std::complex is decently fast only if compiled with flag -ffast-math
    ///PUBLIC STATIC FUNCTIONS
    static inline bool isCardioid_isPeriod2Bulb(const ComplexNum& c){
        mb::ComplexT q = (c.real()-0.25)*(c.real()-0.25) + c.imag()*c.imag();    ///isCardioid
        return (4.0*q*(q+(c.real()-0.25)) < c.imag()*c.imag())||       ///isCardioid
               ((c.real()+1.0)*(c.real()+1.0) + c.imag()*c.imag() < 0.0625); ///isPeriod2Bulb
    }
private:
    ///PRIVATE TYPEDEFS
    typedef float ColorT;

    ///PRIVATE STATIC CONSTANTS
    static constexpr ColorT cycle = 100.0; /*100.0*/                 ///number of colors in a cycle (for coloring)
    static constexpr ColorT pi = acos(-1.0);
    static constexpr ColorT pi_2 = pi*0.5;
    static constexpr ColorT pi2 = 2.0*pi;
    static constexpr ColorT pi2_cycle = pi2/cycle;
    static constexpr ColorT omega = pi2_cycle;
    static constexpr ColorT phi = 1.2L*pi_2;
    static constexpr ColorT AMP[] = {+ 27.5, +110.0, +110.0}, INIT[] = {+227.5, +110.0, +110.0}; ///RED&WHITE
    //static const ColorT AMP[] = {-100.0, -100.0, +  0.0}, INIT[] = {+101.0, +101.0, +  0.0}; ///YELLOW&BLACK
    //static const ColorT AMP[] = {-107.0, +  0.0, +  0.0}, INIT[] = {+128.0, + 10.0, + 10.0}; ///RED&BLACK
    //static const ColorT AMP[] = {-113.0, -113.0, -113.0}, INIT[] = {+140.0, +140.0, +140.0}; ///BLACK&WHITE
    static constexpr ComplexT bailout = 8.0L; // 2.0 // 8.0
    static constexpr ComplexT bailout_sqr = bailout*bailout;
    static constexpr ColorT log10N = log10(bailout);
    static constexpr ColorT log2_log10N = std::log2(log10N);

    ///MEMBER VARIABLES
    const ComplexT              zoom;   ///Relative measure of zoom, 1 for initial zoom
    const unsigned long long    N;      ///Total size of the fractal (=sz.x*sz.y)
    const ComplexT              step;   ///Difference between consecutive pixels
    const ComplexNum            origin; ///Upper-left corner
    const ComplexNum            center; ///Center of the fractal
    IterationT                  numIt=0;///Total number of iterations performed over the fractal
    ComplexNum                  *C, *Z; ///Point in complex space, current value of z
    IterationT                  *IT;    ///Number of iterations
    bool                        *Check; ///True if more iterations should be performed (not diverged yet)
    wxNativePixelData           px;     ///PixelData, to access bmp

    ///PRIVATE FUNCTIONS
    /**
     * Update pixels in [L,R) by making an additional addIt iterations
     */
    void UpdateMathLim(unsigned long L, unsigned long R, IterationT addIt);
    /**
     * Update pixels whose indexes are in q, based on the information in the variables
     */
    inline void UpdatePixel(const unsigned long& i);

    static ColorT CycleFun(ColorT x);

public:
    /**
     * Constructor
     */
    mb(ComplexNum o, ComplexT z, wxSize s, ComplexT H, bool IsCenter = false);
    /**
     * Destructor
     */
    ~mb();

    ///CALCULATIONS ==================================================
    /**
     * Update all pixels by making an additional addIt iterations
     */
    void UpdateMath(IterationT addIt);

    ///GET FUNCTION ==================================================
    const ComplexNum&   GetOrigin()         const{ return origin; }
    const ComplexT&     GetZoom()           const{ return zoom;   }
    const ComplexNum&   GetCenter()         const{ return center; }
    const ComplexT&     GetStep()           const{ return step;   }
    const IterationT&   GetNumIt()          const{ return numIt;  }
    ComplexT            GetHorizontalSize() const{ return step*(ComplexT)GetSize().x; }

    ///STATIC FUNCTIONS ==============================================
    /**
     * Get origin of fractal from the provided center, and vice-versa
     */
    static ComplexNum GetOriginFromCenter(ComplexNum cent, ComplexT z, wxSize s, ComplexT H);
    static ComplexNum GetCenterFromOrigin(ComplexNum orig, ComplexT z, wxSize s, ComplexT H);

    ///OTHER UTILITIES
    virtual bool SaveFile(const wxString& name, wxBitmapType type, const wxPalette *palette = NULL) const;
};

#endif // MANDELBROT_H_INCLUDED
