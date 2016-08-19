//Pseudo IPC without calling the library or syc-clk
//super simple but works great with TriggerTiny_5_4_15
//18 is a problem because it does not communicate serially 

#include <SoftwareSerial.h>
SoftwareSerial mySerial(50,-1);	//used to receive data from all tinies



void setup(){
	mySerial.begin(31250);
	Serial.begin(9600);
	for(byte pin = 2; pin <=17; pin ++) {
		pinMode(pin,OUTPUT);
	}
	for(byte pin = 2; pin <=17; pin ++) {
		pinMode (pin, HIGH);
	}
}

void loop(){
	
	for (byte pin = 2; pin <= 17; pin ++) {
		digitalWrite (pin,LOW);
		delayMicroseconds(100);
		while (mySerial.available()){
			byte that = mySerial.read();
			Serial.println(that);
		}
		digitalWrite (pin,HIGH);
	}
}
		