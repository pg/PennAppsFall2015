Dumpstr is an innovate way for business managers to save money and reduce resources needed for effective waste management. Users can attach a dumpstr sensor to an existing dumpster in order to connect with an iOS application that tracks whether or not the dumpster is full and lists past pickup dates for reference.

The main sensing system consists of an ultrasonic range sensor providing feedback about the status of a dumpster’s fullness. The sensor is integrated with an Arduino Uno, and the Uno communicates with the iOS application via an Adafruit Bluefruit LE SPI Friend to send data to the application over a Bluetooth LE connection.

Sensing a full dumpster, the iOS application informs the user to take out the trash and adds the date to the displayed past pickup list. Currently, the user must handle the situation however he/she chooses. In the future, we envision dumpstr as a pickup service that handles waste disposal seamlessly by using the gathered data to generate daily routes.  

Another possible use for dumpstr involves disposing of city waste in a more efficient manner. The dumpstr sensor can be attached to garbage cans as well, and with the help of users with the downloaded app, the information can be passed to the dumpstr service. City trash would be disposed of in a more efficient manner with the use of this application.      

Built With: Swift, iOS, Arduino, Adafruit Bluefruit LE SPI Friend, Ultrasonic range sensor
