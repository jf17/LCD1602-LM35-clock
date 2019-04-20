/*
#include <DHT.h>
#include <DHT_U.h>
#include "DHT.h"
#define DHTPIN A0     // пин для сигнала поступающего с датчика

//выбор используемого датчика
//#define DHTTYPE DHT11   // DHT 11 
#define DHTTYPE DHT22   // DHT 22  (AM2302)
//#define DHTTYPE DHT21   // DHT 21 (AM2301)
//инициализация датчика
DHT dht(DHTPIN, DHTTYPE);
*/

// Бошевский датчик температуры влажности и давления
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)
Adafruit_BME280 bme; // I2C
//unsigned long delayTime;
//----------------------------------------------
/*
#include <SD.h>
const int PIN_CHIP_SELECT = 4;
*/

#define pwmPin 5
//long th, tl, h, l, ppm;
//int prevVal = LOW;


// датчик CO2 плюс выпрямитель логического уровня
#include <SoftwareSerial.h>
SoftwareSerial mySerial(A1,A2);
int ppmcResult;
byte cmd[9] = {0xFF, 0x01, 0x86, 0x00 , 0x00,0x00 , 0x00, 0x00, 0x79 };
unsigned char response[9];
uint8_t sec10=0;
bool flagTime;
//-----------------------------------------------------------


#include <Wire.h> 
#include "DS3231.h" // датчик времени
#include <LiquidCrystal_I2C.h> // скачать старую версию библиотеки 
//LiquidCrystal_I2C lcd(0x27, 16, 2);
LiquidCrystal_I2C lcd(0x27, 20, 4);
RTClib RTC;



void setup()
{ 
  Serial.begin(9600);
	// initialize the LCD
 mySerial.begin(9600);
// pinMode(pwmPin, INPUT); // газовый сенсор новый
 Wire.begin();
	lcd.begin();

	// Turn on the blacklight and print a message.
	lcd.backlight();

lcd.clear();


// проверка бошевского датчика
   bool status;
    
    // default settings
    status = bme.begin();
    if (!status) {
        Serial.println("Could not find a valid BME280 sensor, check wiring!");
        while (1);
    }
    
    Serial.println("-- Default Test --");

    Serial.println();

    delay(100); // let sensor boot up 
// ---------------------------------------
/*
 Serial.print("Initializing SD card...");
 // Этот пин обязательно должен быть определен как OUTPUT
  pinMode(10, OUTPUT);

  // Пытаемся проинициализировать модуль
  if (!SD.begin(PIN_CHIP_SELECT)) {
    Serial.println("Card failed, or not present");
    // Если что-то пошло не так, завершаем работу:
    return;
  }
  Serial.println("card initialized.");
*/

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
  else if(num ==0){return "00";}
  }

void loop()
{
  
// датчик CO2 плюс выпрямитель логического уровня -------------------------------------
mySerial.write(cmd, 9);
memset(response, 0,9);
mySerial.readBytes(response, 9);
Serial.print(response[0], HEX);
Serial.print(" ");
int i;
byte crc =0;
for(i=1; i<8;i++)
{
  crc += response[i];
  Serial.print(response[i],HEX);
  Serial.print(" ");
}
Serial.println();
crc = 255 - crc;
crc++;
if(!(response[0] == 0xFF && response[1] == 0x86 && response[8] ==crc ))
{
  Serial.println("CRC error: " + String(crc) + " / " + String(response[8] ));
}
else{
  unsigned int responseHigh = (unsigned int) response[2];
  unsigned int responseLow = (unsigned int) response[3];
  unsigned int ppm = (256 * responseHigh) + responseLow;
  Serial.println(ppm);
  ppmcResult = (int) ppm;

  unsigned int ppm1 = (unsigned int) ((response[2] << 8) | response[3]);
  Serial.println(ppm1);
}
// -------------------------------------------------------------------------

  
/* long tt = millis();
  int myVal = digitalRead(pwmPin);

  //Если обнаружили изменение
  if (myVal == HIGH) {
  //  digitalWrite(LedPin, HIGH);
    if (myVal != prevVal) {
      h = tt;
      tl = h - l;
      prevVal = myVal;
    }
  }  else {
  //  digitalWrite(LedPin, LOW);
    if (myVal != prevVal) {
      l = tt;
      th = l - h;
      prevVal = myVal;
      ppm = 5000 * (th - 2) / (th + tl - 4);
    //  Serial.println("PPM = " + String(ppm));
    }
  }

  int ppmcResult = (int) ppm;
  */
  
DateTime now = RTC.now();

 float h1 = bme.readHumidity();
  int h = (int) h1; 
  float t1 = bme.readTemperature();
  int t = (int) t1; 
  float pressure = bme.readPressure();
 float pressure2 = (bme.readPressure() / 133.33F);
  int pressureInt = (int) pressure2;


//  float ppm = gasSensor.getPPM();
//  float rzero = gasSensor.getRZero();
//  float rzeroc = gasSensor.getCorrectedRZero(t1,h1);
 // float ppmc = gasSensor.getCorrectedPPM(t1,h1);

//  int ppmcResult = (int) ppmc;


  Serial.print("Температура = ");
  Serial.print(t1);
  Serial.print(" *C");
  Serial.println();
  
  Serial.print("Влажность = ");
  Serial.print(h1);
  Serial.print(" %");
  Serial.println();

  Serial.print("Давление = ");
  Serial.print(pressure);
  Serial.print(" hPa");
  Serial.println();
  Serial.print(pressure2);
  Serial.print(" mmHg");
  Serial.println();
  
  Serial.print("PPM = ");
  Serial.print(ppmcResult);
  Serial.print(" ppm");
  Serial.println();
  
//  Serial.print("PPM Corect = ");
 // Serial.println(ppmc);
 // Serial.print("RZero = ");
 // Serial.println(rzero);
 // Serial.print("RZero Corect = ");
 // Serial.println(rzeroc);
 
  Serial.println("--------------");
  Serial.println("");

/*
 // Строка с данными, которые мы поместим в файл:
  String logStringData = t  + ',' + h + ',' + pressureInt + ',' + ppmcResult + ',' + String(now.unixtime());
  lcd.setCursor(0,3); //Start at character 0 on line 3
  lcd.print(" DonT OFF !!!!!");     // НЕ выключать 
  Serial.println(" Идёт запись на флэшку !!!");
  File dataFile = SD.open("datalog.csv", FILE_WRITE);
    // Если все хорошо, то записываем строку:
  if (dataFile) {
    dataFile.println(logStringData);
    dataFile.close();
    // Публикуем в мониторе порта для отладки
    Serial.println(logStringData);
  }
  else {
  // Сообщаем об ошибке, если все плохо
    Serial.println("error opening datalog.csv");
  }

  lcd.setCursor(0,3); //Start at character 0 on line 3
  lcd.print(" Write OK !!!!!");     // НЕ выключать
  Serial.println(" Запись окончена !!!");
*/
  
    lcd.setCursor(0,0); //Start at character 0 on line 1
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

  lcd.setCursor(0,1); //Start at character 0 on line 2
  lcd.print(t,1);
  lcd.print("C ");

  lcd.print(h, 1);   // Выводим на экран значение влажности
  lcd.print("% ");     // Выводим текст

  lcd.print(pressureInt, 1);   // Выводим на экран давление
  lcd.print("mmHg ");     // Выводим текст

lcd.setCursor(0,2); //Start at character 0 on line 3
  lcd.print(ppmcResult);     // Выводим CO2
  lcd.print(" ppm ");     // Выводим CO2
    

  delay(10000); // поставил 10 сек т.к. сильно моргал дисплей 
  lcd.clear();

// для CO2 датчика + выпрямителя уровней
if(flagTime == false)
{
  sec10 ++;
  if(sec10 >= 18)
  {
    flagTime == true;
    sec10 =0;
    Serial.println();
    Serial.println("3 minutes have expired");
    Serial.println();
    }
}


  
}
