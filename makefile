CC     =g++

SDIR   =./src
IDIR   =./include
ODIR   =./obj
BDIR   =./bin

IFLAGS =-I$(IDIR) -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\include" -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new" -I"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\lib\gcc_dll_UNICODE_MONOLITHIC_RELEASE\mswu" -I"D:\_ProgrammingLibraries\mcap"
CFLAGS =-MMD -std=c++11 -ffast-math -pipe -mthreads -D__GNUWIN32__ -D__WXMSW__ -DWXUSINGDLL -DwxUSE_UNICODE -O3 $(IFLAGS) -c

#LFLAGS =-L"D:\_ProgrammingLibraries\wxWidgets-3.0.4\lib\gcc_dll_SHARED_RELEASE_MONOLITHIC_UNICODE"
LFLAGS =-L"D:\_ProgrammingLibraries\wxWidgets-3.0.4-new\lib\gcc_dll_UNICODE_MONOLITHIC_RELEASE"

all: makefolders $(BDIR)/main.exe

makefolders:
	mkdir -p obj
	mkdir -p bin

$(BDIR)/main.exe:                       $(ODIR)/FractalBitmap.o $(ODIR)/Mandelbrot.o $(ODIR)/FractalFrame.o $(ODIR)/FractalPanel.o $(ODIR)/InfoPanel.o $(ODIR)/HDPrintscreenDialog.o $(ODIR)/FractalApp.o
	$(CC) $(LFLAGS) -o $(BDIR)/main.exe $(ODIR)/FractalBitmap.o $(ODIR)/Mandelbrot.o $(ODIR)/FractalFrame.o $(ODIR)/FractalPanel.o $(ODIR)/InfoPanel.o $(ODIR)/HDPrintscreenDialog.o $(ODIR)/FractalApp.o -s -mthreads -lwxmsw30u -mwindows
$(ODIR)/FractalBitmap.o:       $(SDIR)/FractalBitmap.cpp
	$(CC) $(CFLAGS)            $(SDIR)/FractalBitmap.cpp       -o $(ODIR)/FractalBitmap.o
$(ODIR)/Mandelbrot.o:          $(SDIR)/Mandelbrot.cpp
	$(CC) $(CFLAGS)            $(SDIR)/Mandelbrot.cpp          -o $(ODIR)/Mandelbrot.o
$(ODIR)/FractalFrame.o:        $(SDIR)/FractalFrame.cpp
	$(CC) $(CFLAGS)            $(SDIR)/FractalFrame.cpp        -o $(ODIR)/FractalFrame.o
$(ODIR)/FractalPanel.o:        $(SDIR)/FractalPanel.cpp
	$(CC) $(CFLAGS)            $(SDIR)/FractalPanel.cpp        -o $(ODIR)/FractalPanel.o
$(ODIR)/InfoPanel.o:           $(SDIR)/InfoPanel.cpp
	$(CC) $(CFLAGS)            $(SDIR)/InfoPanel.cpp           -o $(ODIR)/InfoPanel.o
$(ODIR)/HDPrintscreenDialog.o: $(SDIR)/HDPrintscreenDialog.cpp
	$(CC) $(CFLAGS)            $(SDIR)/HDPrintscreenDialog.cpp -o $(ODIR)/HDPrintscreenDialog.o
$(ODIR)/FractalApp.o:          $(SDIR)/FractalApp.cpp
	$(CC) $(CFLAGS)            $(SDIR)/FractalApp.cpp          -o $(ODIR)/FractalApp.o

-include $(ODIR)/*.d
