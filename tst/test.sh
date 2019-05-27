g++ -I../include/ mb01.cpp ../src/FractalBitmap.cpp ../src/Mandelbrot.cpp `wx-config --cxxflags --libs` -o mb01.app
./mb01.app
