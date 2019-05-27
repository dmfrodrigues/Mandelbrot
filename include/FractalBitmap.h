#ifndef FRACTALBITMAP_H_INCLUDED
#define FRACTALBITMAP_H_INCLUDED

#include <complex>
#include <wx/rawbmp.h>

using namespace std;

class FractalBitmap: public wxBitmap{
public:
    ///PUBLIC TYPEDEFS
    typedef long double ComplexT;
    typedef unsigned long long IterationT;
    typedef std::complex<ComplexT> ComplexNum; ///std::complex is decently fast only if compiled with flag -ffast-math

public:
    /**
     * Constructor
     */
    FractalBitmap();
    /**
     * New
     */
    virtual void New(ComplexNum o, ComplexT z, wxSize s, ComplexT H, bool IsCenter = false) = 0;
    /**
     * CreateNew
     */
    virtual FractalBitmap* CreateNew(ComplexNum o, ComplexT z, wxSize s, ComplexT H, bool IsCenter = false) = 0;
    /**
     * Destructor
     */
    ~FractalBitmap(){};

    ///CALCULATIONS ==================================================
    /**
     * Update all pixels by making an additional addIt iterations
     */
    virtual void UpdateMath(IterationT addIt) = 0;

    ///GET FUNCTION ==================================================
    virtual const ComplexNum&   GetOrigin()         const = 0;
    virtual const ComplexT&     GetZoom()           const = 0;
    virtual const ComplexNum&   GetCenter()         const = 0;
    virtual const ComplexT&     GetStep()           const = 0;
    virtual const IterationT&   GetNumIt()          const = 0;
    virtual ComplexT            GetHorizontalSize() const = 0;

    ///STATIC FUNCTIONS ==============================================
    /**
     * Get origin of fractal from the provided center, and vice-versa
     */
    static ComplexNum GetOriginFromCenter(ComplexNum cent, ComplexT z, wxSize s, ComplexT H);
    static ComplexNum GetCenterFromOrigin(ComplexNum orig, ComplexT z, wxSize s, ComplexT H);

};

#endif // FRACTALBITMAP_H_INCLUDED
