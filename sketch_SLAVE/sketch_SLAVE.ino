
#include <LiquidCrystal.h>
#include<DHT.h>

/**------------------ DHT Variables ------------------**/
#define DHTPIN 6
#define DHTTYPE DHT11
float temperature, humidity, fahrenheit, hif;
DHT dht(DHTPIN, DHTTYPE);
/*******************************************************************************/
/*******************************************************************************/

/**------------------ Button Variables ------------------**/
const int BUTTON_PIN = 7;
const unsigned long DEBOUNCE_DELAY = 50;
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggle
int buttonState = LOW;
int lastButtonState = LOW;
bool isButtonPressed = false;
/*******************************************************************************/
/*******************************************************************************/

/**------------------ Communication Variables ------------------**/
enum
{
  LAZY,
  ACTIVE_BUTTON,
  ACTIVE_MOTION
};

byte syn_state = LAZY;
const byte MSG_BUFFER = 128;
bool is_handshake_completed = false;
unsigned int SYN = 0;
unsigned int ACK_MASTER = 0;
unsigned int incomingByte = 0;
char msg[MSG_BUFFER];
byte msg_size = 0;
bool is_msg_buffer_used = false;
bool is_syn_sent = false;
unsigned long msg_buffer_timer = 0;
const int MSG_BUFFER_TIMEOUT = 2000;
char synArray[MSG_BUFFER];
byte synArray_size;
/*******************************************************************************/
/*******************************************************************************/

/**------------------ PIR Variables ------------------**/
const byte PIR_PIN = 22;              // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int pirVal = 0;                    // variable for reading the pin status
bool isMotionDetected = false;
/*******************************************************************************/
/*******************************************************************************/

/**------------------ LCD Variables ------------------**/
const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
const byte LED_COL = 16;
char lang = 'E';
unsigned long lcdAckTimer = 0;
bool isAcked = false;
unsigned long lcdBuzzTimer = 0;
bool isBuzzed = false;
const unsigned int LCD_UPDATE_DELAY = 5000;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
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

void updateLCD()
{
  lcd.clear();
  switch (lang)
  {
    case 'E':
      lcd.print("Greetings!");
      lcd.setCursor(0, 1);
      lcd.print("Buzz for answer");
      break;

    case 'P':
      lcd.print("Saludos!");
      lcd.setCursor(0, 1);
      lcd.print("Sonar la campana");
      break;

  }
}

void lcdBoot()
{
  lcd.begin(16, 2);
  updateLCD();
}

void xbeeBoot()
{
  Serial.begin(9600);
  while (!Serial)
  {
    ;
  }
}

void buttonBoot()
{
  pinMode(BUTTON_PIN, INPUT);
}

void sensorsBoot()
{
  dht.begin();
}

void checkButton()
{
  // read the state of the pushbutton value:
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState)
  {
    // reset the debouncing timer
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY)
  {
    if (reading != buttonState)
    {
      buttonState = reading;

      if (buttonState == HIGH)
      {
        //Serial.println("You Pressed the button");
        isButtonPressed = true;

        sendSYN();
        syn_state = ACTIVE_BUTTON;
        clearLCDRow(1);
        lcd.setCursor(0, 1);
        lcd.print("Ding! Dong!");
        lcdBuzzTimer = millis();
        isBuzzed = true;
      }
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}

/*
   Motion Sensor PIR
   Sends an 'M' to MASTER when detects motion
*/
void readPIR() {

  pirVal = digitalRead(PIR_PIN);  // read input value
  if (pirVal == HIGH )
  {
    if (pirState == LOW)
    {
      // we have just turned on
      // We only want to print on the output change, not state
      pirState = HIGH;
      isMotionDetected = true;
      sendSYN();
      syn_state = ACTIVE_MOTION;
    }
  }
  else
  {
    if (pirState == HIGH)
    {
      // we have just turned ofF
      // We only want to print on the output change, not state
      pirState = LOW;
      isMotionDetected = false;
    }
  }
}

/*
   A function to collect and show the data form
   DHT11 - Temperature and Humidity sensor
*/
void readTempAndHumid()
{
  //delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  humidity = dht.readHumidity();
  // Read temperature as Celsius (the default)
  temperature = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  fahrenheit = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(humidity) || isnan(temperature) || isnan(fahrenheit))
  {
    //Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  hif = dht.computeHeatIndex(fahrenheit, humidity);
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

void decodeMsg()
{
  for (byte i = 0; i < msg_size; i++)
  {
    switch (msg[i])
    {
      case 'E':
        lang = 'E';
        updateLCD();
        break;

      case 'P':
        lang = 'P';
        updateLCD();
        break;

      case 'A':
        clearLCDRow(1);
        lcd.setCursor(0, 1);
        lcd.print("Guest Ack!");
        lcdAckTimer = millis();
        isAcked = true;
        break;
    }
  }

  clearMsgBuffer();
}

void checkMsg()
{
  // see if there's incoming serial data:
  if (syn_state == LAZY && (Serial.available() > 0))
  {
    incomingByte = Serial.read();
    if (!is_handshake_completed)
    {
      sendAck();
      ACK_MASTER++;
      is_handshake_completed = true;
      msg_buffer_timer = millis();
      is_msg_buffer_used = true;
    }
    else
    {

      char c = (char)incomingByte;
      if (c == 'S')
      {
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

void sendAck()
{
  Serial.write('K');
}

void sendSYN()
{
  Serial.write('O');
}

/*
   sendMSG
   a funvtion to receive an acknowlegment from MASTER
*/
void sendMsg()
{
  if (syn_state != LAZY && (Serial.available() > 0))
  {
    incomingByte = Serial.read();

    if (((char)incomingByte) == 'K')
    {
      if (isButtonPressed)
      {
        Serial.write('B');
        Serial.write('S');
        isButtonPressed = false;
      }

      else if (isMotionDetected)
      {
        Serial.write('M');
        Serial.write('S');
        isMotionDetected = false;
      }
      SYN++;
      is_syn_sent = false;
      syn_state = LAZY;
    }
  }
}

void checkMsgBuffer()
{
  if (is_msg_buffer_used && (millis() - msg_buffer_timer) >= MSG_BUFFER_TIMEOUT)
  {
    clearMsgBuffer();
  }
}

void resetLCD()
{
  if (isBuzzed)
  {
    if ((millis() - lcdBuzzTimer) >= LCD_UPDATE_DELAY)
    {
      updateLCD();
      isBuzzed = false;
    }
  }
  else if (isAcked)
  {
    if ((millis() - lcdAckTimer) >= LCD_UPDATE_DELAY)
    {
      updateLCD();
      isAcked = false;
    }
  }
}

/*
   Setup
*/
void setup()
{
  lcdBoot();
  xbeeBoot();
  buttonBoot();
  sensorsBoot();
}

/*
   Main Loop
*/
void loop()
{
  checkMsg();
  checkMsgBuffer();
  checkButton();
  readTempAndHumid();
  readPIR();
  sendMsg();
  resetLCD();

}
