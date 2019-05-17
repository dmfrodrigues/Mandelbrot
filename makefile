CC     ="C:\Program Files (x86)\CodeBlocks\MinGW\bin\mingw32-g++.exe"
IDIR   =-I"D:\_ProgrammingLibraries\wxWidgets-3.0.4\include" -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4" -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4\lib\gcc_dll_SHARED_RELEASE_MONOLITHIC_UNICODE\mswu"
CFLAGS =-std=c++11 -ffast-math -pipe -mthreads -D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE -O3 $(IDIR) -c

ODIR   =./build/obj
BDIR   =./build/bin

#LFLAGS =-L"D:\_ProgrammingLibraries\wxWidgets-3.0.4\lib\gcc_dll_SHARED_RELEASE_MONOLITHIC_UNICODE"
LFLAGS =-L"..\..\..\..\..\..\_ProgrammingLibraries\wxWidgets-3.0.4\lib\gcc_dll_SHARED_RELEASE_MONOLITHIC_UNICODE"

all: Linkmake

Linkmake:       Mandelbrot.o FractalFrame.o FractalPanel.o InfoPanel.o main.o
    $(CC) $(LFLAGS) -o $(BDIR)/main.exe Mandelbrot.o FractalFrame.o FractalPanel.o InfoPanel.o main.o
Mandelbrot.o:   Mandelbrot.cpp
	$(CC) $(CFLAGS) Mandelbrot.cpp   -o $(ODIR)/Mandelbrot.o
FractalFrame.o: FractalFrame.cpp
	$(CC) $(CFLAGS) FractalFrame.cpp -o $(ODIR)/FractalFrame.o
FractalPanel.o: FractalPanel.cpp
	$(CC) $(CFLAGS) FractalPanel.cpp -o $(ODIR)/FractalPanel.o
InfoPanel.o:    InfoPanel.cpp
	$(CC) $(CFLAGS) InfoPanel.cpp    -o $(ODIR)/InfoPanel.o
main.o:         main.cpp
	$(CC) $(CFLAGS) main.cpp         -o $(ODIR)/main.o
