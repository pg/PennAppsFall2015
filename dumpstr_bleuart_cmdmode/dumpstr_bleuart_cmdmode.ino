/*********************************************************************
 This is an example for our nRF51822 based Bluefruit LE modules

 Pick one up today in the adafruit shop!

 Adafruit invests time and resources providing this open source code,
 please support Adafruit and open-source hardware by purchasing
 products from Adafruit!

 MIT license, check LICENSE for more information
 All text above, and the splash screen below must be included in
 any redistribution
 
 This sketch originates from the bleuart_cmdmode sketch for adafruit nRF51822 based Bluefruit LE modules at https://github.com/adafruit/Adafruit_BluefruitLE_nRF51
 Modified by Dumpstr for PennApps Fall 2015 here: https://github.com/pgebhard/PennAppsFall2015 on 04 to 06 Sep 2015
 
 Aduino Uno reads HC-SR04 distance sensor and sends a message over bluetooth to the dumpstr iOS app.
 
 HC-SR04 Ping distance sensor:
 VCC to arduino 5v 
 GND to arduino GND
 Echo to Arduino Uno pin 7 
 Trig to Arduino Uno pin 8
 
 nRF51822 based Bluefruit LE modules
 VCC to arduino 5v
 GND to arduino GND
 RST to adruino digital 6
 IRQ to adruino digital 7
 CS to adruino digital 8
 MOSI to adruino digital 11 
 MISO to adruino digital 12
 SCK to adruino digital 13
*********************************************************************/

#include <Arduino.h>
#include <SPI.h>
#if not defined (_VARIANT_ARDUINO_DUE_X_) && not defined (_VARIANT_ARDUINO_ZERO_)
  #include <SoftwareSerial.h>
#endif

#include "Adafruit_BLE.h"
#include "Adafruit_BluefruitLE_SPI.h"
#include "Adafruit_BluefruitLE_UART.h"

#include "BluefruitConfig.h"

/*=========================================================================
    APPLICATION SETTINGS

    FACTORYRESET_ENABLE       Perform a factory reset when running this sketch
   
                              Enabling this will put your Bluefruit LE module
                              in a 'known good' state and clear any config
                              data set in previous sketches or projects, so
                              running this at least once is a good idea.
   
                              When deploying your project, however, you will
                              want to disable factory reset by setting this
                              value to 0.  If you are making changes to your
                              Bluefruit LE device via AT commands, and those
                              changes aren't persisting across resets, this
                              is the reason why.  Factory reset will erase
                              the non-volatile memory where config data is
                              stored, setting it back to factory default
                              values.
       
                              Some sketches that require you to bond to a
                              central device (HID mouse, keyboard, etc.)
                              won't work at all with this feature enabled
                              since the factory reset will clear all of the
                              bonding data stored on the chip, meaning the
                              central device won't be able to reconnect.
    MINIMUM_FIRMWARE_VERSION  Minimum firmware version to have some new features
    MODE_LED_BEHAVIOUR        LED activity, valid options are
                              "DISABLE" or "MODE" or "BLEUART" or
                              "HWUART"  or "SPI"  or "MANUAL"
    -----------------------------------------------------------------------*/
    #define FACTORYRESET_ENABLE         1
    #define MINIMUM_FIRMWARE_VERSION    "0.6.6"
    #define MODE_LED_BEHAVIOUR          "MODE"
/*=========================================================================*/

#define ECHOPIN              2 // HC-SR04 Echo Pin
#define TRIGPIN              4 // HC-SR04 Trigger Pin
#define LEDPIN              13 // Onboard LED
#define outputLimit         10 // limit to trigger output 

const int numReadings = 40; // with 50ms delays between readings, will detect objects within 2 seconds

int maximumRange = 200;        // Maximum range needed
int minimumRange = 0;          // Minimum range needed
long duration, distance;       // Duration used to calculate distance
int readings[numReadings];     // the readings from the sensor input
int index = 0;                 // the index of the current reading
int total = 0;                 // the running total
int average = 0;               // the average

/* Create the bluefruit object using hardware SPI, using SCK/MOSI/MISO hardware SPI pins and then user selected CS/IRQ/RST */
Adafruit_BluefruitLE_SPI ble(BLUEFRUIT_SPI_CS, BLUEFRUIT_SPI_IRQ, BLUEFRUIT_SPI_RST);

// A small helper
void error(const __FlashStringHelper*err) {
  Serial.println(err);
  while (1);
}

/**************************************************************************/
/*!
    @brief  Sets up the HW an the BLE module (this function is called
            automatically on startup)
*/
/**************************************************************************/
void setup(void)
{
  while (!Serial);  // required for Flora & Micro
  delay(500);

  Serial.begin(115200);
  Serial.println(F("Adafruit Bluefruit Command Mode Example"));
  Serial.println(F("---------------------------------------"));

  /* Initialise the module */
  Serial.print(F("Initialising the Bluefruit LE module: "));

  if ( !ble.begin(VERBOSE_MODE) )
  {
    error(F("Couldn't find Bluefruit, make sure it's in CoMmanD mode & check wiring?"));
  }
  Serial.println( F("OK!") );

  if ( FACTORYRESET_ENABLE )
  {
    /* Perform a factory reset to make sure everything is in a known state */
    Serial.println(F("Performing a factory reset: "));
    if ( ! ble.factoryReset() ){
      error(F("Couldn't factory reset"));
    }
  }

  /* Disable command echo from Bluefruit */
  ble.echo(false);

  Serial.println("Requesting Bluefruit info:");
  /* Print Bluefruit information */
  ble.info();

  Serial.println(F("Please use Adafruit Bluefruit LE app to connect in UART mode"));
  Serial.println(F("Then Enter characters to send to Bluefruit"));
  Serial.println();

  ble.verbose(false);  // debug info is a little annoying after this point!

  /* Wait for connection */
  while (! ble.isConnected()) {
      delay(500);
  }

  // LED Activity command is only supported from 0.6.6
  if ( ble.isVersionAtLeast(MINIMUM_FIRMWARE_VERSION) )
  {
    // Change Mode LED Activity
    Serial.println(F("******************************"));
    Serial.println(F("Change LED activity to " MODE_LED_BEHAVIOUR));
    ble.sendCommandCheckOK("AT+HWModeLED=" MODE_LED_BEHAVIOUR);
    Serial.println(F("******************************"));
  }
  
  // initialize all the sensor readings to 0: 
  for (int thisReading = 0; thisReading < numReadings; thisReading++) {
    readings[thisReading] = 0; Serial.println("init to 0");
  }
  
  pinMode(TRIGPIN, OUTPUT);
  pinMode(ECHOPIN, INPUT);
  pinMode(LEDPIN, OUTPUT); // Use LED indicator (if required)

}

/**************************************************************************/
/*!
    @brief  Constantly poll for new command or response data
*/
/**************************************************************************/
void loop(void)
{
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
     Serial.print("[Send] ");
     Serial.println("full");
     ble.print("AT+BLEUARTTX=");
     ble.println("full");
     
   }
   
   // if we're at the end of the array...
   if (index >= numReadings) {              
     // ...wrap around to the beginning: 
     index = 0;
   }
 }
  /*
  // test msg every 1 second
  Serial.print("[Send] ");
  Serial.println("test message from dumpstr");
  ble.print("AT+BLEUARTTX=");
  ble.println("test message from dumpstr");
  */
  
  // Check for user input
  char inputs[BUFSIZE+1];

  if ( getUserInput(inputs, BUFSIZE) )
  {
    // Send characters to Bluefruit
    Serial.print("[Send] ");
    Serial.println(inputs);

    ble.print("AT+BLEUARTTX=");
    ble.println(inputs);

    // check response stastus
    if (! ble.waitForOK() ) {
      Serial.println(F("Failed to send?"));
    }
  }

  // Check for incoming characters from Bluefruit
  ble.println("AT+BLEUARTRX");
  ble.readline();
  if (strcmp(ble.buffer, "OK") == 0) {
    // no data
    return;
  }
  // Some data was found, its in the buffer
  Serial.print(F("[Recv] ")); Serial.println(ble.buffer);
  ble.waitForOK();
  
  //Delay 50ms before next reading.
 delay(50);
}

/**************************************************************************/
/*!
    @brief  Checks for user input (via the Serial Monitor)
*/
/**************************************************************************/
bool getUserInput(char buffer[], uint8_t maxSize)
{
  // timeout in 100 milliseconds
  TimeoutTimer timeout(100);

  memset(buffer, 0, maxSize);
  while( (Serial.peek() < 0) && !timeout.expired() ) {}

  if ( timeout.expired() ) return false;

  delay(2);
  uint8_t count=0;
  do
  {
    count += Serial.readBytes(buffer+count, maxSize);
    delay(2);
  } while( (count < maxSize) && !(Serial.peek() < 0) );

  return true;
}
