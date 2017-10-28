/*
  CSS 427
  Fall 2017
  Project: Smart Doorbell
  Source Function: Smart Doorbell Slave
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

// These are the pins for the CC3000 chip if you are using a breakout board
#define ADAFRUIT_CC3000_IRQ   3
#define ADAFRUIT_CC3000_VBAT  5
#define ADAFRUIT_CC3000_CS    10

// Create CC3000 instance
Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS, ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT,
                         SPI_CLOCK_DIV2);
// Create aREST instance
aREST rest = aREST();

// Your WiFi SSID and password
#define WLAN_SSID       "Conard James's iPhone"
#define WLAN_PASS       "12345678"
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
int temperature;
int humidity;
/* END OF BLOCK FOR aREST TESTING FOR WIFI*/

void setup(void)
{
  // Start Serial
  Serial.begin(115200);
  Serial.println(F("Start-up SMDB_SLave!\n"));

  rest.set_id("001");
  rest.set_name("SMDB_Slave");

  /* aREST TESTING FOR WIFI*/
  // Init variables and expose them to REST API
  temperature = 24;
  humidity = 40;
  rest.variable("temperature", &temperature);
  rest.variable("humidity", &humidity);
  rest.function("led", ledControl);
  /* END OF BLOCK FOR aREST TESTING FOR WIFI*/


  // Set up CC3000 and get connected to the wireless network.
  if (!cc3000.begin())
  {
    while (1);
  }
  if (!cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY))
  {
    Serial.println(F("Failed!"));
    while (1);
  }
  Serial.println(F("Connected to WI-FI!"));

  /* Wait for DHCP to complete */
  Serial.println(F("Request DHCP"));
  while (!cc3000.checkDHCP())
  {
    delay(100);
  }
  Serial.println(F("Completed DHCP request"));

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
  Serial.println(F("Listening for connections..."));

  // Enable watchdog
  wdt_enable(WDTO_4S);
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

}

// Print connection details of the CC3000 chip
bool displayConnectionDetails(void)
{
  uint32_t ipAddress, netmask, gateway, dhcpserv, dnsserv;

  if (!cc3000.getIPAddress(&ipAddress, &netmask, &gateway, &dhcpserv, &dnsserv))
  {
    Serial.println(F("Unable to retrieve the IP Address!\r\n"));
    return false;
  }
  else
  {
    Serial.print(F("\nIP Addr: ")); cc3000.printIPdotsRev(ipAddress);
    Serial.print(F("\nNetmask: ")); cc3000.printIPdotsRev(netmask);
    Serial.print(F("\nGateway: ")); cc3000.printIPdotsRev(gateway);
    Serial.print(F("\nDHCPsrv: ")); cc3000.printIPdotsRev(dhcpserv);
    Serial.print(F("\nDNSserv: ")); cc3000.printIPdotsRev(dnsserv);
    Serial.println();
    return true;
  }
}

/* FOR aREST TESTING FOR WIFI*/
int ledControl(String command) {

  // Get state from command
  int state = command.toInt();

  digitalWrite(6, state);
  return 1;
}
/* END OF BLOCK FOR aREST TESTING FOR WIFI*/
