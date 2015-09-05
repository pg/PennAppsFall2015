import mraa

# Setup
touch = mraa.Gpio(2)
touch.dir(mraa.DIR_IN)
buzz = mraa.Gpio(7)
buzz.dir(mraa.DIR_OUT)

def loop():
    if touch.read():
        buzz.write(1)
    else:
        buzz.write(0)

if __name__ == '__main__':
    while 1:
        loop()