#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <stdint.h>
#include "SparkFunBME280.h"
#include "Wire.h"
//#include "DHT.h"

TinyGPSPlus gps;
BME280 mySensor; 

SoftwareSerial ss(2, 3); //RX, TX
RF24 radio(8, 7); // CE, CSN


//#define DHTPIN 5
//#define DHTTYPE DHT11 
//DHT dht(DHTPIN, DHTTYPE);
struct MyData {
 float temp;
  float hum;
  float pres;
  float alt;
  double latitude;
  double longitude; 
};
MyData data;



void setup() {
  Serial.begin(9600);
  ss.begin(9600);
//dht.begin();

  //  Setup transmitter radio
  radio.begin();
  radio.openWritingPipe(0xF0F0F0F0E1LL);
  radio.setChannel(0x76);
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_250KBPS);
  radio.stopListening();
  radio.enableDynamicPayloads();
  radio.powerUp();


mySensor.settings.commInterface = I2C_MODE;
  mySensor.settings.I2CAddress = 0x76;
  mySensor.settings.runMode = 3; //  3, Normal mode
  mySensor.settings.tStandby = 0; //  0, 0.5ms
  mySensor.settings.filter = 0; //  0, filter off
  mySensor.settings.tempOverSample = 1;
  mySensor.settings.pressOverSample = 1;
  mySensor.settings.humidOverSample = 1;

  Serial.print("Program Started\n");
  Serial.println("Starting BME280s... result of .begin():");
  delay(10);  //Make sure sensor had enough time to turn on. BME280 requires 2ms to start up.
  //Calling .begin() causes the settings to be loaded
  Serial.print("Sensor A: 0x");
  Serial.println(mySensor.begin(), HEX);

  
}


void loop() {
  while (ss.available() > 0){
    if (gps.encode(ss.read())){
      //getInfo();

    if (gps.location.isValid()){
   data.latitude = gps.location.lat();
    data.longitude = gps.location.lng();
    
  }
    else{
    Serial.println("Invalid location");
  }
  
  }
   
  }
  Serial.print("Humidity: ");
  Serial.println(mySensor.readFloatHumidity(),2);
  Serial.print("Temperature: ");
  Serial.println(mySensor.readTempC(),2 ); 
  Serial.print("Pressure: ");
  Serial.println(mySensor.readFloatPressure(), 2);
  Serial.print("Altitude: ");
  Serial.println(mySensor.readFloatAltitudeMeters(), 2);

    data.hum = mySensor.readFloatHumidity();
  data.temp = mySensor.readTempC();
  data.pres = mySensor.readFloatPressure();
  data.alt = mySensor.readFloatAltitudeMeters();
  
  radio.write(&data, sizeof(MyData));


      //sendInfo();
   
  }
