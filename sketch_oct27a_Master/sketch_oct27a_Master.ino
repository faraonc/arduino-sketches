/*
  CSS 427
  Fall 2017
  Project: Smart Doorbell
  Source Function: Smart Doorbell Master
  Team Optimistic:
    Conard James B. Faraon
    Pouria Ghadimi
*/

// Import required libraries
#include <Adafruit_CC3000.h>
#include <SPI.h>

/*-------------------------- Uncomment for MDNS -------------------------------*/
//#include <CC3000_MDNS.h>
/*-------------------------- Uncomment for MDNS -------------------------------*/

#include <aREST.h>
#include <avr/wdt.h>

// These are the pins for the CC3000 chip if you are using a breakout board with MEGA 2560
//Vin to 5v
//grnd-grnd
//SCK - #52
//MISO #50
//MOSI #51
//CS for CC3000 #10
//VBAT_EN #5
//IRQ #3
//3.3v N/A
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// Create CC3000 instance
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                         SPI_CLOCK_DIV2);
// Create aREST instance
aREST rest = aREST();

// Your WiFi SSID and password
#define WLAN_SSID       "Starxf"
#define WLAN_PASS       "Carsomyr"
#define WLAN_SECURITY   WLAN_SEC_WPA2

// The port to listen for incoming TCP connections
#define LISTEN_PORT           80

// Server instance
Adafruit_CC3000_Server restServer(LISTEN_PORT);

// DNS responder instance
/*-------------------------- Uncomment for MDNS -------------------------------*/
//MDNSResponder mdns;
/*-------------------------- Uncomment for MDNS -------------------------------*/

/* FOR aREST TESTING FOR WIFI*/
//int temperature;
//int humidity;
/* END OF BLOCK FOR aREST TESTING FOR WIFI*/

static unsigned long currentSyn = 0;
const int LED_PIN = 6; // the pin that the LED is attached to
int incomingByte;      // a variable to read incoming serial data into

void setup(void)
{
  // Start Serial
  // This will be printed in LCD
  Serial.begin(9600);
  //  Serial.println(F("Start-up SMDB_Master!\n"));

  rest.set_id("001");
  rest.set_name("SMDB_Master");

  /* aREST TESTING FOR WIFI*/
  // Init variables and expose them to REST API
  //  temperature = 24;
  //  humidity = 40;
  //  rest.variable("temperature", &temperature);
  //  rest.variable("humidity", &humidity);
  rest.function("led", ledControl);
  /* END OF BLOCK FOR aREST TESTING FOR WIFI*/


  // Set up CC3000 and get connected to the wireless network.
  if (!cc3000.begin())
  {
    while (1);
  }
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY))
  {
    //    Serial.println(F("Failed!"));
    while (1);
  }
  //  Serial.println(F("Connected to WI-FI!"));

  /* Wait for DHCP to complete */
  //  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100);
  }
  //  Serial.println(F("Completed DHCP request"));

  // Print CC3000 IP address. Enable if mDNS doesn't work
  while (! displayConnectionDetails())
  {
    delay(1000);
  }

  // Start multicast DNS responder
  /*-------------------------- Uncomment for MDNS -------------------------------*/
  //  if (!mdns.begin("arduino", cc3000))
  //  {
  //    while(1);
  //  }
  /*-------------------------- Uncomment for MDNS -------------------------------*/

  // Start server
  restServer.begin();
  //  Serial.println(F("Listening for connections..."));

  // Enable watchdog
  wdt_enable(WDTO_4S);

  pinMode(LED_PIN, OUTPUT);
}

void loop() {

  // Handle any multicast DNS requests
  /*-------------------------- Uncomment for MDNS -------------------------------*/
  //  mdns.update();
  /*-------------------------- Uncomment for MDNS -------------------------------*/

  // Handle REST calls
  Adafruit_CC3000_ClientRef client = restServer.available();
  rest.handle(client);
  wdt_reset();

  // Check connection, reset if connection is lost
  if (!cc3000.checkConnected())
  {
    while (1) {}
  }
  wdt_reset();

  // see if there's incoming serial data:
  if (Serial.available() > 0) 
  {
    // read the oldest byte in the serial buffer:
    incomingByte = Serial.read();
    // if it's a capital H (ASCII 72), turn on the LED:
    if (incomingByte == 'H') 
    {
      digitalWrite(LED_PIN, HIGH);
    }
    // if it's an L (ASCII 76) turn off the LED:
    if (incomingByte == 'L') {
      digitalWrite(LED_PIN, LOW);
    }
  }

}

// Print connection details of the CC3000 chip
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if (!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    //    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    //    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    //    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    //    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    //    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    //    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    //    Serial.println();
    return true;
  }
}

/* FOR aREST TESTING FOR WIFI*/
int ledControl(String command) {

  // Get state from command
  int state = command.toInt();
  //  Serial.println(state);
  digitalWrite(LED_PIN, state);
  return 1;
}
/* END OF BLOCK FOR aREST TESTING FOR WIFI*/
