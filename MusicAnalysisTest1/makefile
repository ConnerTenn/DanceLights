
GPP=g++ -Wall -std=c++17 -I portaudio/include
LAUDIO=libportaudio.a -lasound -ljack -pthread

all: program1 program2 program3

program1: main.o DanceLight.o Spectrum.o Screen.o Globals.o
	$(GPP) main.o DanceLight.o Spectrum.o Screen.o Globals.o -o program1

program2: main2.o DanceLight.o Spectrum.o Screen.o Globals.o
	$(GPP) main2.o DanceLight.o Spectrum.o Screen.o Globals.o $(LAUDIO) -o program2
	
program3: main3.o DanceLight.o Spectrum.o Screen.o Globals.o
	$(GPP) main3.o DanceLight.o Spectrum.o Screen.o Globals.o $(LAUDIO) -o program3
	
main.o: main.cpp DanceLight.h Spectrum.h Screen.h Globals.h
	$(GPP) main.cpp -c -o main.o

main2.o: main2.cpp DanceLight.h Spectrum.h Screen.h Globals.h
	$(GPP) main2.cpp -c -o main2.o
	
main3.o: main3.cpp DanceLight.h Spectrum.h Screen.h Globals.h
	$(GPP) main3.cpp -c -o main3.o
	
DanceLight.o: DanceLight.cpp DanceLight.h Spectrum.h Screen.h Globals.h
	$(GPP) DanceLight.cpp -c -o DanceLight.o
	
Spectrum.o: Spectrum.cpp Spectrum.h Globals.h 
	$(GPP) Spectrum.cpp -c -o Spectrum.o
	
Screen.o: Screen.cpp Screen.h Globals.h
	$(GPP) Screen.cpp -c -o Screen.o
	
Globals.o: Globals.cpp Globals.h
	$(GPP) Globals.cpp -c -o Globals.o
	
	
audio: Audio.c
	$(GPP) Audio.c $(LAUDIO) -o audio

clean: 
	rm *.o program* -f
