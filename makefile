PROG=mandelbrot

#MCAP=../mcap
IFLAGS =$(IDIR) #-I$(MCAP)
LFLAGS =`wx-config --libs` -L"fractal-app/lib" -lfractalapp

CC     =g++

SDIR   =./src
IDIR   =-I./include -I./fractal-app/include
ODIR   =./obj

CFLAGS =-std=c++11 -g $(IFLAGS) `wx-config --cxxflags` # -ffast-math -O3

all: makefolders $(PROG)

makefolders:
	mkdir -p obj
	mkdir -p bin

$(PROG):             $(ODIR)/Mandelbrot.o $(ODIR)/FractalApp.o
	make -C fractal-app
	$(CC) -o $(PROG) $(ODIR)/Mandelbrot.o $(ODIR)/FractalApp.o $(LFLAGS)

$(ODIR)/%.o:          $(SDIR)/%.cpp
	$(CC) $(CFLAGS) -c $^ -o $@

clean:
	make -C fractal-app clean
	rm -rf bin
	rm -rf obj

cleanall:
	git clean -dfX
