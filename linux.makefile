
IFLAGS =$(IDIR) -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\include" -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new" -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\lib\gcc_dll_UNICODE_MONOLITHIC_RELEASE\mswu" -I"D:\_ProgrammingLibraries\mcap"
LFLAGS =`wx-config --libs` -L"../fractal-app/lib" -lfractalapp

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
