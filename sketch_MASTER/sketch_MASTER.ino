#include <SoftwareSerial.h>
#include <WiFiEsp.h>
#include <LiquidCrystal.h>

const char *SSID     = "Conard James's iPhone";
const char *PASSWORD = "12345678";
const byte HTTP_PORT = 80;
const byte ESP_RX = 53;
const byte ESP_TX = 52;
const byte TEST_LED = 13;
const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
const int BUZZER = 9;

int status = WL_IDLE_STATUS;
unsigned int ACK_SLAVE = 0;
unsigned int SYN = 0;
 
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

//ESP's RX = 53 & ESP's TX = 52
SoftwareSerial ESPserial(ESP_TX, ESP_RX);
WiFiEspServer server(HTTP_PORT);
WiFiEspClient client;
// use a ring buffer to increase speed and reduce memory allocation
RingBuffer buf(8);

void setup()
{

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Print a message to the LCD.
  lcd.print("hello, world!");
  // Set buzzer - pin 9 as an output
  pinMode(BUZZER, OUTPUT);

  // Start the software serial for communication with the ESP8266
  ESPserial.begin(9600);

  // communication with the host computer
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }

//  // initialize ESP module
//  WiFi.init(&ESPserial);
//
//  // check for the presence of the shield
//  if (WiFi.status() == WL_NO_SHIELD)
//  {
//    Serial.println("WiFi shield not present");
//    // don't continue
//    while (true);
//  }
//
//  Serial.println("WiFi shield exists. Connecting...");
//
//  // attempt to connect to WiFi network
//  while (status != WL_CONNECTED)
//  {
//    Serial.print("Attempting to connect to WPA SSID: ");
//    Serial.println(SSID);
//    // Connect to WPA/WPA2 network
//    status = WiFi.begin(SSID, PASSWORD);
//  }
//
//  Serial.println("You're connected to the network");
//  printWifiStatus();
//
//  // start the web server on port 80
//  server.begin();
//
//  client = NULL;

  /*DEBUGGING PURPOSE ONLY*/
  pinMode(TEST_LED, OUTPUT);
  /*DEBUGGING PURPOSE ONLY*/

}

void loop()
{
  updateLCD();
  buzz();

//  client = server.available();  // listen for incoming clients
//
//  // if you get a client,
//  if (client)
//  {
//    Serial.println("New client");
//    serviceClient();
//    // close the connection
//    client.stop();
//    Serial.println("Client disconnected");
//  }
}


/*TO-DO*/
void updateLCD()
{
  // set the cursor to column 0, line 1
  // (note: line 1 is the second row, since counting begins with 0):
  lcd.setCursor(0, 1);
  // print the number of seconds since reset:
  lcd.print(millis() / 1000);
}

void buzz()
{
  // Send 1KHz sound signal...
  tone(BUZZER, 1000); 
  delay(1000);
  noTone(BUZZER);
  delay(1000);

}

void serviceClient()
{
  // initialize the circular buffer
  buf.init();

  // loop while the client's connected
  while (client.connected())
  {
    // if there's bytes to read from the client,
    if (client.available())
    {
      char c = client.read();               // read a byte, then
      buf.push(c);                          // push it to the ring buffer

      // printing the stream to the serial monitor will slow down
      // the receiving of data from the ESP filling the serial buffer
      //Serial.write(c);

      // you got two newline characters in a row
      // that's the end of the HTTP request, so send a response
      if (buf.endsWith("\r\n\r\n"))
      {
        sendHttpResponse();
        break;
      }

      /*DEBUGGING PURPOSE ONLY*/
      // Check to see if the client request was "GET /H" or "GET /L":
      if (buf.endsWith("GET /H"))
      {
        Serial.println("Turn led ON!!!!!!!!!!!!!!!");
        digitalWrite(TEST_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
      }
      else if (buf.endsWith("GET /L"))
      {
        Serial.println("Turn led OFF!!!!!!!!!!!!!!!");
        digitalWrite(TEST_LED, LOW);    // turn the LED off by making the voltage LOW
      }
      /*DEBUGGING PURPOSE ONLY*/
    }
  }
}

void sendHttpResponse()
{
  // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
  // and a content-type so the client knows what's coming, then a blank line:
  client.println("HTTP/1.1 200 OK");
  client.println("Content-type:text/html");
  client.println();

  // the content of the HTTP response follows the header:
  client.print("The LED is responding");
  client.println("<br>");
  client.println("<br>");

  client.println("Click <a href=\"/H\">here</a> turn the LED on<br>");
  client.println("Click <a href=\"/L\">here</a> turn the LED off<br>");

  // The HTTP response ends with another blank line:
  client.println();
}

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print where to go in the browser
  Serial.println();
  Serial.print("To see this page in action, open a browser to http://");
  Serial.println(ip);
  Serial.println();
}
