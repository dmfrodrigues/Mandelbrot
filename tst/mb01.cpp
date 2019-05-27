#include "Mandelbrot.h"

int main(){
    mb *f = new mb();
    f->New({-1.25L,0.0L}, 1.0L, wxSize(700,400), 2.5L, true);
    for(int i = 0; i < 10; ++i){
        f->UpdateMath(100);
    }
    return 0;
}
