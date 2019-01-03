
GPP=g++ -Wall -std=c++17
LAUDIO=../libportaudio.a -lasound -ljack -pthread
LMAD=../libmad/*.o 
LPIXTERMLIBS=-pthread -lgdk_pixbuf_xlib-2.0 -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype  -lgtk-x11-2.0 -lgdk-x11-2.0 -lpangocairo-1.0 -latk-1.0 -lcairo -lgdk_pixbuf-2.0 -lgio-2.0 -lpangoft2-1.0 -lpango-1.0 -lgobject-2.0 -lglib-2.0 -lfontconfig -lfreetype -lX11
LPIXTERM=-L ../../PixelTerm -lPixelTerm $(LPIXTERMLIBS)
LIBS = $(LMAD) $(LAUDIO) -lpthread
CFLAGS = -I ../libmad/ -I ../../PixelTerm

all: program

program: main.o LightStrip.o DanceLight.o DanceController.o InputController.o Spectrum.o TextScreen.o Globals.o
	$(GPP) $(CFLAGS) $^ $(LIBS) $(LPIXTERM) -o $@
	
main.o: main.cpp DanceController.h Spectrum.h TextScreen.h Globals.h
	$(GPP) $(CFLAGS) $< -c -o $@
	
LightStrip.o: LightStrip.cpp LightStrip.h DanceLight.h Globals.h
	$(GPP) $(CFLAGS) $< -c -o $@
	
DanceLight.o: DanceLight.cpp DanceLight.h Globals.h
	$(GPP) $(CFLAGS) $< -c -o $@
	
DanceController.o: DanceController.cpp DanceController.h Globals.h
	$(GPP) $(CFLAGS) $< -c -o $@
	
InputController.o: InputController.cpp InputController.h Globals.h
	$(GPP) $(CFLAGS) $< -c -o $@
	
Spectrum.o: Spectrum.cpp Spectrum.h Globals.h 
	$(GPP) $(CFLAGS) $< -c -o $@
	
TextScreen.o: TextScreen.cpp TextScreen.h Globals.h
	$(GPP) $(CFLAGS) $< -c -o $@
	
Globals.o: Globals.cpp Globals.h
	$(GPP) $(CFLAGS) $< -c -o $@

force: clean all

clean: 
	rm *.o program* -f
