PROG=mandelbrot

MCAP=/media/DATA/dmfro/projects/mcap
IFLAGS =$(IDIR) -I$(MCAP)
LFLAGS =`wx-config --libs` -L"fractal-app/lib" -lfractalapp

CC     =g++

SDIR   =./src
IDIR   =-I./include -I./fractal-app/include
ODIR   =./obj

CFLAGS =-std=c++11 -ffast-math -O3 $(IFLAGS) `wx-config --cxxflags` -c

all: makefolders $(PROG)

makefolders:
	mkdir -p obj
	mkdir -p bin

$(PROG):             $(ODIR)/Mandelbrot.o $(ODIR)/FractalApp.o
	make -C fractal-app
	$(CC) -o $(PROG) $(ODIR)/Mandelbrot.o $(ODIR)/FractalApp.o $(LFLAGS)

$(ODIR)/%.o:          $(SDIR)/%.cpp
	$(CC) $(CFLAGS) $^ -o $@

clean:
	rm -rf lib
	rm -rf obj
