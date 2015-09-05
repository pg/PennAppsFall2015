import mraa

def setup():
    trig = mraa.Gpio(8)
    trig.dir(mraa.DIR_OUT)
    echo = mraa.Gpio(7)
    echo.dir(mraa.DIR_IN)

def loop():
    # Nothing

if __name__ == '__main__':
    setup()
    while 1:
        loop()