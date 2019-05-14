CC=g++
CFLAGS=-Wall -ID:\_ProgrammingLibraries\wxWidgets-3.0.4\lib\gcc_dll_SHARED_RELEASE_MONOLITHIC_UNICODE\mswu `wx-config --cxxflags --libs`

mandelbrotmake: Mandelbrot.h Mandelbrot.cpp
	$(CC) -c $(CFLAGS) Mandelbrot.cpp -o obj/Mandelbrot.o
