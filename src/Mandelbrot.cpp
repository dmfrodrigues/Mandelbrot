#include "Mandelbrot.h"


#include <fstream>
#include <iomanip>
#include <thread>

///Constants
const mb::ComplexT mb::bailout(8.0L); // 2.0 // 8.0
const mb::ComplexT mb::bailout_sqr = mb::bailout*mb::bailout;



///Constructor
mb::mb(IterationT addIter):FractalBitmap(),px(*((wxBitmap*)this)),addIt(addIter){}
#include <iostream>
///New
void mb::New(ComplexNum o, ComplexT st, wxSize s, bool IsCenter){
    Create(s, 24);
    px = wxNativePixelData(*((wxBitmap*)this));

    const unsigned N = GetWidth()*GetHeight();
    wxNativePixelData::Iterator p = px.GetPixels();
    for(unsigned i = 0; i < N; ++i, ++p){
        p.Red() = p.Green() = p.Blue() = 0;
    }

    step = st;
    origin = (IsCenter? GetOriginFromCenter(o, step, GetSize()) : o);
    center = GetCenterFromOrigin(origin, step, GetSize());
    numIt = 0;
    if(C   !=NULL){ delete[] C ;   } C     = new ComplexNum[N]; //std::cout << sizeof(C) << std::endl;
    if(Z   !=NULL){ delete[] Z ;   } Z     = new ComplexNum[N]; std::fill(Z,Z+N,ComplexNum(ComplexT(0.0L),ComplexT(0.0L)));
    if(IT  !=NULL){ delete[] IT;   } IT    = new IterationT[N]; std::fill(IT,IT+N,0);
    if(LCHK!=NULL){ delete[] LCHK; } LCHK = new std::list<unsigned>[NThreads];

    ///Fill 'C', 'LCHK' with new information
    unsigned long i = 0;
    ComplexNum c = origin;
    for(int y = 0; y < GetHeight(); ++y, c.imag(c.imag()-step)){
        c.real(origin.real());
        for(int x = 0; x < GetWidth(); ++x, c.real(c.real()+step), ++i){
            C[i] = c;
            if(!isCardioid_isPeriod2Bulb(c)) LCHK[i*NThreads/N].push_back(i);
        }
    }
}

///CreateNew
mb* mb::CreateNew(ComplexNum o, ComplexT st, wxSize s, bool IsCenter) const{
    mb *ret = new mb(addIt);
    ret->New(o,st,s,IsCenter);
    return ret;
}

mb::~mb(){
    delete[] C;
    delete[] Z;
    delete[] IT;
    delete[] LCHK;
}

void mb::UpdateMath(){
    std::thread *ArrThreads[NThreads];
    std::deque<unsigned> vchanged[NThreads];
    for(unsigned i = 0; i < NThreads; ++i){
        ArrThreads[i] = new std::thread(&mb::UpdateMathLim, this, i, addIt, &(vchanged[i]));
    }
    for(unsigned i = 0; i < NThreads; ++i) ArrThreads[i]->join();
    for(unsigned i = 0; i < NThreads; ++i) delete ArrThreads[i];
    for(const auto& d:vchanged) UpdatePixels(d);

    numIt += addIt;

    BalanceLists();
}

void mb::UpdateMathLim(unsigned index, IterationT addIt, std::deque<unsigned>* changed){
    changed->clear();
    auto& L = LCHK[index];
    auto j = L.begin();
    bool ESCAPED;
    ComplexT cr, ci;
    ComplexT zr, zi, zi_;
    ComplexT t, n;
    while(j != L.end()){
        ESCAPED = false;
        cr = C[*j].real(); ci = C[*j].imag();

        zr = Z[*j].real(); zi = Z[*j].imag();
        IterationT nit;
        for(nit = addIt+1; --nit;){
            mpf_set(zi_.n, zi.n);

            mpf_mul(zi.n, zr.n, zi.n);
            mpf_mul_ui(zi.n, zi.n, 2);
            mpf_add(zi.n, zi.n, ci.n);

            mpf_mul(zr.n, zr.n, zr.n);
            mpf_mul(t.n, zi_.n, zi_.n);
            mpf_sub(zr.n, zr.n, t.n);
            mpf_add(zr.n, zr.n, cr.n);

            mpf_mul(n.n, zr.n, zr.n);
            mpf_mul(t.n, zi.n, zi.n);
            mpf_add(n.n, n.n, t.n);
            if(mpf_cmp(n.n, bailout_sqr.n) > 0){
                Z[*j].real(zr); Z[*j].imag(zi);
                IT[*j] += addIt-nit;
                changed->push_back(*j);
                j = L.erase(j);
                ESCAPED = true;
                break;
            }
        }
        if(!ESCAPED){
            Z[*j].real(zr); Z[*j].imag(zi);
            IT[*j] += addIt-nit;
            ++j;





        }
    }

}

void mb::UpdatePixels(const std::deque<unsigned>& v){
    wxNativePixelData::Iterator p(px);





    for(const unsigned& i:v){
        p.MoveTo(px, i%GetSize().x, i/GetSize().x);

        //x = (ColorT)IT[i]-3.0*(log2(0.5*log10(Z[i].absSqr()))-log2_log10N); ///continuous/wavy pattern
        //x = (ColorT)IT[i]-1.0L*(log2(0.5*log10(Z[i].absSqr()))-log2_log10N); ///continuous pattern, modified formula
        ColorT x = (ColorT)IT[i]-(0.5L*log10((double)std::norm(Z[i]))/log10N-1.0L); ///continuous pattern, original formula
        //x = (ColorT)IT[i]; ///discrete pattern

        ColorT y = CycleFun(omega*x + phi);
        p.Red()   = AMP[0]*y + INIT[0];
        p.Green() = AMP[1]*y + INIT[1];
        p.Blue()  = AMP[2]*y + INIT[2];
    }
}

bool mb::SaveFile(const wxString& name, wxBitmapType type, const wxPalette *palette) const{
    wxBitmap::SaveFile(name, type, palette);
    std::ofstream ostrm(name.ToStdString() + ".txt");
    ostrm << "timedate\t"    << wxDateTime::Now().Format("%d-%b-%Y %H:%M:%S").c_str() << "\n"
          << "timeelapsed\t" << std::setprecision( 8) << 0.0                          << "\n"
          << "re(c)\t"       << std::setprecision(20) << center.real()                << "\n"
          << "im(c)\t"       << std::setprecision(20) << center.imag()                << "\n"
          << "step\t"        << std::setprecision(20) << step                         << "\n"
          << "size.x\t"      << GetSize().x                                           << "\n"
          << "size.y\t"      << GetSize().y                                           << "\n"
          << "NumIt\t"       << numIt                                                 << "\n" << std::flush;
    ostrm.close();
    return true;
}

void mb::BalanceLists(){
    for(unsigned i = 0; i < NThreads-1; ++i){
        auto &L = LCHK[i], &R = LCHK[i+1];
        while(!R.empty() && L.size() < R.size()){
            L.push_back(R.front()); R.pop_front();
        }
        while(!L.empty() && L.size() > R.size()){
            R.push_front(L.back()); L.pop_back();
        }
    }
}

mb::ColorT mb::CycleFun(mb::ColorT x){
    x = remainderf(x, pi2);
    if(x < -pi_2) x = -pi-x;
    if(+pi_2 < x) x =  pi-x;
    return x/pi_2;                                ///Linear
}

mb::IterationT mb::GetNotEscaped() const{
    unsigned ret = 0;
    for(unsigned i = 0; i < NThreads; ++i)
        ret += LCHK[i].size();
    return ret;
}
