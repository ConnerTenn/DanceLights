#include <iostream>
#include <wiringPi.h>

int main()
{
	wiringPiSetup();
	
	wiringPiSetupGpio();
	
	pinMode(17, INPUT);
	digitalWrite(23, HIGH);
	delay(1000);
	digitalWrite(23, LOW);
	

	return 0;
}