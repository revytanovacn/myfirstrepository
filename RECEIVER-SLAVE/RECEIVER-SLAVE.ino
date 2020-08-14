#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include "DHT.h"
#include <LiquidCrystal_I2C.h>
#include <Wire.h>


LiquidCrystal_I2C lcd(0x27, 16, 2);

//TinyGPSPlus gps;
SoftwareSerial DataSerial(4,5); //R
unsigned long previousMillis = 0;
const long interval = 1000;
 
String recData[3];
RF24 radio(8, 7); // CE, CSN

struct MyData {
  float hum; 
  float temp;
  float pres;
  float alt;
  double latitude;
  double longitude;
};
MyData data;

void setup() {
  lcd.init();
  lcd.backlight();
  //  Setup Serial
  Serial.begin(9600);
  DataSerial.begin(9600);

  //  Setup transmitter radio
  radio.begin();
  radio.openReadingPipe(1, 0xF0F0F0F0E1LL);
  radio.setChannel(0x76);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.startListening();
  radio.enableDynamicPayloads();
  radio.powerUp();
  
}


  void loop() {
  Serial.println(); 
  Serial.println ("------------D A T A   B U O Y-----------");
  Serial.println();
  Serial.print("Date  :  ");
  Serial.println (__DATE__);    
  Serial.print("Time  :  ");
  Serial.println(__TIME__);
  Serial.println();
  delay (1000);

unsigned long currentMillis = millis();
  if(currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;

    //baca data dari master
    String receive = "";
    while(DataSerial.available()>0)
    {
      receive += char(DataSerial.read());
    }
    receive.trim();

    if(receive != "");
    {
      //memecah data dari sendData = "106.000381#8.3993"(arry)
      int index = 0;
      for(int i = 0; i<= receive.length(); i++) 
      {
        char  delimiter = '#';
        if(receive[i] !=delimiter)
         recData[index] += receive[i];
        else
          index++;
      }

      //pastikan data lengkap
      //urutan 0 = lat 1 = long 2 = kompas
      if(index == 2)
      {
        //tampilan nilai lengkap gps

 lcd.clear();
 lcd.setCursor (6,0);
 lcd.print("DATA");
 lcd.setCursor (6,1);
 lcd.print ("BUOY");
 delay (500);

 lcd.clear();
 lcd.print ("DATE");
 lcd.setCursor (5,0);
 lcd.print (__DATE__);
 lcd.setCursor (0,1);
 lcd.print ("TIME");
 lcd.setCursor (5,1);
 lcd.print (__TIME__);
 delay (1000);
 

 lcd.clear();
 lcd.print ("Lat1");
 lcd.setCursor (5,0);
 lcd.print ( recData [0]);
 lcd.setCursor (0,1);
 lcd.print ("Lng1");
 lcd.setCursor (5,1);
 lcd.print ( recData[1]);
 delay (1000);


 lcd.clear();
 lcd.print ("Lat2");
 lcd.setCursor (5,0);
 lcd.print (data.latitude, 6);
 lcd.setCursor (0,1);
 lcd.print ("Lng2");
 lcd.setCursor (5,1);
 lcd.print (data.longitude, 6);
 delay (1000);

 
 lcd.clear();
 lcd.print ("Heading Error");
 lcd.setCursor (0,1);
 lcd.print ( recData[2]);
 lcd.print((char)223);
 delay (1000);


 lcd.clear();
 lcd.print ("Temp");
 lcd.setCursor (5,0);
 lcd.print (data.hum);
 lcd.print (" C");
 lcd.setCursor (0,1);
 lcd.print ("Hum");
 lcd.setCursor (5,1);
 lcd.print (data.temp);
 lcd.print (" %");
 delay (1000);

 lcd.clear();
 lcd.print ("Alt");
 lcd.setCursor (6,0);
 lcd.print (data.alt);
 lcd.print (" m");
 lcd.setCursor (0,1);
 lcd.print ("Press");
 lcd.setCursor (5,1);
 lcd.print (data.pres);
 lcd.print ("hPa");
 delay (1000);

 lcd.clear();

        
  Serial.println("Latitude 1 :"  + recData[0]); //nilai latitude 1
  Serial.println("Longitude 1 :"  + recData[1]);//nilai longitude 1
  Serial.println("");
  
  Serial.println("Heading Eror (Deg):"  + recData[2]);//nilai longitude 2
  Serial.println("");
      }
      recData[0] = "";
      recData[1] = "";
      recData[2] = "";
    }
    //request data from master
    DataSerial.println("YES");
  }
  

 if (radio.available()) {
    
    Serial.println();
    radio.read(&data, sizeof(MyData));    


  Serial.print(F("Latitude 2 :  "));
  Serial.println(data.latitude, 6);
  Serial.print(F("Longitude 2 :  "));
  Serial.println(data.longitude, 6);
  Serial.println();

  

  Serial.print(F("Temperature (C): "));
  Serial.println(data.hum);
  Serial.print(F("Humidity (%): "));
  Serial.println(data.temp);
  Serial.print(F("Altitude (m): "));
  Serial.println(data.alt);
  Serial.print(F("Pressure (hPa): "));
  Serial.println(data.pres);

  delay(1000);}

 
}
