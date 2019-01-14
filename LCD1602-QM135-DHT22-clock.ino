#include <DHT.h>
#include <DHT_U.h>

#include "MQ135.h"

#include <Wire.h> 
#include "DS3231.h" // установить библиотеку 
#include <LiquidCrystal_I2C.h> // скачать старую версию библиотеки 
//int potPin = 0; // initialize analog pin 0 for LM35 temperature sensor
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTClib RTC;

#include "DHT.h"
#define DHTPIN A0     // пин для сигнала поступающего с датчика

//выбор используемого датчика
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)

//инициализация датчика
DHT dht(DHTPIN, DHTTYPE);

MQ135 gasSensor = MQ135(A1); // газовый сенсор


// пин A4 - (SDA)
// пин A5 - (SCL) 
// пин A0 - LM35 out 
// питание 5V

void setup()
{
  Serial.begin(9600);
	// initialize the LCD
 Wire.begin();
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();

lcd.clear();
}

String zero(int num){
  if(num ==1){return "01";}
  else if(num ==2){return "02";}
  else if(num ==3){return "03";}
  else if(num ==4){return "04";}
  else if(num ==5){return "05";}
  else if(num ==6){return "06";}
  else if(num ==7){return "07";}
  else if(num ==8){return "08";}
  else if(num ==9){return "09";}
  }

void loop()
{
DateTime now = RTC.now();

  float h1 = dht.readHumidity();
  int h = (int) h1; 
  // Read temperature as Celsius
  float t1 = dht.readTemperature();
  int t = (int) t1; 

  float ppm = gasSensor.getPPM();
  float rzero = gasSensor.getRZero();
  float rzeroc = gasSensor.getCorrectedRZero(t1,h1);
  float ppmc = gasSensor.getCorrectedPPM(t1,h1);

  int ppmcResult = (int) ppmc;


  Serial.print("Температура = ");
  Serial.println(t1);
  Serial.print("Влажность = ");
  Serial.println(h1);
  Serial.print("PPM = ");
  Serial.println(ppm);
  Serial.print("PPM Corect = ");
  Serial.println(ppmc);
  Serial.print("RZero = ");
  Serial.println(rzero);
  Serial.print("RZero Corect = ");
  Serial.println(rzeroc);
  Serial.println("--------------");
  Serial.println("");
  
  
//int val;// define variable
//int dat;// define variable
//val=analogRead(0);// read the analog value of the sensor and assign it to val
//dat=(125*val)>>8;// temperature calculation formula

  lcd.setCursor(0,0); //Start at character 0 on line 0
  //lcd.print("T: ");
 // delay(100);
  lcd.print(t,1);
  lcd.print("C ");
 // lcd.setCursor(4, 0);            // Устанавливаем курсор в начало 1 строки
    lcd.print(h, 1);                // Выводим на экран значение влажности
    lcd.print("% ");     // Выводим текст
    lcd.print(ppmcResult);     // Выводим CO2
    lcd.print("ppm");     // Выводим CO2
  

    lcd.setCursor(0,1); //Start at character 0 on line 1
    if(now.hour()<10){lcd.print(zero(now.hour()));}
    else{lcd.print(now.hour(), DEC);}
    lcd.print(':');
    if(now.minute()<10){lcd.print(zero(now.minute()));}
    else{lcd.print(now.minute(), DEC);}
    lcd.print(' ');
    if(now.day()<10){lcd.print(zero(now.day()));}
    else{lcd.print(now.day(), DEC);}
    lcd.print('.');
    if(now.month()<10){lcd.print(zero(now.month()));}
    else{lcd.print(now.month(), DEC);}
    lcd.print('.');
    lcd.print(now.year(), DEC);

  delay(10000); // поставил 10 сек т.к. сильно моргал дисплей 
  lcd.clear();
}
