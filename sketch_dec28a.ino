#include <DHT.h> // include the DHT library
#include <SoftwareSerial.h> // include the SoftwareSerial library

#define DHTTYPE DHT11 // specify the type of DHT sensor you are using
#define DHTPIN 4 // specify the pin the DHT is connected to

DHT dht(DHTPIN, DHTTYPE); // create a DHT object

#define RX_PIN 2 // specify the RX pin for the PMS3003 sensor
#define TX_PIN 3 // specify the TX pin for the PMS3003 sensor

SoftwareSerial pmSerial(RX_PIN, TX_PIN); // create a SoftwareSerial object for the PMS3003 sensor

void setup() {
  // initialize the serial monitor
  Serial.begin(9600);
  // initialize the DHT sensor
  dht.begin();
  // initialize the SoftwareSerial object for the PMS3003 sensor
  pmSerial.begin(9600);
}

void loop() {
  // read the humidity and temperature from the DHT sensor
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  // check if the DHT read was successful
  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
  } else {
    // print the humidity and temperature to the serial monitor
    Serial.print("Humidity: ");
    Serial.print(h);
    Serial.print(" %\t");
    Serial.print("Temperature: ");
    Serial.print(t);
    Serial.println(" *C ");
  }

  // read data from the PMS3003 sensor
  if (pmSerial.available()) {
    // create a buffer to store the incoming data
    byte pmData[32];
    // read the data from the sensor into the buffer
    pmSerial.readBytes(pmData, 32);
    // check if the data is valid by checking the first byte
    if (pmData[0] == 0x42) {
      // calculate the PM2.5 concentration in ug/m^3
      int pm25 = (pmData[13] << 8) + pmData[14];
      // print the PM2.5 concentration to the serial monitor
      Serial.print("PM2.5: ");
      Serial.print(pm25);
      Serial.println(" ug/m^3");
    }
  }
  // wait 1 second before reading the sensors again
  delay(1000);
}
