#include <Wire.h>
#include <LiquidCrystal_I2C.h> // LCD library
#include <AltSoftSerial.h> //Bluetooth library
#include <Relay.h> //Relay library
#include <DS3231.h> //Time library


DS3231  rtc(SDA, SCL); //Init the DS3231 using the hardware interface
LiquidCrystal_I2C lcd(0x27,20,4); //Set the LCD address to 0x27 for the display
  
int airQualtity = 0; //Random inintialization
AltSoftSerial btSerial; //Serial for Bluetooth

void setup() {
  Serial.begin(115200); //115200 because Bluetooth Serial is on 9600
  rtc.begin();

  pinMode(10, OUTPUT);  
  noTone(10);
  //tone(10, 1);

  pinMode(7, OUTPUT); //Set pin 7 as output for the relay
  digitalWrite(7, LOW); //Set the relay off 

  btSerial.begin(9600); //Begin the Bluetooth connection
  
  lcd.init(); //setup the display
  lcd.backlight();
  lcd.setCursor(0,0);  // Set the cursor on (0, 0)
  lcd.print("Air Quality: ");

  lcd.setCursor(0,2);  // Set the cursor on (0, 0)
  lcd.print("Data: ");
 
}

void airQualityDisplay()
{
    int sensorValue = analogRead(A0);
    lcd.setCursor(13,0);
    lcd.print(sensorValue);
    lcd.setCursor(13,1);
    if (sensorValue > 100)
      if (sensorValue < 500) 
      {
        lcd.print("GOOD ");
        //noTone(10);
      }
      else 
      {
        //tone(10, 1000);
        lcd.print("TOXIC");
      }
    else 
    {
      //tone(10, 1000);
      lcd.print("BAD  ");
    }
    if (sensorValue > 100)
      if (sensorValue < 600) 
      {
        btSerial.write("GOOD\n");
        Serial.write("GOOD\n");
        delay(500);
      }
      else 
      {
        btSerial.write("TOXIC\n");
        Serial.write("TOXIC\n");
        delay(500);
      }
    else 
    {
      btSerial.write("BAD\n"); 
      Serial.write("BAD\n");
      delay(500);
    }
}

void dataDisplay()
{
  lcd.setCursor(7,2);
  lcd.print(rtc.getDateStr());
  lcd.setCursor(0,3);
  lcd.print(rtc.getTimeStr());
  lcd.setCursor(10,3);
  lcd.print(rtc.getDOWStr());
}


void airConditioning(){
  char ok;
  if (btSerial.available()) {
    ok = btSerial.read();
    if(ok == '1')
      digitalWrite(7, LOW);
    if(ok == '0')
      digitalWrite(7, HIGH);
  }
}

void loop() {
  airQualityDisplay();
  dataDisplay();
  airConditioning();
    
}
