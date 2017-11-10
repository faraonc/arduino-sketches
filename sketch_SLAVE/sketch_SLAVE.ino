
#include <LiquidCrystal.h>
#include<DHT.h>

/**------------------ DHT Variables ------------------**/
#define DHTPIN 6
#define DHTTYPE DHT11
bool DHTinit = false;
unsigned long DHTtimer = 0;
const int DHT_DELAY = 2000;
float humidity, fahrenheit, hif;
DHT dht(DHTPIN, DHTTYPE);
/*******************************************************************************/
/*******************************************************************************/

/**------------------ Button Variables ------------------**/
const byte BUTTON_PIN = 7;
const unsigned long DEBOUNCE_BUTTON_DELAY = 50;
unsigned long lastButtonDebounceTime = 0;  // the last time the output pin was toggle
int buttonState = LOW;
int lastButtonState = LOW;
bool isButtonPressed = false;
/*******************************************************************************/
/*******************************************************************************/

/**------------------ PIR Variables ------------------**/
const byte PIR_PIN = 22;
const unsigned long DEBOUNCE_PIR_DELAY = 500;
unsigned long lastPIRDebounceTime = 0;  // the last time the output pin was toggle
int pirState = LOW;
int lastPIRState = LOW;
bool isMotionDetected = false;
/*******************************************************************************/
/*******************************************************************************/

/**------------------ Photocell Variables ------------------**/
const int PHOTOR_PIN = A7;
unsigned int lightLevel = 0 ;
bool photorInit = false;
unsigned long photorTimer = 0;
const int PHOTOR_DELAY = 2000;

/*******************************************************************************/
/*******************************************************************************/

/**------------------ Air Sensor Variables ------------------**/
const int MQ7_pin=A9;
bool airSensInit = false
int value = 0;
float CO_
/**------


/******************************************************************************/------- MQ3 Variables ------------------**/ppm_value = 0
/**------------------ MQ3 Variables ------------------**/
const int MQ3_pin=A8;
int value = 0;
float ppm_value = 0;
/*******************************************************************************/
/*******************************************************************************/

/**------------------ LCD Variables ------------------**/
const byte LED_COL = 16;
const unsigned int UPDATE_LCD_DELAY = 5000;
const int RS = 12, EN = 11, D4 = 5, D5 = 4, D6 = 3, D7 = 2;
char lang = 'E';
unsigned long lcdAckTimer = 0;
bool isAcked = false;
unsigned long lcdBuzzTimer = 0;
bool isBuzzed = false;
LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);
/*******************************************************************************/
/*******************************************************************************/

/**------------------ Communication Variables ------------------**/
const int MSG_BUFFER_TIMEOUT = 2000;
const byte MSG_BUFFER = 128;
enum
{
  LAZY,
  ACTIVE_BUTTON,
  ACTIVE_MOTION
};
byte syn_state = LAZY;

unsigned int syn = 0;
unsigned int ack_master = 0;
unsigned int incomingByte = 0;

bool is_syn_sent = false;
bool is_handshake_completed = false;
char msg[MSG_BUFFER];
byte msg_size = 0;
bool is_msg_buffer_used = false;
unsigned long msg_buffer_timer = 0;
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

void photocellBoot()
{
  pinMode(PHOTOR_PIN, INPUT);
}

void MQ7Boot()
{
  pinMode(ledPin, OUTPUT);
}

void dingDong()
{
  clearLCDRow(1);
  lcd.setCursor(0, 1);
  lcd.print("Ding! Dong!");
  lcdBuzzTimer = millis();
  isBuzzed = true;
}

void checkButton()
{
  // read the state of the pushbutton value:
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonState)
  {
    // reset the debouncing timer
    lastButtonDebounceTime = millis();
  }

  if ((millis() - lastButtonDebounceTime) > DEBOUNCE_BUTTON_DELAY)
  {
    if (reading != buttonState)
    {
      buttonState = reading;

      if (buttonState == HIGH)
      {
        isButtonPressed = true;
        sendSyn();
        syn_state = ACTIVE_BUTTON;
        dingDong();
        Serial.println("You Pressed the button");
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
void readPIR()
{
  // read the state of the pir value:
  int reading = digitalRead(PIR_PIN);

  if (reading != lastPIRState)
  {
    // reset the debouncing timer
    lastPIRDebounceTime = millis();
  }

  if ((millis() - lastPIRDebounceTime) > DEBOUNCE_PIR_DELAY)
  {
    if (reading != pirState)
    {
      if (reading == HIGH)
      {
        if (pirState == LOW)
        {
          // we have just turned on
          // We only want to print on the output change, not state
          pirState = HIGH;
          isMotionDetected = true;
          sendSyn();
          syn_state = ACTIVE_MOTION;
          //Serial.println("Motion Detected! Try again in  half a sec!");
        }
      }
      else
      {
        if (pirState == HIGH)
        {
          pirState = LOW;
        }
      }
    }
  }
  lastPIRState = reading;
}

/*
   A function to collect and show the data form
   DHT11 - Temperature and Humidity sensor
*/
void readTempAndHumid()
{
  if (!DHTinit)
  {
    DHTinit = true;
    DHTtimer = millis();
  }
  if ((millis() - DHTtimer) > DHT_DELAY) //delay(2000);
  {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    humidity = dht.readHumidity();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    fahrenheit = dht.readTemperature(true);

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(fahrenheit))
    {
      //Serial.println("Failed to read from DHT sensor!");
      DHTinit = false;
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    hif = dht.computeHeatIndex(fahrenheit, humidity);
    //    Serial.print ("Fahrenheit :");
    //    Serial.println (fahrenheit);
    //    Serial.print ("Humidity :");
    //    Serial.println (humidity);
    //    Serial.print ("Heat Index Factor:");
    //    Serial.println (hif);
    DHTinit = false;
  }
}

void readPhotocell()
{
  if (!photorInit)
  {
    photorInit = true;
    photorTimer = millis();
  }
  if (millis() - photorTimer > PHOTOR_DELAY)
  {
    lightLevel = analogRead(PHOTOR_PIN);
//    Serial.print("lightLevel: ");
//    Serial.print(lightLevel);
//    if (lightLevel < 10)
//    {
//      Serial.println(" - Dark");
//    }
//    else if (lightLevel < 200)
//    {
//      Serial.println(" - Dim");
//    }
//    else if (lightLevel < 500)
//    {
//      Serial.println(" - Light");
//    }
//    else if (lightLevel < 800)
//    {
//      Serial.println(" - Bright");
//    }
//    else
//    {
//      Serial.println(" - Very bright");
//    }
    photorInit = false;
  }
}

void readMQ7()
{
  
}

void greetGuest()
{
  clearLCDRow(1);
  lcd.setCursor(0, 1);
  switch (lang)
  {
    case 'E':
      lcd.print("Hey! Guest Ack!");
      break;
    case 'B':
      lcd.print("Hola visitante!");
      break;
  }

  lcdAckTimer = millis();
  isAcked = true;
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
        greetGuest();
        break;
    }
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
    incomingByte = Serial.read();
    if (!is_handshake_completed)
    {
      sendAck();
      ack_master++;
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
      syn++;
      is_syn_sent = false;
      syn_state = LAZY;
    }
  }
}

void resetLCD()
{
  if (isBuzzed)
  {
    if ((millis() - lcdBuzzTimer) >= UPDATE_LCD_DELAY)
    {
      updateLCD();
      isBuzzed = false;
    }
  }
  else if (isAcked)
  {
    if ((millis() - lcdAckTimer) >= UPDATE_LCD_DELAY)
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
  photocellBoot();
}

/*
   Main Loop
*/
void loop()
{
  checkMsg();
  //clear message buffer to prevent collision and weird behavior
  checkMsgBuffer();
  checkButton();
  readTempAndHumid();
  readPIR();
  readPhotocell();
  sendMsg();
  resetLCD();
}
