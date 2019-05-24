#include "Mandelbrot.h"

#include <thread>

void mb::UpdateMath(IterationT addIt){
    std::thread *ArrThreads[NThreads];
    for(unsigned long L, R, i = 0; i < NThreads; ++i){
        L =  i   *N/NThreads;
        R = (i+1)*N/NThreads;
        ArrThreads[i] = new std::thread(&mb::UpdateMathLim, this, L, R, addIt);
    }
    for(unsigned long i = 0; i < NThreads; ++i){
        ArrThreads[i]->join();
    }
    numIt += addIt;
}

void mb::UpdateMathLim(IterationT addIt){
    unsigned long i = threadIdx.x;
    unsigned long NThreads = blockDim.x;
    unsigned long N = GetSize().x*GetSize().y;
    unsigned long L =  i   *N/NThreads;
    unsigned long R = (i+1)*N/NThreads;
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
