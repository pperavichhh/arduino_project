#include <SoftwareSerial.h>
#include <DHT.h>



SoftwareSerial mySerial(2,3); // RX, TX
unsigned int pm1 = 0;
unsigned int pm2_5 = 0;
unsigned int pm10 = 0;
void setup() {
  Serial.begin(9600);
  while (!Serial) ;
    mySerial.begin(9600);
}
 
void loop() {
  int index = 0;
  char value;
  char previousValue;
 
  while (mySerial.available()) {
    value = mySerial.read();
    if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d)){
      Serial.println("Cannot find the data header.");
      break;
    } 
 
    if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14) {
      previousValue = value;
    }

    else if (index == 7) {
      pm2_5 = 256 * previousValue + value;
      Serial.print("\"pm2_5\": ");
      Serial.print(pm2_5);
      Serial.print(" ug/m3");
      Serial.print(", ");
    }
  
  index++;
}
while(mySerial.available()) mySerial.read();
Serial.println(" }");
delay(1000);
}