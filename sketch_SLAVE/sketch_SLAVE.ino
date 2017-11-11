#include <PIRMotion.h>
#include <LiquidCrystal.h>
#include<DHT.h>

/**------------------ DHT Variables ------------------**/
#define DHTPIN 23
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
PIRMotion pirM(PIR_PIN, 1);
const byte DEBOUNCE_PIR_DELAY = 5; //secs
bool isMotionDetected = false;
/*******************************************************************************/
/*******************************************************************************/

/**------------------ Rain Sensor Variables ------------------**/
const int RAINS_PIN = A8;
unsigned int rainSReading = 0 ;
bool rainSInit = false;
unsigned long rainSTimer = 0;
const int RAINS_DELAY = 2000;
enum
{
  HEAVY_RAIN,
  LIGHT_RAIN,
  DRY
};
byte rainS_state = DRY;

/*******************************************************************************/
/*******************************************************************************/

/**------------------ Photocell Variables ------------------**/
const int PHOTOR_PIN = A7;
unsigned int lightLevel = 0 ;
bool photorInit = false;
unsigned long photorTimer = 0;
const int PHOTOR_DELAY = 2000;
enum
{
  DARK,
  DIM,
  LIGHT,
  BRIGHT,
  VERY_BRIGHT
};
byte light_state = LIGHT;

/*******************************************************************************/
/*******************************************************************************/

/**------------------ Air Sensor Variables ------------------**/

bool airSensInit = false;
unsigned long airSensTimer = 0;
const int AIRSENSOR_DELAY = 2000;

/**------------------ MQ7 Variables ------------------**/
const int MQ7_pin = A9;
int MQ7_reading = 0;
float CO_value = 0;

/**------------------ MQ3 Variables ------------------**/
const int MQ3_pin = A8;
int MQ3_reading = 0;
float alcohol_value = 0;

/**------------------ MQ2 Variables ------------------**/
const int MQ2_pin = A10;
int MQ2_reading = 0;
float smoke_value = 0;


/**------------------ MQ135 Variables ------------------**/
const int MQ135_pin = A11;
int MQ135_reading = 0;
float sulfide_value = 0;
/*******************************************************************************/
/*******************************************************************************/

/**------------------ Calibration Variables ------------------**/
bool isCalibrated = false;
/*******************************************************************************/
/*******************************************************************************/

/**------------------ debugging Variables ------------------**/
bool debugInit = false;
unsigned long debugTimer = 0;
const int DEBUG_DELAY = 3000;
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

void pirBoot()
{
  pirM.setMotionCallback(pirmCallback);
  pirM.setLagAfterMotion(DEBOUNCE_PIR_DELAY);
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
        //        Serial.println("You Pressed the button");
      }
    }
  }
  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;
}

void pirmCallback()
{
  isMotionDetected = true;
  sendSyn();
  syn_state = ACTIVE_MOTION;
}

void readPIR()
{
  pirM.update();
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
  else if ((millis() - DHTtimer) >= DHT_DELAY) //delay(2000);
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
  else if ((millis() - photorTimer) >= PHOTOR_DELAY)
  {
    lightLevel = analogRead(PHOTOR_PIN);
    //    Serial.print("LightLevel: ");
    //    Serial.print(lightLevel);
    if (lightLevel < 10)
    {
      light_state = DARK;
    }
    else if (lightLevel < 200)
    {
      light_state = DIM;
    }
    else if (lightLevel < 500)
    {
      light_state = LIGHT;
    }
    else if (lightLevel < 800)
    {
      light_state = BRIGHT;
    }
    else
    {
      light_state = VERY_BRIGHT;
    }
    photorInit = false;
  }
}

void readRainSensor()
{
  if (!rainSInit)
  {
    rainSInit = true;
    rainSTimer = millis();
  }
  else if ((millis() - rainSTimer) >= RAINS_DELAY)
  {
    rainSReading = analogRead(RAINS_PIN);
    int range = map(rainSReading, 0, 1023, 0, 3);
    //    Serial.print("Rain Sensor Reading: ");
    //    Serial.print(rainSReading);
    // range value:
    switch (range)
    {
      case 0:    // Sensor getting wet
        rainS_state = HEAVY_RAIN;
        break;
      case 1:    // Sensor getting wet
        rainS_state = LIGHT_RAIN;
        break;
      case 2:    // Sensor dry - To shut this up delete the " Serial.println("Not Raining"); " below.
        rainS_state = DRY;
        break;
    }
    rainSInit = false;
  }
}

void readAirSensors()
{
  if (!airSensInit)
  {
    airSensInit = true;
    airSensTimer = millis();
  }
  else if ( (millis() - airSensTimer) > AIRSENSOR_DELAY)
  {

    MQ7_reading = analogRead(MQ7_pin); //reads the analaog value from the CO sensor's AOUT pin
    CO_value = (1000.0 / 1023) * MQ7_reading;
    Serial.print("CO ppm: ");
    Serial.println(CO_value);

    MQ3_reading = analogRead(MQ3_pin);
    alcohol_value = (4.0 / 1023) * MQ3_reading;
    Serial.print("Alcohol mg/L: ");
    Serial.println(alcohol_value);

    MQ2_reading = analogRead(MQ2_pin);
    smoke_value = (1000.0 / 1023) * MQ2_reading;
    Serial.print("smoke ppm: ");
    Serial.println(smoke_value);

    MQ135_reading = analogRead(MQ135_pin);
    sulfide_value = (1000.0 / 1023) * MQ135_reading;
    Serial.print("sulfide ppm: ");
    Serial.println(sulfide_value);

    airSensInit = false;

  }
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
    case 'P':
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

/* DEBUGG */
void showData()
{
  if (!debugInit)
  {
    debugInit = true;
    debugTimer = millis();
  }
  else if ( (millis() - debugTimer) > DEBUG_DELAY)
  {
    Serial.print ("Light :");
    switch (light_state)
    {
      case DARK:
        Serial.println (" DARK ");
        break;
      case DIM:
        Serial.println (" DIM ");
        break;
      case LIGHT:
        Serial.println (" LIGHT ");
        break;
      case BRIGHT:
        Serial.println (" BRIGHT ");
        break;
      case VERY_BRIGHT:
        Serial.println (" VERY BRIGHT ");
        break;
    }

    Serial.print ("Rain :");
    switch (rainS_state)
    {
      case HEAVY_RAIN:
        Serial.println (" HEAVY RAIN ");
        break;
      case LIGHT_RAIN:
        Serial.println (" LIGHT RAIN ");
        break;
      case DRY:
        Serial.println (" DRY ");
        break;
    }

    Serial.print ("Fahrenheit :");
    Serial.println (fahrenheit);
    Serial.print ("Humidity :");
    Serial.println (humidity);
    Serial.print ("Heat Index Factor:");
    Serial.println (hif);
    Serial.println("");

    debugInit = false;
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
  pirBoot();
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
  readPhotocell();
  readRainSensor();
  readTempAndHumid();
  //  readAirSensors();
  readPIR();
  sendMsg();
  resetLCD();

  if (!isCalibrated)
  {
    isCalibrated = true;
  }
  //  else
  //  {
  //    showData();
  //
  //    //TO-DO
  //    //Write a function to send data
  //  }
}
