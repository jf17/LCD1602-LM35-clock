#include <Wire.h> 
#include "DS3231.h" // установить библиотеку 
#include <LiquidCrystal_I2C.h> // скачать старую версию библиотеки 
int potPin = 0; // initialize analog pin 0 for LM35 temperature sensor
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTClib RTC;


// пин A4 - (SDA)
// пин A5 - (SCL) 
// пин A0 - LM35 out 
// питание 5V

void setup()
{
	// initialize the LCD
 Wire.begin();
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();

lcd.clear();
}

void loop()
{
DateTime now = RTC.now();
  
int val;// define variable
int dat;// define variable
val=analogRead(0);// read the analog value of the sensor and assign it to val
dat=(125*val)>>8;// temperature calculation formula

  lcd.setCursor(0,0); //Start at character 0 on line 0
  lcd.print("Temp: ");
 // delay(100);
  lcd.print(dat);
  lcd.print("C");

    lcd.setCursor(0,1); //Start at character 0 on line 1
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    lcd.print(now.minute(), DEC);
    lcd.print(' ');
    lcd.print(now.day(), DEC);
    lcd.print('/');
    lcd.print(now.month(), DEC);
    lcd.print('/');
    lcd.print(now.year(), DEC);

  delay(10000); // поставил 10 сек т.к. сильно моргал дисплей 
  lcd.clear();
}
