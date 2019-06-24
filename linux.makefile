GMPARITH=/media/dmfr/DATA/_ProgrammingLibraries/gmp-arith
IFLAGS =$(IDIR) -I$(GMPARITH)
LFLAGS =`wx-config --libs` -L"../fractal-app/lib" -lfractalapp -lgmp -lgmpxx

CC     =g++

SDIR   =./src
IDIR   =-I./include -I../fractal-app/include
ODIR   =./obj
BDIR   =./bin

CFLAGS =-std=c++11 -ffast-math -O3 $(IFLAGS) `wx-config --cxxflags` -c

all: makefolders $(BDIR)/main.app

makefolders:
	mkdir -p obj
	mkdir -p bin

$(BDIR)/main.app:              $(ODIR)/Mandelbrot.o $(ODIR)/FractalApp.o
	$(CC) -o $(BDIR)/main.app $(ODIR)/Mandelbrot.o $(ODIR)/FractalApp.o $(LFLAGS)

$(ODIR)/%.o:          $(SDIR)/%.cpp
	$(CC) $(CFLAGS) $^ -o $@
