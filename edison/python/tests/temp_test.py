import pyupm_i2clcd as lcd
import mraa
import time
import math

# Initialize Jhd1313m1 at 0x3E (LCD_ADDRESS) and 0x62 (RGB_ADDRESS)
myLcd = lcd.Jhd1313m1(0, 0x3E, 0x62)

# Clear and set LCD
myLcd.clear()
myLcd.setCursor(0,0)

# Setup temp sensor
try:
    temp = mraa.Aio(4)
except:
    print ("Are you sure you have an ADC?")

B = 4275

def loop():
    # Green
    myLcd.setColor(0, 255, 0)

    R = 100000 * temp.readFloat()
    temperature = 1.0/(math.log(R/100000.0)/B+1/298.15)-273.15

    # Zero the cursor
    myLcd.setCursor(0,0)
    myLcd.write(str(temperature))
    print (temperature)
    time.sleep(1)

if __name__ == '__main__':
    while 1:
        loop()