#include <DHT.h> // include the DHT library
#include <SoftwareSerial.h> // include the SoftwareSerial library

#define DHTPIN 2 // define the pin the DHT11 is connected to
#define DHTTYPE DHT11 // define the DHT11 sensor type
DHT dht(DHTPIN, DHTTYPE); // create an instance of the DHT class

#define RX_PIN 10 // define the RX pin for the PMS3003
#define TX_PIN 11 // define the TX pin for the PMS3003
SoftwareSerial pmsSerial(RX_PIN, TX_PIN); // create an instance of the SoftwareSerial class

void setup() {
  Serial.begin(9600); // start the serial monitor
  dht.begin(); // start the DHT11 sensor
  pmsSerial.begin(9600); // start the PMS3003 sensor
}

void loop() {
  // read data from the DHT11 sensor
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();

  // print the humidity and temperature to the serial monitor
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%\tTemperature: ");
  Serial.print(temperature);
  Serial.println("°C");

  // read data from the PMS3003 sensor
  if (pmsSerial.available()) {
    byte buffer[24];
    pmsSerial.readBytes(buffer, 24);
    int pm25 = (buffer[16] << 8 | buffer[17]); // get the PM2.5 value
    int pm10 = (buffer[18] << 8 | buffer[19]); // get the PM10 value

    // print the PM2.5 value to the serial monitor
    Serial.print("PM2.5: ");
    Serial.print(pm25);
    Serial.println(" μg/m^3");
  }

  delay(1000); // wait 1 second before reading again
}
