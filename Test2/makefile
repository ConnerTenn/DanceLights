
GPP=g++ -Wall -std=c++17
LAUDIO=../libportaudio.a -lasound -ljack -pthread
LMAD=../libmad/*.o -I ../libmad/
LIBS = $(LMAD) -lpthread

all: program

program: main.o LightStrip.o DanceLight.o DanceController.o InputController.o Spectrum.o Screen.o Globals.o
	$(GPP) $^ $(LIBS) -o $@
	
main.o: main.cpp DanceController.h Spectrum.h Screen.h Globals.h
	$(GPP) $< -c -o $@
	
LightStrip.o: LightStrip.cpp LightStrip.h DanceLight.h Globals.h
	$(GPP) $< -c -o $@
	
DanceLight.o: DanceLight.cpp DanceLight.h Globals.h
	$(GPP) $< -c -o $@
	
DanceController.o: DanceController.cpp DanceController.h Globals.h
	$(GPP) $< -c -o $@
	
InputController.o: InputController.cpp InputController.h Globals.h
	$(GPP) $< -c -o $@
	
Spectrum.o: Spectrum.cpp Spectrum.h Globals.h 
	$(GPP) $< -c -o $@
	
Screen.o: Screen.cpp Screen.h Globals.h
	$(GPP) $< -c -o $@
	
Globals.o: Globals.cpp Globals.h
	$(GPP) $< -c -o $@

force: clean all

clean: 
	rm *.o program* -f
