#include<Wire.h>
#include <SoftwareSerial.h>
SoftwareSerial myGsm(2,3);
boolean flagRx = false;
char buf[40];
char *data;
int level,vib,x,y,z,stand,igni;
float lati,longi;

void receiveEvent(int howMany)
{
   int i;
  for (i=0; i<howMany; i++)
  {
    buf[i] = Wire.read();     // receive byte as a character
  }
  buf[i] = '\0';  // assume it is a string, add zero-terminator
  flagRx = true;    // set flag, we received something
}
void extractData(char *str)
{
  char temp;String value1;
  data=strtok(str,"/");
  while(data!=NULL)
  {
    //Serial.println(data);
    String data1= String(data);
    temp=data1.charAt(0);
    value1=data1.substring(1);
    //Serial.println(temp);
   // Serial.println(value1);
    if(temp=='L')
    level=value1.toInt();
    else if(temp=='V')
    vib=value1.toInt();
    else if(temp=='X')
    x=value1.toInt();
    else if(temp=='Y')
    y=value1.toInt();
    else if(temp=='Z')
    z=value1.toInt();
    else if(temp=='S')
    stand=value1.toInt();
    else if(temp=='A')
    lati=value1.toFloat();
    else if(temp=='B')
    longi=value1.toFloat();
    data=strtok(NULL,"/");
    igni=digitalRead(5);
  }
  
}

void printSerialData()
{
 while(myGsm.available()!=0)
 Serial.write(myGsm.read());
}
void setup()
{
  Serial.begin(9600);
  Wire.begin(9); 
  Wire.onReceive(receiveEvent);
  pinMode(4,INPUT);
  myGsm.begin(9600); 
   printSerialData();
     myGsm.println("AT+SAPBR=3,1,\"CONTYPE\",\"GPRS\"");//setting the SAPBR,connection type is GPRS
     delay(1000);
     printSerialData();
     myGsm.println("AT+SAPBR=3,1,\"APN\",\"\"");//setting the APN,2nd parameter empty works for all networks 
     delay(5000);
     printSerialData();
     myGsm.println();
     myGsm.println("AT+SAPBR=1,1");
     delay(10000);
     printSerialData();
    /*Serial.println(level);
    Serial.println(vib);
    Serial.println(x);
    Serial.println(y);
    Serial.println(z);
    Serial.println(stand);
    Serial.println(lati);
    Serial.println(longi);*/
}
void loop()
{
 
  if (flagRx)
  {
     Serial.println();
     Serial.println(buf);
     delay(1000);
     extractData(buf);
     myGsm.println("AT+CGATT=1");
     delay(200);
     printSerialData();
     myGsm.println("AT+HTTPINIT"); //init the HTTP request
     delay(2000); 
     String req="AT+HTTPPARA=\"URL\",\"vtmssih.000webhostapp.com/vsd.php?L="+String(level)+"&V="+String(vib)+"&X="+String(x)+"&Y="+String(y)+"&Z="+String(z)+"&S="+String(stand)+"&A="+String(lati)+"&B="+String(longi)+"&I="+String(igni)+"\"";
     myGsm.println(req);// setting the httppara, 
     //the second parameter is the website from where you want to access data 
     delay(2000);
     printSerialData();
     myGsm.println("AT+HTTPACTION=0");//submit the GET request 
  }
 
}
