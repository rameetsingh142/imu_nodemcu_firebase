#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include "Wire.h"
 


// Set these to run example.
#define FIREBASE_HOST "rameet123-250e2.firebaseio.com"
#define FIREBASE_AUTH "Rk9KX6nSCwgTZQ3jovptWIcyUHdi5jvYJJv877GG"
#define WIFI_SSID "rameet"
#define WIFI_PASSWORD "0142PPpp"
const int MPU_addr=0x68; // I2C address of the MPU-6050
 
int16_t AcX,AcY,AcZ;

void setup() {
  Serial.begin(9600);

  // connect to wifi.
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.print("connected: ");
  Serial.println(WiFi.localIP());
  
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
  Wire.begin();
 
Wire.beginTransmission(MPU_addr);
Wire.write(0x6B); // PWR_MGMT_1 register
Wire.write(0); // set to zero (wakes up the MPU-6050)
Wire.endTransmission(true);
}



void loop() {
  Wire.beginTransmission(MPU_addr);
Wire.write(0x3B); // starting with register 0x3B (ACCEL_XOUT_H)
Wire.endTransmission(false);
Wire.requestFrom(MPU_addr,14,true); // request a total of 14 registers
AcX=Wire.read()<<8|Wire.read(); // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)
AcY=Wire.read()<<8|Wire.read(); // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
AcZ=Wire.read()<<8|Wire.read(); // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)

delay(500);

  // set string value
  Firebase.setInt("X",AcX );

  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
    Firebase.setInt("Y",AcY );

  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
      Firebase.setInt("Z",AcZ );

  // handle error
  if (Firebase.failed()) {
      Serial.print("setting /message failed:");
      Serial.println(Firebase.error());  
      return;
  }
  delay(1000);
Serial.println("AcX = "); Serial.println(AcX);
Serial.println(" | AcY = "); Serial.println(AcY);
Serial.println(" | AcZ = "); Serial.println(AcZ);


}
