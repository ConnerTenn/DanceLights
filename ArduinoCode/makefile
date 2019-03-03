
all: compile upload

compile:
	[[ -d ./build ]] || mkdir build
	arduino-builder \
		-compile \
		-hardware /usr/share/arduino/hardware \
		-tools /usr/bin \
		-fqbn archlinux-arduino:avr:uno \
		-libraries ~/Git/ \
		-build-path ./build \
		main.ino
		
upload:
	sudo avrdude -p m328p -P /dev/ttyUSB0 -c avrisp -v -U flash:w:build/main.ino.hex -F
	
clean:
	rm build -r
