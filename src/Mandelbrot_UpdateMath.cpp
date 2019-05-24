#include "Mandelbrot.h"

#include <thread>

const unsigned NThreads = 8;
void FractalBitmap::UpdateMath(IterationT addIt){
    std::thread *ArrThreads[NThreads];
    for(unsigned long L, R, i = 0; i < NThreads; ++i){
        L =  i   *N/NThreads;
        R = (i+1)*N/NThreads;
        ArrThreads[i] = new std::thread(&FractalBitmap::UpdateMathLim, this, L, R, addIt);
    }
    for(unsigned long i = 0; i < NThreads; ++i){
        ArrThreads[i]->join();
    }
    numIt += addIt;
}

void FractalBitmap::UpdateMathLim(unsigned long L, unsigned long R, IterationT addIt){
    for(unsigned long i = L; i < R; ++i){
        if(!Check[i]) continue;
        IterationT it;
        ComplexNum z = Z[i], c = C[i];
        for(it = 0; it < addIt; ++it){
            z = z*z + c;
            if(std::norm(z) > bailout_sqr){
                Z[i] = z; IT[i] += it;
                Check[i] = false;
                UpdatePixel(i);
                break;
            }
        }
        if(Check[i]){
            Z[i] = z;
            IT[i] += it;
        }
    }
}
