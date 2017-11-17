#include <SoftwareSerial.h>
#include <WiFiEsp.h>
#include <LiquidCrystal.h>
#include <Keypad.h>

/**------------------ WIFI Variables ------------------**/
const char *SSID     = "Starxf";
const char *PASSWORD = "Carsomyr";
const byte HTTP_PORT = 80;
const byte ESP_RX = 53;
const byte ESP_TX = 52;
const byte MSG_BUFFER = 128;
const int WIFI_CONNECT_DELAY = 2000;

//ESP's RX = 53 & ESP's TX = 52
SoftwareSerial ESPserial(ESP_TX, ESP_RX);
WiFiEspServer server(HTTP_PORT);
WiFiEspClient client;

// use a ring buffer to increase speed and reduce memory allocation
RingBuffer buf(8);
int status = WL_IDLE_STATUS;

unsigned int ack_slave = 0;
unsigned int syn = 0;

bool is_handshake_completed = false;
bool is_syn_sent = false;

byte incoming_byte;
char msg[MSG_BUFFER];
unsigned int msg_size = 0;
bool is_msg_buffer_used = false;
unsigned long msg_buffer_timer = 0;
const int MSG_BUFFER_TIMEOUT = 2000;

enum
{
  LAZY,
  ENGLISH,
  SPANISH,
  GUEST_ACK
};
byte syn_state = LAZY;
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

//connect to the column pinouts of the keypad
byte colPins[COLS] = {28, 27, 26};
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS);
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
String light = "", rain = "", temperature = "", humidity = "", lpg = "", co = "", co2 = "", smoke = "", dust = "";

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
  // print the SSID of the network you're attached to
  // Serial.print("SSID: ");
  // Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  clearLCDRow(0);
  lcd.setCursor(0, 0);
  lcd.print("IP: ");
  lcd.print(ip);
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
    lcd.print(SSID);
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
        digitalWrite(MOTION_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
      }
      else if (buf.endsWith("GET /L"))
      {
        digitalWrite(MOTION_LED, LOW);    // turn the LED off by making the voltage LOW
      }
      /*DEBUGGING PURPOSE ONLY*/
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
        digitalWrite(MOTION_LED, LOW);
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

      case '#':
        espBoot();
        break;

      case '0':
        reboot();
        break;
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
      light = "BB";
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
  String default_data = "";
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
        default_data = "" ;
        break;
      case 'T':
        mapRain(default_data);
        default_data = "";
        break;
      case 'H':
        temperature  = default_data;
        default_data = "";
        break;
      case 'G':
        humidity = default_data;
        default_data = "";
        break;
      case 'C':
        lpg = default_data;
        default_data = "";
        break;
      case 'E':
        co = default_data;
        default_data = "";
        break;
      case 'N':
        smoke = default_data;
        default_data = "";
        break;
      case 'D':
        co2 = default_data;
        default_data = "";
        break;
      case 'S':
        dust = default_data;
        default_data = "";
        break;
      default:
        default_data += c;
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
}

void sendSyn()
{
  Serial.write('O');
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
      ack_slave++;
      is_handshake_completed = true;
      msg_buffer_timer = millis();
      is_msg_buffer_used = true;
    }
    else
    {
      char c = (char)incoming_byte;
      if (c == 'S')
      {
        msg[msg_size] = c;
        msg_size++;
        decodeMsg();
      }
      else
      {
        msg[msg_size] = c;
        msg_size++;
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

      switch (syn_state)
      {
        case ENGLISH:

          Serial.write('E');
          Serial.write('S');
          break;

        case SPANISH:
          Serial.write('P');
          Serial.write('S');
          break;

        case GUEST_ACK:
          Serial.write('A');
          Serial.write('S');
          break;
      }
      syn++;
      is_syn_sent = false;
      syn_state = LAZY;
    }
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
  listenClient();
  sendMsg();
}
