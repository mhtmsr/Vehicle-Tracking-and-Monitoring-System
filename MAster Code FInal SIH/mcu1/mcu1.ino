#include <SPI.h>
#include <MFRC522.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,16,2);
#define SS_PIN 10
#define RST_PIN 9
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
void setup() 
{
  pinMode(4,OUTPUT); //For Notifying MCU2 that Un-authorized start has been detected.
  pinMode(3,OUTPUT);
  pinMode(2,OUTPUT);
  pinMode(1,OUTPUT);
  digitalWrite(1,LOW);
  digitalWrite(2,LOW);
  digitalWrite(3,LOW);
  digitalWrite(4,LOW);
  Serial.begin(9600);// Initiate a serial communication
  SPI.begin();       // Initiate  SPI bus
  mfrc522.PCD_Init();// Initiate MFRC522
  Serial.println("Approximate your Card to the Reader...");
  Serial.println();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Show the Key-Fob");

}
void loop() 
{
   if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    return;
  }
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    return;
  }
  //Show UID on serial monitor
  Serial.print("UID tag :");
  String content= "";
  byte letter;
for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " "));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  Serial.println();
  Serial.print("Message : ");
  content.toUpperCase();
if (content.substring(1) == "39 1D 95 A3") //Our Key-Fob Tag
  {
    Serial.println("Authorized access");
    Serial.println();
    lcd.clear();
    lcd.print("ACCESS GRANTED");
    delay(3000);
    lcd.clear();
    lcd.print("  Please Wear");
    lcd.setCursor(0,1);
    lcd.print("   Helmet!!!");
    delay(3000);
    digitalWrite(3,LOW);
    digitalWrite(2,HIGH);
    digitalWrite(1,HIGH);
    digitalWrite(4,LOW);
  }
 
 //else  
 {
   digitalWrite(4,HIGH);
   Serial.println("Un-Authorized ");
   Serial.println();
   lcd.clear();
   lcd.print("!!Unauthorized!!");
   delay(3000);
  
  digitalWrite(4,LOW);
} 
