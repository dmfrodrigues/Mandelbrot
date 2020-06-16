#ifndef MANDELBROT_H_INCLUDED
#define MANDELBROT_H_INCLUDED

#include "FractalBitmap.h"

#include <deque>
#include <vector>
#include <list>


class mb: public FractalBitmap{
private:
    ///PRIVATE TYPEDEFS
    typedef float ColorT;

    ///PRIVATE STATIC CONSTANTS
    static constexpr ColorT cycle = 100.0L; /*100.0*/                 ///number of colors in a cycle (for coloring)
    static constexpr ColorT pi = 3.1415926535897L; //acos(-1.0L);
    static constexpr ColorT pi_2 = pi*0.5L;
    static constexpr ColorT pi2 = 2.0L*pi;
    static constexpr ColorT pi2_cycle = pi2/cycle;
    static constexpr ColorT omega = pi2_cycle;
    static constexpr ColorT phi = 1.2L*pi_2;
    static constexpr ColorT AMP[] = {+ 27.5, +110.0, +110.0}, INIT[] = {+227.5, +110.0, +110.0}; ///RED&WHITE
    //static const ColorT AMP[] = {-100.0, -100.0, +  0.0}, INIT[] = {+101.0, +101.0, +  0.0}; ///YELLOW&BLACK
    //static const ColorT AMP[] = {-107.0, +  0.0, +  0.0}, INIT[] = {+128.0, + 10.0, + 10.0}; ///RED&BLACK
    //static const ColorT AMP[] = {-113.0, -113.0, -113.0}, INIT[] = {+140.0, +140.0, +140.0}; ///BLACK&WHITE
    static const complex_t bailout;
    static const complex_t bailout_sqr;
    static constexpr ColorT log10N = 0.90308998699; //log10(bailout);
    static constexpr ColorT log2_log10N = -0.14705834492L; //std::log2(log10N);
    static constexpr unsigned NThreads = 8;

    ///MEMBER VARIABLES
    wxNativePixelData   px;     ///PixelData, to access bmp
    complex_t           step;   ///Difference between consecutive pixels
    ComplexNum          origin; ///Upper-left corner
    ComplexNum          center; ///Center of the fractal
    iter_t          numIt=0;///Total number of iterations performed over the fractal
    const iter_t    addIt;
    ComplexNum          *C=NULL, *Z=NULL; ///Point in complex space, current value of z
    iter_t          *IT=NULL;    ///Number of iterations
    std::list<unsigned> *LCHK=NULL;



    ///PRIVATE FUNCTIONS
    /**
     * Update pixels in [L,R) by making an additional addIt iterations
     */
    void UpdateMathLim(unsigned index, iter_t addIt, std::deque<unsigned>* changed);
    /**
     * Update pixels whose indexes are in q, based on the information in the variables
     */
    void UpdatePixels(const std::deque<unsigned>& v);
    /**
     * Balance lists
     */
    void BalanceLists();

    static ColorT CycleFun(ColorT x);

    ///PRIVATE STATIC FUNCTIONS
    static inline bool isCardioid_isPeriod2Bulb(const ComplexNum& c){
        complex_t q = (c.real()-complex_t(0.25L))*(c.real()-complex_t(0.25L)) + c.imag()*c.imag();    ///isCardioid
        return (complex_t(4.0L)*q*(q+(c.real()-complex_t(0.25L))) < c.imag()*c.imag())||       ///isCardioid
               ((c.real()+complex_t(1.0L))*(c.real()+complex_t(1.0L)) + c.imag()*c.imag() < complex_t(0.0625L)); ///isPeriod2Bulb
    }

public:
    /**
     * Constructor
     */
    mb(iter_t addIter);

    mb(const mb *p);
    /**
     * init
     */
    void init(ComplexNum o, complex_t st, wxSize s, bool IsCenter = false);
    /**
     * clone
     */
    mb* clone() const;
    /**
     * Destructor
     */
    ~mb();

    ///CALCULATIONS ==================================================
    /**
     * Update all pixels by making an additional addIt iterations
     */
    void update();

    ///GET FUNCTION ==================================================
    ComplexNum getOrigin()         const{ return origin; }
    ComplexNum getCenter()         const{ return center; }
    complex_t   getStep()           const{ return step;   }
    complex_t   GetHorizontalSize() const{ return step*(complex_t)GetSize().x; }
    iter_t getNum()            const{ return numIt; }
    iter_t GetCyclesPerRun()   const{ return addIt; }
    iter_t GetNotEscaped()     const;

    ///OTHER UTILITIES
    bool SaveFile(const wxString& name, wxBitmapType type, const wxPalette *palette = NULL) const;
};

#endif
