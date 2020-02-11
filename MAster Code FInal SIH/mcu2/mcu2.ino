#include <NewPing.h>
#include <math.h>
#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <SPI.h>
#include <LoRa.h>
#include <Wire.h>
String data="";
//Ignition detect
int igniPin=5;
int igniDetect=0;
//Vibration sensor
int vibr_Pin =A0;
long vib;
int v=0;
long duration;
int level;
//Accelerometer
const int x_out = A1; 
const int y_out = A2; 
const int z_out = A3; 
 int x_adc_value, y_adc_value, z_adc_value; 
//GPS
int RXPin = 4, TXPin = 3;
double lati;
double longi;
static const uint32_t GPSBaud = 9600;
//Ultrasonic
int trigPin = 5;
int echoPin = 6;
NewPing sonar(trigPin,echoPin,100);
//Ir_sensor
int ir=7;
int stand;
TinyGPSPlus gps;
SoftwareSerial ss(RXPin, TXPin);

void setup()
{
  pinMode(vibr_Pin, INPUT);
  pinMode(trigPin, OUTPUT); 
  pinMode(echoPin, INPUT);
  pinMode(ir,INPUT_PULLUP);
  Serial.begin(9600); 
  ss.begin(9600);
  Wire.begin(); 
}

void FetchSensorData();

void loop()
{
FetchSensorData();
delay(500);
data = "I"+String(igniDetect)+"/"+"L"+String(level)+"/"+"V"+String(v)+"/"+"X"+String(x_adc_value)+"/"+"Y"+String(y_adc_value)+"/"+"Z"+String(z_adc_value)+"/"+"S"+String(stand)+"/"+"A"+String(lati)+"/"+"B"+String(longi);
Wire.beginTransmission(9); // transmit to device #9
Wire.println(data);              // sends x 
Serial.println(data);
Wire.endTransmission();    // stop transmitting

Serial.println("------------------------------------------------------------------");
delay(200);
}

                                            void FuelLevelFetch()
                                            {
                                              //Ultrasonic Sensor Data...................
                                              level=sonar.ping_cm();
                                              delay(500);
                                            }
                                                                                                        void VibrationSense()
                                                                                                        {
                                                                                                           //Vibration sensor data...................
                                                                                                          vib=pulseIn (vibr_Pin, HIGH);
                                                                                                          delay(100);
                                                                                                          if (vib > 500)
                                                                                                          {
                                                                                                            v=1;
                                                                                                        //    Serial.print("VIBRATION DETECTED");
                                                                                                          }
                                                                                                          else
                                                                                                          {
                                                                                                            v=0;
                                                                                                          //  Serial.print("NO VIBRATION");
                                                                                                           // Serial.println();
                                                                                                          }
                                                                                                        }
                                           void  AccelerometerDataFetch()
                                           {
                                               //Accelerometer data.................
                                               
                                                double x_g_value, y_g_value, z_g_value;
                                                double roll, pitch, yaw;
                                                x_adc_value = analogRead(x_out);  
                                                y_adc_value = analogRead(y_out);
                                                z_adc_value = analogRead(z_out);                                                            
                                              /*  Serial.print("x = ");
                                                Serial.print(x_adc_value);
                                                Serial.print("\t\t");
                                                Serial.print("y = ");
                                                Serial.print(y_adc_value);
                                                Serial.print("\t\t");
                                                Serial.print("z = ");
                                                Serial.print(z_adc_value);*/
                                          }
                                                                                                           void gpsDataFetch()
                                                                                                           {
                                                                                                                //GPS data.....................
              
                                                                                                                  while (ss.available() > 0)
                                                                                                                  {
                                                                                                                      gps.encode(ss.read());
                                                                                                                      if (gps.location.isUpdated())
                                                                                                                      {
                                                                                                                       // Serial.print("--------------GPS--------------");
                                                                                                                       // Serial.print("Latitude= "); 
                                                                                                                       // Serial.print(gps.location.lat(), 6);
                                                                                                                       // Serial.print("    Longitude= "); 
                                                                                                                       // Serial.print(gps.location.lng(), 6);
                                                                                                                       lati=gps.location.lat();
                                                                                                                       longi=gps.location.lng();
                                                                                                                        
                                                                                                                      }
                                                                                                                     
                                                                                                                  }
                                                                                                                  if(!ss.available())
                                                                                                                      {
                                                                                                                      //Serial.println();
                                                                                                                     // Serial.println("Gps not Connected");
                                                                                                                      lati=0;
                                                                                                                      longi=0;
                                                                                                                      }
                                                                                                                            
                                                                                                           }
                                         void irFetch()
                                         {
                                            if(digitalRead(ir)==0)
                                            {
                                             // Serial.println("Stand is ON");
                                             stand=0;
                                            }
                                            else
                                            {
                                              //Serial.println("Stand is OFF");
                                              stand=1;
                                            }
                                         }
                                             void igniFetch()
                                         {
                                            if(digitalRead(igniPin)==0)
                                            {
                                             igniDetect=0;
                                            }
                                            else
                                            {
                                             igniDetect=1;
                                            }
                                         }
void FetchSensorData()
{
  
  FuelLevelFetch();
  VibrationSense();
  AccelerometerDataFetch();
  gpsDataFetch();
  irFetch();
  igniFetch();
  
}
