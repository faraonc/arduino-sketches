#include <AltSoftSerial.h>
#include <SoftwareSerial.h>
#include <WiFiEsp.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

/**------------------ WIFI Variables ------------------**/
const char *SSID     = "Conard James's iPhone";
const char *PASSWORD = "12345678";
const byte LCD_SSID_MAX_SIZE = 9;
const byte HTTP_PORT = 80;
const byte ESP_RX = 53;
const byte ESP_TX = 52;
const int WIFI_CONNECT_DELAY = 2000;

//ESP's RX = 53 & ESP's TX = 52
//SoftwareSerial ESPserial(ESP_TX, ESP_RX);
//TX 46, RX 48
AltSoftSerial ESPserial;
WiFiEspServer server(HTTP_PORT);
WiFiEspClient client;

// use a ring buffer to increase speed and reduce memory allocation
RingBuffer buf(8);
int status = WL_IDLE_STATUS;

unsigned int syn_slave_payload = 0;
unsigned int syn_slave = 0;
unsigned int ack_from_slave_to_master = 0;
unsigned int syn = 0;

unsigned int syn_master_payload = 0;
unsigned int ack_from_master_to_slave = 0;
unsigned int ack_terminal = 0;
unsigned int syn_terminal = 0;

unsigned int MAX_VALUE = 65000;

bool is_handshake_completed = false;
bool is_syn_sent = false;

byte incoming_byte;
const byte MSG_BUFFER = 40;
char msg[MSG_BUFFER];
unsigned int msg_size = 0;
bool is_msg_buffer_used = false;
unsigned long msg_buffer_timer = 0;
const int MSG_BUFFER_TIMEOUT = 2500;
const byte MAX_DEFAULT_DATA_BUFFER = 8;
const byte MAX_DEFAULT_DATA_CHAR = 8;
bool is_wifi_health_check_started = false;
unsigned long wifi_health_timer = 0;
const int WIFI_HEALTH_CHECK_TIMEOUT = 10000;

//for AJAX support
char http_req[4];
byte http_req_i = 0;

enum
{
  LAZY,
  ENGLISH,
  SPANISH,
  GUEST_ACK,
  ON_DEMAND
};
byte syn_state = LAZY;

const String H0 = "HTTP/1.1 200 OK\nContent-type:text/html\nConnection: close\n\n";
const String H1 = "<!DOCTYPE html><html><head><meta charset=\"utf-8\">";
const String ICO_PATH = "<link rel=\"icon\" href=\"data:,\">";
const String H2 = "<link rel=\"stylesheet\" href=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/css/bootstrap.min.css\">";
const String H3 = "<script src=\"https://ajax.googleapis.com/ajax/libs/jquery/3.2.0/jquery.min.js\"></script>";
const String H4 = "<script src=\"https://maxcdn.bootstrapcdn.com/bootstrap/3.3.7/js/bootstrap.min.js\"></script>";
const String H5 = "</head><body><nav class=\"navbar navbar-inverse\"><div class=\"container-fluid\">";
const String H6 = "<div class=\"navbar-header\"><p id=\"team\" class=\"navbar-brand\"></p></div><ul class=\"nav navbar-nav\">";
const String H7 = "<li> <a href=\"/U\" id=\"blink\">";

const String H8 = "</a></li><li class=\"dropdown\"><a class=\"dropdown-toggle\"data-toggle=\"dropdown\" href=\"#\">Communication";
const String H9 = "<span class=\"caret\"></span></a><ul class=\"dropdown-menu\"><li><a>Master to Slave: ";
const String H10 = "</a></li><li><a>Slave to Master: ";
const String H11 = "</a></li><li><a>Master to Terminal: ";
const String H12 = "</a></li><li><a>Terminal to Master: ";
const String H13 = "</a></li></ul></li></ul></div></nav>";
const String H14 = "<div class=\"container-fluid text-center\">";
const String H15 = "<div id=\"jumbox\" class=\"jumbotron jumbotron-fluid\"><h1 id=\"db-header\"></h1></div>";

const String J1 = "<div id=\"weather\"><h1></h1><div class=\"row\"><div class=\"col-sm-6\"><img id=\"weather-icon\"></div>";
const String J2 = "<div class=\"col-sm-6\"><h2 id=\"date-today\"></h2><br><h3 id=\"air-quality\">Air Quality: <span></span>";
const String J3 = "</h3><br><h4 id=\"status-day\"></h4><h4 id=\"advice-day\"></h4></div></div></div><br><br>";

const String H16 = "<div class =\"row\"><div class=\"col-sm-4\"><h2>Temperature</h2><h2 id=\"temp-value\">";

const String H17 = "&#8457;</h2></div><div class=\"col-sm-4\"><h2>Humidity</h2><h2 id=\"humid-value\">";

const String H18 = "%</h2></div><div class=\"col-sm-4\"><h2>Rain</h2><h2 id=\"rain-value\">";

const String H19 = "</h2></div></div>";
const String H20 = "<div class=\"row\"><div class=\"col-sm-4\"><h2>Smoke</h2><h2 id=\"smoke-value\">";

const String H21 = " ppm</h2></div>";
const String H22 = "<div class=\"col-sm-4\"><h2>Dust</h2><h2 id=\"dust-value\">";

const String H23 = " mg/m<sup>3</sup></h2></div>";
const String H24 = "<div class=\"col-sm-4\"><h2>Light</h2><h2 id=\"light-value\">";

const String H25 = "</h2></div></div>";
const String H26 = "<div class =\"row\"><div class=\"col-sm-4\"><h2>CO</h2><h2 id=\"co-value\">";

const String H27 = " ppm</h2></div>";
const String H28 = "<div class=\"col-sm-4\"><h2>CO2</h2><h2 id=\"co2-value\">";

const String H29 = " ppm</h2></div><div class=\"col-sm-4\">";
const String H30 = "<h2>LPG</h2><h2 id=\"lpg-value\">";

const String H31 = " ppm</h2></div></div><hr><p id=\"foot\"></p><div>";
const String H32 = "<script src=\"http://students.washington.edu/faraonc/Arduino/webpage/smdb.js\"></script></body></html>";
/*******************************************************************************/
/*******************************************************************************/

/**------------------ Keypad Variables ------------------**/
const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

//connect to the row pinouts of the keypad
byte rowPins[ROWS] = {25, 24, 23, 22};
bool isRequesting = false;

//connect to the column pinouts of the keypad
byte colPins[COLS] = {28, 27, 26};
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
/*******************************************************************************/
/*******************************************************************************/

/**------------------ LCD Variables ------------------**/
const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
const byte LED_COL = 20;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
enum {
  CO,
  CO2,
  S,
  LPG,
  T,
  H,
  DT,
  LT,
  RN
};
/*******************************************************************************/
/*******************************************************************************/

/**------------------ LED Variables ------------------**/
const byte MOTION_LED = 30;

/**------------------ Buzzer Variables ------------------**/
const int BUZZER = 29;
const int BUZZER_TONE1 = 1200;
const int BUZZER_TONE2 = 1000;
const int BUZZER_DELAY = 250;
/*******************************************************************************/
/*******************************************************************************/

/**------------------ Display variables ------------------**/
String light = "?", rain = "?", temperature = "0", humidity = "0", lpg = "0", co = "0", co2 = "0", smoke = "0", dust = "0";
bool isMotionDetected = false;

/*******************************************************************************/
/*******************************************************************************/


void clearLCDRow(byte row)
{
  lcd.setCursor(0, row);
  for (byte i = 0; i < LED_COL; i++)
  {
    lcd.print(" ");
  }
}

void lcdPrint(int field, String data)
{
  switch (field)
  {
    case (CO):
      lcd.setCursor(3, 1);
      lcd.print("   ");
      lcd.setCursor(3, 1);
      lcd.print(data);
      break;
    case (CO2):
      lcd.setCursor(10, 1);
      lcd.print("    ");
      lcd.setCursor(10, 1);
      lcd.print(data);
      break;
    case (S):
      lcd.setCursor(17, 1);
      lcd.print("   ");
      lcd.setCursor(17, 1);
      lcd.print(data);
      break;
    case (LPG):
      lcd.setCursor(4, 2);
      lcd.print("    ");
      lcd.setCursor(4, 2);
      lcd.print(data);
      break;
    case (T):
      lcd.setCursor(11, 2);
      lcd.print("   ");
      lcd.setCursor(11, 2);
      lcd.print(data);
      break;
    case (H):
      lcd.setCursor(17, 2);
      lcd.print("   ");
      lcd.setCursor(17, 2);
      lcd.print(data);
      break;
    case (DT):
      lcd.setCursor(3, 3);
      lcd.print("    ");
      lcd.setCursor(3, 3);
      lcd.print(data);
      break;
    case (LT):
      lcd.setCursor(11, 3);
      lcd.print("  ");
      lcd.setCursor(11, 3);
      lcd.print(data);
      break;
    case (RN):
      lcd.setCursor(17, 3);
      lcd.print("   ");
      lcd.setCursor(17, 3);
      lcd.print(data);
  }
}

void lcdDisplay()
{
  lcdPrint(CO, co);
  lcdPrint(CO2, co2);
  lcdPrint(S, smoke);
  lcdPrint(LPG, lpg);
  lcdPrint(T, temperature);
  lcdPrint(H, humidity);
  lcdPrint(DT, dust);
  lcdPrint(LT, light);
  lcdPrint(RN, rain);
}

void lcdBoot()
{
  // set up the LCD's number of columns and rows:
  lcd.begin(20, 4);
  lcd.setCursor(0, 1);
  lcd.print("CO:");
  lcd.setCursor(7, 1);
  lcd.print("C2:");
  lcd.setCursor(15, 1);
  lcd.print("S:");
  lcd.setCursor(0, 2);
  lcd.print("LPG:");
  lcd.setCursor(9, 2);
  lcd.print("T:");
  lcd.setCursor(15, 2);
  lcd.print("H:");
  lcd.setCursor(0, 3);
  lcd.print("DT:");
  lcd.setCursor(8, 3);
  lcd.print("LT:");
  lcd.setCursor(14, 3);
  lcd.print("RN:");
}

void buzzerBoot()
{
  // Set buzzer - pin 9 as an output
  pinMode(BUZZER, OUTPUT);
}

void ledBoot()
{
  pinMode(MOTION_LED, OUTPUT);
}

void xbeeBoot()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
}

void printWifiStatus()
{
  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  clearLCDRow(0);
  lcd.setCursor(0, 0);
  lcd.print("IP: ");
  lcd.print(ip);
}

int getSSIDStringSize()
{
  int size = 0;
  while (SSID[size] != '\0')
  {
    size++;
  }

  return size;
}

void espBoot()
{
  status = WL_IDLE_STATUS;
  clearLCDRow(0);
  lcd.setCursor(0, 0);
  lcd.print("Initializing WIFI");
  // Start the software serial for communication with the ESP8266
  ESPserial.begin(9600);

  // initialize ESP module
  WiFi.init(&ESPserial);

  // check for the presence of the shield
  if (WiFi.status() == WL_NO_SHIELD)
  {
    clearLCDRow(0);
    lcd.setCursor(0, 0);
    lcd.print("No WiFi shield!");
    // don't continue
    // while (true);
  }

  // attempt to connect to WiFi network
  if (status != WL_CONNECTED)
  {
    clearLCDRow(0);
    lcd.setCursor(0, 0);
    lcd.print("Connect: "  );

    if (getSSIDStringSize() > LCD_SSID_MAX_SIZE)
    {
      //LCD Support
      lcd.print("Wi-Fi");
    }
    else
    {
      lcd.print(SSID);
    }

    // Connect to WPA/WPA2 network
    status = WiFi.begin(SSID, PASSWORD);
    delay(WIFI_CONNECT_DELAY);
  }

  if (status == WL_CONNECTED)
  {
    printWifiStatus();// start the web server on port 80
    server.begin();
    client = NULL;
  }
  else if (status != WL_CONNECTED)
  {
    clearLCDRow(0);
    lcd.setCursor(0, 0);
    lcd.print("WIFI OFF");
  }
}

String getMotion()
{
  if (isMotionDetected)
  {
    return "Motion Detected";
  }
  else
  {
    return "No Motion";
  }
}

String getRain()
{
  if (rain == "HVY")
  {
    return "HEAVY";
  }
  else
  {
    return rain;
  }
}

String getLight()
{
  if (light == "DK")
  {
    return "DARK";
  }
  else if (light == "DM")
  {
    return "DIM";
  }
  else if (light == "LT")
  {
    return "SEMI BRIGHT";
  }
  else if (light == "BR")
  {
    return "BRIGHT";
  }
  else if (light == "VB")
  {
    return "VERY BRIGHT";
  }

  return light;

}

void sendAjaxRequest()
{
  String json_data = "{\"motion\":\"" + getMotion();
  json_data.concat("\",\"master_slave\":\"" + String(syn + syn_master_payload + ack_from_master_to_slave));
  json_data.concat("\",\"slave_master\":\"" + String(syn_slave + syn_slave_payload + ack_from_slave_to_master));
  json_data.concat("\",\"master_terminal\":\"" + String(syn_terminal));
  json_data.concat("\",\"temp\":\"" + String(temperature));
  json_data.concat("\",\"humid\":\"" + String(humidity));
  json_data.concat("\",\"dust\":\"" + String(dust));
  json_data.concat("\",\"co\":\"" + String(co));
  json_data.concat("\",\"co2\":\"" + String(co2));
  json_data.concat("\",\"smoke\":\"" + String(smoke));
  json_data.concat("\",\"lpg\":\"" + String(lpg));
  json_data.concat("\",\"rain\":\"" + getRain());
  json_data.concat("\",\"light\":\"" + getLight());
  json_data.concat("\",\"terminal_master\":\"" + String(ack_terminal) + "\"}");
  client.print(H0);
  client.print(json_data);
}

void sendHttpResponse()
{
  client.print(H0);
  client.print(H1);
  client.print(ICO_PATH);
  client.print(H2);
  client.print(H3);
  client.print(H4);
  client.print(H5);
  client.print(H6);
  client.print(H7);
  client.print(getMotion());
  client.print(H8);
  client.print(H9);
  client.print(syn + syn_master_payload + ack_from_master_to_slave);
  client.print(H10);
  client.print(syn_slave + syn_slave_payload + ack_from_slave_to_master);
  client.print(H11);
  client.print(syn_terminal);
  client.print(H12);
  client.print(ack_terminal);
  client.print(H13);
  client.print(H14);
  client.print(H15);
  client.print(J1);
  client.print(J2);
  client.print(J3);
  client.print(H16);
  client.print(temperature);
  client.print(H17);
  client.print(humidity);
  client.print(H18);
  client.print(getRain());
  client.print(H19);
  client.print(H20);
  client.print(smoke);
  client.print(H21);
  client.print(H22);
  client.print(dust);
  client.print(H23);
  client.print(H24);
  client.print(getLight());
  client.print(H25);
  client.print(H26);
  client.print(co);
  client.print(H27);
  client.print(H28);
  client.print(co2);
  client.print(H29);
  client.print(H30);
  client.print(lpg);
  client.print(H31);
  client.print(H32);
}

void incrementSynSlavePayload()
{
  if (syn_slave_payload >= MAX_VALUE)
  {
    syn_slave_payload = 0;
  }
  syn_slave_payload++;
}

void incrementSynSlave()
{
  if (syn_slave >= MAX_VALUE)
  {
    syn_slave = 0;
  }
  syn_slave++;
}

void incrementAckFromSlaveToMaster()
{
  if (ack_from_slave_to_master >= MAX_VALUE)
  {
    ack_from_slave_to_master = 0;
  }
  ack_from_slave_to_master++;
}

void incrementSyn()
{
  if (syn >= MAX_VALUE)
  {
    syn = 0;
  }
  syn++;
}

void incrementSynMasterPayload()
{
  if (syn_master_payload >= MAX_VALUE)
  {
    syn_master_payload = 0;
  }
  syn_master_payload += 2;
}

void incrementAckFromMasterToSlave()
{
  if (ack_from_master_to_slave >= MAX_VALUE)
  {
    ack_from_master_to_slave = 0;
  }
  ack_from_master_to_slave++;
}

void incrementAckTerminal()
{
  if (ack_terminal >= MAX_VALUE)
  {
    ack_terminal = 0;
  }
  ack_terminal++;
}

void incrementSynTerminal()
{
  if (syn_terminal >= MAX_VALUE)
  {
    syn_terminal = 0;
  }
  syn_terminal++;
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
      //      Serial.write(c);

      if (c == 'a' && http_req_i == 0)
      {
        http_req[http_req_i] = c;
        http_req_i++;
      }
      else if (c == 'j' && http_req_i == 1)
      {
        http_req[http_req_i] = c;
        http_req_i++;
      }
      else if (c == 'a' && http_req_i == 2)
      {
        http_req[http_req_i] = c;
        http_req_i++;
      }
      else if (c == 'x' && http_req_i == 3)
      {
        http_req[http_req_i] = c;
      }

      // you got two newline characters in a row
      // that's the end of the HTTP request, so send a response
      if (buf.endsWith("\r\n\r\n"))
      {
        incrementSynTerminal();
        incrementAckTerminal();
        if (http_req[0] == 'a' && http_req[1] == 'j' && http_req[2] == 'a' && http_req[3] == 'x')
        {
          sendAjaxRequest();
        }
        else
        {
          sendHttpResponse();
        }
        memset(http_req, 0, sizeof(http_req));
        http_req_i = 0;
        break;
      }

      if (buf.endsWith("GET /U"))
      {
        if (isMotionDetected)
        {
          isMotionDetected = false;
          digitalWrite(MOTION_LED, LOW);
        }
      }
    }
  }
}

void listenClient()
{
  if (status == WL_CONNECTED)
  {
    client = server.available();  // listen for incoming clients
    // if you get a client,
    if (client)
    {
      serviceClient();
      // close the connection
      delay(1);
      client.stop();
    }
  }
}

void buzz()
{
  tone(BUZZER, BUZZER_TONE1);
  delay(BUZZER_DELAY);
  noTone(BUZZER);
  delay(BUZZER_DELAY);

  tone(BUZZER, BUZZER_TONE2);
  delay(BUZZER_DELAY);
  noTone(BUZZER);
  delay(BUZZER_DELAY);

  tone(BUZZER, BUZZER_TONE1);
  delay(BUZZER_DELAY);
  noTone(BUZZER);
  delay(BUZZER_DELAY);

  tone(BUZZER, BUZZER_TONE2);
  delay(BUZZER_DELAY);
  noTone(BUZZER);
  delay(BUZZER_DELAY);
}

void detectedMotion()
{
  isMotionDetected = true;
  digitalWrite(MOTION_LED, HIGH);
}

void checkKeypad()
{
  char key = keypad.getKey();

  if (key != NO_KEY)
  {
    switch (key)
    {
      case '*':
        if (isMotionDetected)
        {
          isMotionDetected = false;
          digitalWrite(MOTION_LED, LOW);
        }
        break;

      case '1':
        syn_state = ENGLISH;
        sendSyn();
        break;

      case '2':
        syn_state = SPANISH;
        sendSyn();
        break;

      case '3':
        syn_state = GUEST_ACK;
        sendSyn();
        break;

      case '5':
        if (!isRequesting)
        {
          isRequesting = true;
          syn_state = ON_DEMAND;
          sendSyn();
        }
        break;

      case '#':
        espBoot();
        break;

      case '0':
        reboot();
    }
  }
}

void clearMsgBuffer()
{
  is_msg_buffer_used = false;
  memset(msg, 0, sizeof(msg));
  msg_size = 0;
  is_handshake_completed = false;
  is_syn_sent = false;
  syn_state = LAZY;
  isRequesting = false;
}

void checkMsgBuffer()
{
  if (is_msg_buffer_used && (millis() - msg_buffer_timer) >= MSG_BUFFER_TIMEOUT)
  {
    clearMsgBuffer();
  }
}

void mapLight (String lightData)
{
  switch (lightData.charAt(0))
  {
    case '0':
      light = "DK";
      break;
    case '1':
      light = "DM";
      break;
    case '2':
      light = "LT";
      break;
    case '3':
      light = "BR";
      break;
    case '4':
      light = "VB";
  }
}

void mapRain(String rainData)
{
  switch (rainData.charAt(0))
  {
    case '0':
      rain = "HVY";
      break;
    case '1':
      rain = "WET";
      break;
    case '2':
      rain = "DRY";
  }
}

void translate()
{
  char default_data[MAX_DEFAULT_DATA_BUFFER];
  byte index = 0;
  //memset(default_data, 0, sizeof(default_data));
  for (int i = 0 ; i < msg_size; i++)
  {
    char c = msg[i];
    switch (c)
    {
      case 'Z':
        break;

      case 'L':
        break;

      case 'R':
        mapLight(default_data);
        index = 0;
        memset(default_data, 0, sizeof(default_data));
        break;

      case 'T':
        mapRain(default_data);
        index = 0;
        memset(default_data, 0, sizeof(default_data));
        break;

      case 'H':
        temperature  = default_data;
        index = 0;
        memset(default_data, 0, sizeof(default_data));
        break;

      case 'G':
        humidity = default_data;
        index = 0;
        memset(default_data, 0, sizeof(default_data));
        break;

      case 'C':
        lpg = default_data;
        index = 0;
        memset(default_data, 0, sizeof(default_data));
        break;

      case 'E':
        co = default_data;
        index = 0;
        memset(default_data, 0, sizeof(default_data));
        break;

      case 'N':
        smoke = default_data;
        index = 0;
        memset(default_data, 0, sizeof(default_data));
        break;

      case 'D':
        co2 = default_data;
        index = 0;
        memset(default_data, 0, sizeof(default_data));
        break;

      case 'S':
        dust = default_data;
        index = 0;
        memset(default_data, 0, sizeof(default_data));
        isRequesting = false;
        break;

      default:
        default_data[index] = c;
        index++;
        //overloaded
        if (index >= MAX_DEFAULT_DATA_CHAR)
        {
          index = 0;
          memset(default_data, 0, sizeof(default_data));
          break;
        }
    }
  }
}

void decodeMsg()
{
  switch (msg[0])
  {
    case 'B':
      buzz();
      break;

    case 'M':
      detectedMotion();
      break;

    case 'Z':
      translate();
      lcdDisplay();
  }
  clearMsgBuffer();
}

void sendAck()
{
  Serial.write('K');
  incrementAckFromMasterToSlave();
}

void sendSyn()
{
  Serial.write('O');
  incrementSyn();
}

void checkMsg()
{
  // see if there's incoming serial data:
  if (syn_state == LAZY && (Serial.available() > 0))
  {
    incoming_byte = Serial.read();
    if (!is_handshake_completed)
    {
      sendAck();
      incrementSynSlave();
      is_handshake_completed = true;
      msg_buffer_timer = millis();
      is_msg_buffer_used = true;
    }
    else
    {
      char c = (char)incoming_byte;
      incrementSynSlavePayload();
      if (c == 'S')
      {
        msg[msg_size] = c;
        msg_size++;
        decodeMsg();
      }
      else
      {
        if (msg_size < MSG_BUFFER)
        {
          msg[msg_size] = c;
          msg_size++;
        }
      }
    }
  }
}

void sendMsg()
{
  if (syn_state != LAZY && (Serial.available() > 0))
  {
    incoming_byte = Serial.read();

    if (((char)incoming_byte) == 'K')
    {
      incrementAckFromSlaveToMaster();
      switch (syn_state)
      {
        case ENGLISH:

          Serial.write('{');
          Serial.write('S');
          break;

        case SPANISH:
          Serial.write('|');
          Serial.write('S');
          break;

        case GUEST_ACK:
          Serial.write('}');
          Serial.write('S');
          break;

        case ON_DEMAND:
          Serial.write('~');
          Serial.write('S');
      }
      incrementSynMasterPayload();
      is_syn_sent = false;
      syn_state = LAZY;
    }
  }
}

void checkConnection()
{
  if (!is_wifi_health_check_started)
  {
    is_wifi_health_check_started = true;
    wifi_health_timer = millis();
  }
  else if ((millis() - wifi_health_timer) >= WIFI_HEALTH_CHECK_TIMEOUT)
  {
    if (WiFi.status() != WL_CONNECTED)
    {
      espBoot();
    }
    is_wifi_health_check_started = false;
    wifi_health_timer = 0;
  }

}

void reboot()
{
  lcdBoot();
  ledBoot();
  buzzerBoot();
  xbeeBoot();
  espBoot();

}

void setup()
{
  reboot();
}

void loop()
{
  checkMsg();
  //clear message buffer to prevent collision and weird behavior
  checkMsgBuffer();
  checkKeypad();
  sendMsg();
  listenClient();
}

