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
 
 /* to use for spi bluetooth comm
 /* ...hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);
*/


#define ECHOPIN              2 // HC-SR04 Echo Pin
#define TRIGPIN              4 // HC-SR04 Trigger Pin
#define LEDPIN              13 // Onboard LED
//#define outputPin 2 // Output to trigger interrupt on Intel Edison board
#define BLUEFRUIT_SPI_CS     8 // SPI chip select
#define BLUEFRUIT_SPI_IRQ    7 // SPI nRF51 -> Arduino 'interrupt'when data is available on the nRF51, indicating that a new SPI transaction should be initiated by the Arduino/MCU
#define BLUEFRUIT_SPI_RST    6 // SPI reset
#define BLUEFRUIT_SPI_SCK   13 // SPI serial clock
#define BLUEFRUIT_SPI_MISO  12 // SPI Master In Slave Out (nRF51 -> Arduino communication)
#define BLUEFRUIT_SPI_MOSI  11 // SPI Master Out Slave In (Arduino -> nRF51 communication)
#define outputLimit         10 // limit to trigger output 

const int numReadings = 40; // with 50ms delays between readings, will detect objects within 2 seconds

int maximumRange = 200;        // Maximum range needed
int minimumRange = 0;          // Minimum range needed
long duration, distance;       // Duration used to calculate distance
int readings[numReadings];     // the readings from the sensor input
int index = 0;                 // the index of the current reading
int total = 0;                 // the running total
int average = 0;               // the average

void setup() {
 Serial.begin (9600);
 
 // initialize all the sensor readings to 0: 
 for (int thisReading = 0; thisReading < numReadings; thisReading++) {
   readings[thisReading] = 0; Serial.println("init to 0");
 }
 
 pinMode(TRIGPIN, OUTPUT);
 pinMode(ECHOPIN, INPUT);
 pinMode(LEDPIN, OUTPUT); // Use LED indicator (if required)
 //pinMode(outputPin, OUTPUT);
 
}

void loop() {
/* The following TRIGPIN/ECHOPIN cycle is used to determine the
 distance of the nearest object by bouncing soundwaves off of it. */ 
 
 digitalWrite(TRIGPIN, LOW); 
 delayMicroseconds(2); 

 digitalWrite(TRIGPIN, HIGH);
 delayMicroseconds(10); 
 
 digitalWrite(TRIGPIN, LOW);
 duration = pulseIn(ECHOPIN, HIGH);
 
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
     digitalWrite(LEDPIN, HIGH);
     //digitalWrite(outputPin, HIGH);
     
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
