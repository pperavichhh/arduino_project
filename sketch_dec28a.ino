#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h> // include I2C library
#include <BlynkSimpleSerialBLE.h>

char auth[] = "YourAuthToken";

SoftwareSerial mySerial(2,3); // RX, TX
unsigned int pm1 = 0;
unsigned int pm2_5 = 0;
unsigned int pm10 = 0;

#define I2C_ADDR 0x27 // I2C address of the LCD
#define LCD_COL 16 // Number of columns in the LCD
#define LCD_ROW 2 // Number of rows in the LCD
LiquidCrystal_I2C lcd(I2C_ADDR, LCD_COL, LCD_ROW);

void setup() {
  Serial.begin(9600);
  while (!Serial);
  mySerial.begin(9600);
  
  lcd.begin(); // initialize the LCD
  lcd.setCursor(0, 0); // set cursor to the first row and first column
  Blynk.begin(auth);
}

void loop() {
  int index = 0;
  char value;
  char previousValue;

  while (mySerial.available()) {
    value = mySerial.read();
    if ((index == 0 && value != 0x42) || (index == 1 && value != 0x4d)){
      Serial.println("Cannot find the data header .");
      break;
    }

    if (index == 4 || index == 6 || index == 8 || index == 10 || index == 12 || index == 14) {
      previousValue = value;
    }
    else if (index == 5) {
      pm1 = 256 * previousValue + value;
      Serial.print("{ ");
      Serial.print("\"pm1\": ");
      Serial.print(pm1);
      Serial.print(" ug/m3");
      Serial.print(", ");
      
      lcd.print("PM1: "); // display PM1 on the first row
      lcd.print(pm1);
      lcd.print(" ug/m3");
    }
    else if (index == 7) {
      pm2_5 = 256 * previousValue + value;
      Serial.print("\"pm2_5\": ");
      Serial.print(pm2_5);
      Serial.print(" ug/m3");
      Serial.print(", ");
      
      lcd.setCursor(0, 1); // move cursor to the second row
      lcd.print("PM2.5: "); // display PM2.5 on the second row
      lcd.print(pm2_5);
      lcd.print(" ug/m3");
    }
    else if (index == 9) {
      pm10 = 256 * previousValue + value;
      Serial.print("\"pm10\": ");
      Serial.print(pm10);
      Serial.print(" ug/m3");
      
      lcd.setCursor(1, 0); // move cursor to the second row, first column
      lcd.print("PM10: "); // display PM10 on the second row
      lcd.print(pm10);
      lcd.print(" ug/m3");
     
      Blynk.virtualWrite(V1, pm1);
      Blynk.virtualWrite(V2, pm2_5);
      Blynk.virtualWrite(V3, pm10);

    } else if (index > 15) {
      break;
    }
    index++;
  }
  while(mySerial.available()) mySerial.read();
  Serial.println(" }");
  delay(1000);
}


 
