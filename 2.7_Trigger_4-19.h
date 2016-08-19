//Trigger_Tiny

//Same as 2_Trigger_4-12 but designed for 7 inputs

///////////
//Library//
///////////

#include <SoftwareSerial.h>
SoftwareSerial mySerial(-1,10);
	
//constants
int threshold = 306;		//voltage (1.5V)
byte seekTime = 1;		//(milliseconds)
int bounceTime = 50;	//time out after strike (miliseconds)

//signals
int signal [7];
int prior [7] = {0,0,0,0,0,0,0};
int x;	//signal during PeakSeek

//Temporary variables
byte pin;
int piezoID;
int peak = 0;
unsigned long time;
unsigned long timer = 0;
unsigned long lastPeakTime = 0;	//was not equal to zero in working Arduino test

//Arrays
int PiezoPin [8] = {0,1,2,3,4,5,6};	//0-7
int noteNumber [8] = {1,2,3,4,5,6,7};	//change for each tiny (1-83)


//////////
//Set-up//
//////////

void setup() {
	for (byte pin = 0; pin <= 6; pin ++){
		pinMode (pin,INPUT);
	}
}

/////////////
//main loop//
/////////////

void loop () {
	for (pin = 0; pin < 8; pin ++) {
		time = millis ();
		if (time - lastPeakTime >= bounceTime) {	//only if enough time has passed for piezo to chill out
			JumpCheck ();	//runs for each pin
		}
	}
}

///////////////////////////////////////
//checks for a jump in signal//////////
//from last time around the main loop//
///////////////////////////////////////

void JumpCheck () {
signal[pin] = analogRead (PiezoPin[pin]);
	if (signal[pin] - prior[pin] > threshold) {
		PeakSeek ();		//determine peek
		Report ();			//write the data
	}
		prior[pin] = signal[pin];	//assigns prior for next execution of JumpCheck
}

/////////////////////////////////
//If JumpCheck detects a strike//
/////////////////////////////////

void PeakSeek () {
//set timers
	time = millis();	//updated throughout loop
	timer = millis();	//updated once per loop
//search for highest value
	while (time - timer <= seekTime) {
		time = millis();
		x = analogRead(PiezoPin[pin]);	
		peak = max (peak,x);		//
	}
	lastPeakTime = time;
}

////////////////////////////////
//Executed after peek is found//
////////////////////////////////

void Report () {
//Adjust peak value and note ID for export
	peak = constrain (peak, 300, 1025);
	peak = map (peak, 300, 1025, 0,127);
	piezoID = noteNumber[pin];
	
//Wait for mega to call on you
	byte call = 1;
			while (call != 0) {
        		call = digitalRead(9);
        	}
		
//Send Data
    pinMode(10,OUTPUT);
    mySerial.begin(31250);
    mySerial.write (piezoID);
    mySerial.write (peak);
	mySerial.end();
//Resets
	pinMode (10,INPUT_PULLUP);  //sets pin to hi Z
	peak = 0;
}