#include <TinyGPS.h>
#include <SoftwareSerial.h>

/*
#include <DHT.h>
#include <DHT_U.h>
#include <SimpleDHT.h>
*/
SoftwareSerial SIM900(7, 8);

TinyGPS gps;  //Creates a new instance of the TinyGPS object

int state = 0;
const int pin = 9;

void setup()
{
  Serial.begin(9600);
  SIM900.begin(9600); 
  dht.begin(); 
 
}

void loop()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;

  // For one second we parse GPS data and report some key values
  for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (Serial.available())
    {
      char c = Serial.read();
      
      if (gps.encode(c))
        newData = true;  
    }
  }

  if (newData && digitalRead(pin) == HIGH && state == 0)      //If newData is true
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);  
    SIM900.print("Push button input = ");
    SIM900.print(digitalRead(pin) );
   
    SIM900.print("AT+CMGF=1\r");
    delay(100);
    SIM900.println("AT + CMGS = \"+91xxxxxxxxxx\"");// recipient's mobile number with country code
    delay(100);

    SIM900.println("HELP! I am in trouble!");
    SIM900.println("These are my location co-ordinates!");
    SIM900.print("Latitude = ");
    SIM900.println(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat, 6);
    SIM900.print("Longitude = ");
    SIM900.print(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon, 6);
    delay(100);
    SIM900.println((char)26); // End AT command with a ^Z, ASCII code 26
    delay(100);
    
    SIM900.print("Temp = ");
    SIM900.print(t);
    SIM900.println(" ");
    SIM900.print("Humidity = ");
    SIM900.println(h);

    if(t>35){
      SIM900.println("Very high body temperature detected");
    }else{
      SIM900.println("Normal body temperature detected");
    }
 
    
    state = 1;
  }
    if (digitalRead(pin) == LOW) {
      state = 0;
  }
  Serial.println(failed);
}
