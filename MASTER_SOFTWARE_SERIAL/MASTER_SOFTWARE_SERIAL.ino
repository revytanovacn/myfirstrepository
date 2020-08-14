#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <MechaQMC5883.h>

TinyGPSPlus gps;

//#define SLAVE_ADDRESS = 9; //alamat slave
//#define ANSWERSIZE 5

SoftwareSerial ss(2,3); //RX, TX
MechaQMC5883 qmc;

const float fullCircle = 360.0;
const float rad2deg = 180.0 / PI;
float x;
float y;
float h_e;//heading error
float latitude, longitude;
/*struct MyData {
  double latitude;
  double longitude; 
};
MyData data;*/

void setup() {
  Serial.begin(9600);
  ss.begin(9600);
  Wire.begin();
  qmc.init ();
}

void loop() {
  while (ss.available() > 0){
    if (gps.encode(ss.read())){
      //getInfo();

    if (gps.location.isValid()){
    latitude = gps.location.lat();
    longitude = gps.location.lng();
  }
    
    else{
    Serial.print("Invalid location");
  }
int x, y, z, heading;
    qmc.read (&x, &y, &z);

    //set declination on your location and fix heading
    //you can find your declination on: http://magnetic-declination.com/
    //(+)positive or (-) for negative
// for bytom/ poland declination angle is 4'26E (positive)
//Formula: (deg + (min 60.0)) / (180/M_PI);
//float declinationAngle = (4.0 + (26.0/60.0))/(180/M_PI);

    
    h_e= atan2 ((double)y,(double)x) *rad2deg ;



//periksa heading <0 deg dan heading >360deg

    if (h_e > fullCircle)
          {
          h_e -= fullCircle;
          }
          if (heading < 0.0)
          {
          h_e += fullCircle;
          }


  
  // nilai berbentuk sendData = "106.000381#8.3993"
  String sendData = String(latitude,6) + "#" + String(longitude,7) +"#"+ String (h_e,4);
  Serial.println(sendData);
  delay(1000);
  
 }
}
}
