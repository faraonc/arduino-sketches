#include <SoftwareSerial.h>
#include "ESP8266WiFi.h"
#include <aREST.h>

SoftwareSerial ESPserial(22, 23); // RX | TX

// Create aREST instance
aREST rest = aREST();

// WiFi parameters
const char* ssid = "wifi-name";
const char* password = "wifi-password";

// The port to listen for incoming TCP connections
#define LISTEN_PORT 80

// Create an instance of the server
WiFiServer server(LISTEN_PORT);

int x = 50;

void setup()
{
  // communication with the host computer
  Serial.begin(9600);
  while (!Serial) {
    ;
  }

  delay(100);
  Serial.println("Ready");
  Serial.println("");

  // Start the software serial for communication with the ESP8266
  ESPserial.begin(9600);

  // Init variables and expose them to REST API
  rest.variable("x", &x);
  // Give name and ID to device
  rest.set_id("1");
  rest.set_name("esp8266");

  // Connect to WiFi
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  // Start the server
  server.begin();
  Serial.println("Server started");
  // Print the IP address
  Serial.println(WiFi.localIP());
}

void loop()
{
  //  // listen for communication from the ESP8266 and then write it to the serial monitor
  //  if (ESPserial.available())
  //  {
  //    Serial.write(ESPserial.read());
  //  }
  //
  //  // listen for user input and send it to the ESP8266
  //  while (Serial.available() > 0)
  //  {
  //    char inByte = Serial.read();
  //    ESPserial.write(inByte);
  //    Serial.print(inByte);
  //  }

  // Handle REST calls
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
  while (!client.available()) {
    delay(1);
  }
  rest.handle(client);

}

