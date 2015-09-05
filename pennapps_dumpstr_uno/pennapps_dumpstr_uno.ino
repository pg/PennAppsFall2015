/*
 Aduino Uno reads HC-SR04 distance sensor and outputs digital signal to trigger interrupt on Intel Edison + Arduino board
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino Uno pin 7 
 Trig to Arduino Uno pin 8
 
 This sketch originates from Virtualmix: http://goo.gl/kJ8Gl
 Has been modified by Winkle ink here: http://winkleink.blogspot.com.au/2012/05/arduino-hc-sr04-ultrasonic-distance.html
 And modified further by ScottC here: http://arduinobasics.blogspot.com.au/2012/11/arduinobasics-hc-sr04-ultrasonic-sensor.html
 on 10 Nov 2012.
 And modified further by Dumpstr for PennApps Fall 2015 here: https://github.com/pgebhard/PennAppsFall2015 on 04 to 06 Sep 2015
 */


#define echoPin 7 // Echo Pin
#define trigPin 8 // Trigger Pin
#define LEDPin 13 // Onboard LED
#define outputPin 2 // Output to trigger interrupt on Intel Edison board
#define outputLimit 10 // limit to trigger output to Intel Edison board

const int numReadings = 40; // with 50ms delays between readings, will detect objects within 2 seconds

int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance; // Duration used to calculate distance
int readings[numReadings];      // the readings from the sensor input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

void setup() {
 Serial.begin (9600);
 
 // initialize all the sensor readings to 0: 
 for (int thisReading = 0; thisReading < numReadings; thisReading++) {
   readings[thisReading] = 0; Serial.println("init to 0");
 }
 
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
 pinMode(outputPin, OUTPUT);
 
}

void loop() {
/* The following trigPin/echoPin cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 
 digitalWrite(trigPin, LOW); 
 delayMicroseconds(2); 

 digitalWrite(trigPin, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(trigPin, LOW);
 duration = pulseIn(echoPin, HIGH);
 
 //Calculate the distance (in cm) based on the speed of sound.
 distance = duration/58.2;
 
 if (distance >= maximumRange || distance <= minimumRange){
   /* Send a negative number to computer 
   to indicate "out of range" */
   Serial.println("-1");
    
 }
 else {
   /* Send the distance to the computer using Serial protocol */
   Serial.print("distance: ");
   Serial.println(distance);
 
   // subtract the last reading:
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = distance; 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                                               

  // calculate the average:
  average = total / numReadings;         
  // send it to the computer as ASCII digits
  Serial.print("average: ");
  Serial.println(average);
   
   if (average < outputLimit && index == numReadings) {
     /* if calculated average is below the threshold, turn on the LED and output signal high to edison */
     digitalWrite(LEDPin, HIGH);
     digitalWrite(outputPin, HIGH);
     
   }
   
   // if we're at the end of the array...
   if (index >= numReadings) {              
     // ...wrap around to the beginning: 
     index = 0;
   }
 }
 
 //Delay 50ms before next reading.
 delay(50);
}
