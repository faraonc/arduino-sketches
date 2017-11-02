#include <SoftwareSerial.h>
#include <ESP8266.h>
#include <aREST.h>

const char *SSID     = "Starxf";
const char *PASSWORD = "Carsomyr";

SoftwareSerial ESPserial(52,53); //ESP's RX = 53 & ESP's TX = 52
ESP8266 wifi(ESPserial);

void setup()
{
  // Start the software serial for communication with the ESP8266
  ESPserial.begin(9600);

  // communication with the host computer
  Serial.begin(9600);
  while (!Serial) 
  {
    ;
  }

  Serial.println("Begin Serial!\n");

  if (!wifi.init(SSID, PASSWORD))
  {
    Serial.println("Wifi Init failed. Check configuration.");
    while (true) ; // loop eternally
  }
  Serial.println("WI-FI Connected!\n");
  
}

void loop()
{
    // listen for communication from the ESP8266 and then write it to the serial monitor
    if (ESPserial.available())
    {
      Serial.write(ESPserial.read());
    }
  
    // listen for user input and send it to the ESP8266
    while (Serial.available() > 0)
    {
      char inByte = Serial.read();
      ESPserial.write(inByte);
    }

}

