#include <SoftwareSerial.h>


SoftwareSerial ESPserial(8, 9); // RX | TX

void setup()
{
  // communication with the host computer
  Serial.begin(115200);
  while (!Serial) {
    ;
  }

  Serial.println("Ready");
  Serial.println("");

  // Start the software serial for communication with the ESP8266
  ESPserial.begin(115200);
}

void loop()
{
  // listen for communication from the ESP8266 and then write it to the serial monitor
  if (ESPserial.available())
  {
    Serial.write(ESPserial.read());
  }

  // listen for user input and send it to the ESP8266
  if (Serial.available() > 0)
  {
    ESPserial.write(Serial.read());
  }
}

