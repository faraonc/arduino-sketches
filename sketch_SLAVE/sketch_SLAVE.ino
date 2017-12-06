#include <MQ135.h>
#include <LiquidCrystal.h>
#include<DHT.h>

/**------------------ DHT Variables ------------------**/
#define DHTPIN 23
#define DHTTYPE DHT11
const float MAX_TEMP = 160; // Fahrenheit
const float MAX_HUMI = 100;
bool DHTinit = false;
unsigned long DHTtimer = 0;
const int DHT_DELAY = 2000;
float temperature, humidity, fahrenheit, hif;
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
const unsigned int MAX_CO = 800;
const unsigned int MAX_CO2 = 5000;
const unsigned int MAX_SMOKE = 800;
const unsigned int MAX_LPG = 2000;

/**------------------ MQ2 Variables ------------------**/

const int MQ2_PIN = A9;
unsigned int lpg = 0;
unsigned int co = 0;
unsigned int smoke = 0;

//which is derived from the chart in datasheet
#define         RL_VALUE                     (5)     //define the load resistance on the board, in kilo ohms
#define         RO_CLEAN_AIR_FACTOR          (9.83)  //RO_CLEAR_AIR_FACTOR=(Sensor resistance in clean air)/RO,
//cablibration phase
#define         CALIBARAION_SAMPLE_TIMES     (50)    //define how many samples you are going to take in the calibration phase
#define         CALIBRATION_SAMPLE_INTERVAL  (400)   //define the time interal(in milisecond) between each samples in the

//normal operation
#define         READ_SAMPLE_TIMES            (10)     //define the time interal(in milisecond) between each samples in 


#define         GAS_LPG                      (0)
#define         GAS_CO                       (1)
#define         GAS_SMOKE                    (2)

//two points are taken from the curve.
//with these two points, a line is formed which is "approximately equivalent"
//to the original curve.
//data format:{ x, y, slope}; point1: (lg200, 0.21), point2: (lg10000, -0.59)
float           LPGCurve[3]  =  {2.3, 0.21, -0.47};

//two points are taken from the curve.
//with these two points, a line is formed which is "approximately equivalent"
//to the original curve.
//data format:{ x, y, slope}; point1: (lg200, 0.72), point2: (lg10000,  0.15)
float           COCurve[3]  =  {2.3, 0.72, -0.34};

//two points are taken from the curve.
//with these two points, a line is formed which is "approximately equivalent"
//to the original curve.
//data format:{ x, y, slope}; point1: (lg200, 0.53), point2: (lg10000,  -0.22)
float           SmokeCurve[3] = {2.3, 0.53, -0.44};

//Ro is initialized to 10 kilo ohms
float           Ro           =  10;


/**------------------ MQ135 Variables ------------------**/
const int MQ135_PIN = A10;
MQ135 mq135_sensor = MQ135(MQ135_PIN);
float co2rzero, correctedCo2RZero, co2Resistance, co2 , correctedCo2;
/*******************************************************************************/
/*******************************************************************************/

/**------------------ Calibration Variables ------------------**/
bool isCalibrated = false;
const byte CALIBRATION_PERIOD = 2;
const int CALIBRATION_DELAY_TIMER = 2000;
/*******************************************************************************/
/*******************************************************************************/

/**------------------ debugging Variables ------------------**/
bool debugInit = false;
bool isDebugging = false;
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

/**------------------ Dust Sensor Variables ------------------**/
// Pin 1 VCC is connected to Arduino 5V pin with capacitor and resistor
// Pin 2 GND is connected to any Arduino ground pin.
// Pin 3 ILED (Infrared LED) is connected to Arduino 52 pin
// Pin 4 GND is connected to any Arduino ground pin.
// Pin 5 AOUT (Analog Output) is connected to Arduino A11 pin.
// Pin 6 VCC is connected to Arduino 5V pin.
const float MAX_DUST = 2.00;
const byte DUST_LED = 52;
const int DUST_PIN = A11;
const int DUST_SAMPLING_TIME = 280;
const int DUST_DELTA_TIME = 40;
const int DUST_SLEEP_TIME = 9680;
float dustVoMeasured = 0;
float dustCalcVoltage = 0;
float dustDensity = 0;
bool dustInit = false;
unsigned long dustTimer = 0;
const int DUST_DELAY = 2000;
/*******************************************************************************/
/*******************************************************************************/

/**------------------ Communication Variables ------------------**/
const int MSG_BUFFER_TIMEOUT = 2000;
const byte MSG_BUFFER = 2;
enum
{
  LAZY,
  ACTIVE_BUTTON,
  ACTIVE_MOTION,
  ACTIVE_DATA
};
byte syn_state = LAZY;

bool send_init = false;
bool isSendingData = false;
unsigned long send_timer = 0;
const unsigned long SEND_INTERVAL_DELAY = 60000;
const unsigned long SEND_INIT_DELAY = 30000;
unsigned long send_delay = SEND_INIT_DELAY;
unsigned int incomingByte = 0;

bool is_syn_sent = false;
bool is_handshake_completed = false;
char msg[MSG_BUFFER];
byte msg_size = 0;
bool is_msg_buffer_used = false;
unsigned long msg_buffer_timer = 0;
bool onDemand = false;
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
  lcd.clear();

  switch (lang)
  {
    case 'E':
      lcd.print("Calibrating!");
      lcd.setCursor(0, 1);
      lcd.print("Please Wait");
      break;

    case 'P':
      lcd.print("Calibracion");
      lcd.setCursor(0, 1);
      lcd.print("Espera");
      break;
  }
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

void dustBoot()
{
  pinMode(DUST_LED, OUTPUT);
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

  if ((millis() - lastButtonDebounceTime) > DEBOUNCE_BUTTON_DELAY && syn_state == LAZY)
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
        if (isDebugging)
        {
          Serial.println("You Pressed the button");
        }
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

  if ((millis() - lastPIRDebounceTime) > DEBOUNCE_PIR_DELAY && syn_state == LAZY)
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
          if (isDebugging)
          {
            Serial.println("Motion Detected!");
          }
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
  else if ((millis() - DHTtimer) >= DHT_DELAY) //delay(2000);
  {
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    humidity = dht.readHumidity();
    // Read temperature as Fahrenheit (isFahrenheit = true)
    fahrenheit = dht.readTemperature(true);
    // Read temperature as Celsius (the default)
    temperature = dht.readTemperature();

    // Check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(fahrenheit) || isnan(temperature))
    {
      //Serial.println("Failed to read from DHT sensor!");
      DHTinit = false;
      return;
    }

    // Compute heat index in Fahrenheit (the default)
    hif = dht.computeHeatIndex(fahrenheit, humidity);
    if (fahrenheit > MAX_TEMP) fahrenheit = MAX_TEMP;
    if (humidity > MAX_HUMI) humidity = MAX_HUMI;

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
    if (lightLevel < 100)
    {
      light_state = DARK;
    }
    else if (lightLevel < 400)
    {
      light_state = DIM;
    }
    else if (lightLevel < 700)
    {
      light_state = LIGHT;
    }
    else if (lightLevel < 900)
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

float MQResistanceCalculation(int raw_adc)
{
  return ( ((float)RL_VALUE * (1023 - raw_adc) / raw_adc));
}

void MQCalibration()
{
  float val = 0;

  for (int i = 0; i < CALIBARAION_SAMPLE_TIMES; i++)
  {
    val += MQResistanceCalculation(MQ2_PIN);
    delay(CALIBRATION_SAMPLE_INTERVAL);
  }
  val = val / CALIBARAION_SAMPLE_TIMES;                 //calculate the average value

  val = val / RO_CLEAN_AIR_FACTOR;                      //divided by RO_CLEAN_AIR_FACTOR yields the Ro
  //according to the chart in the datasheet

  Ro = val;

  co2rzero = mq135_sensor.getRZero();
  correctedCo2RZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
  co2Resistance = mq135_sensor.getResistance();
  co2 = mq135_sensor.getPPM();
  correctedCo2 = mq135_sensor.getCorrectedPPM(temperature, humidity);
}

float MQRead()
{
  float rs = 0;

  for (int i = 0; i < READ_SAMPLE_TIMES; i++)
  {
    rs += MQResistanceCalculation(analogRead(MQ2_PIN));
  }

  rs = rs / READ_SAMPLE_TIMES;

  return rs;
}

int MQGetGasPercentage(float rs_ro_ratio, int gas_id)
{
  if ( gas_id == GAS_LPG )
  {
    return MQGetPercentage(rs_ro_ratio, LPGCurve);
  }
  else if ( gas_id == GAS_CO )
  {
    return MQGetPercentage(rs_ro_ratio, COCurve);
  }
  else if ( gas_id == GAS_SMOKE )
  {
    return MQGetPercentage(rs_ro_ratio, SmokeCurve);
  }

  return 0;
}

int MQGetPercentage(float rs_ro_ratio, float *pcurve)
{
  return (pow(10, ( ((log(rs_ro_ratio) - pcurve[1]) / pcurve[2]) + pcurve[0])));
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
    lpg = MQGetGasPercentage(MQRead() / Ro, GAS_LPG);
    co = MQGetGasPercentage(MQRead() / Ro, GAS_CO);
    smoke = MQGetGasPercentage(MQRead() / Ro, GAS_SMOKE);
    co2rzero = mq135_sensor.getRZero();
    correctedCo2RZero = mq135_sensor.getCorrectedRZero(temperature, humidity);
    co2Resistance = mq135_sensor.getResistance();
    co2 = mq135_sensor.getPPM();
    correctedCo2 = mq135_sensor.getCorrectedPPM(temperature, humidity);
    if (co > MAX_CO)co = MAX_CO;
    if (smoke > MAX_SMOKE)smoke = MAX_SMOKE;
    if (correctedCo2 > MAX_CO2) correctedCo2 = MAX_CO2;
    if (lpg > MAX_LPG) lpg = MAX_LPG;
    airSensInit = false;
  }
}

void readDustSensor()
{
  if (!dustInit)
  {
    dustInit = true;
    dustTimer = millis();
  }
  else if ((millis() - dustTimer) >= DUST_DELAY)
  {
    // power on the LED
    digitalWrite(DUST_LED, LOW);
    delayMicroseconds(DUST_SAMPLING_TIME);

    // read the dust value
    dustVoMeasured = analogRead(DUST_PIN);
    delayMicroseconds(DUST_DELTA_TIME);
    // turn the LED off
    digitalWrite(DUST_LED, HIGH);
    delayMicroseconds(DUST_SLEEP_TIME);

    // 0 - 5V mapped to 0 - 1023 integer values
    // recover voltage
    dustCalcVoltage = dustVoMeasured * (5.0 / 1024.0);

    // linear eqaution taken from http://www.howmuchsnow.com/arduino/airquality/
    // Chris Nafis (c) 2012
    dustDensity = 0.17 * dustCalcVoltage - 0.1;
    if (dustDensity < 0) dustDensity = 0;
    if (dustDensity > MAX_DUST) dustDensity = MAX_DUST;
    // unit: mg/m3
    dustInit = false;
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
  isButtonPressed = false;
  isMotionDetected = false;
  isSendingData = false;
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
  switch (msg[0])
  {
    case '{':
      lang = 'E';
      updateLCD();
      break;

    case '|':
      lang = 'P';
      updateLCD();
      break;

    case '}':
      greetGuest();
      break;

    case '~':
      onDemand = true;
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
        if (msg_size == 0)
        {
          msg[msg_size] = c;
          msg_size++;
        }
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
      else if (isSendingData)
      {
        Serial.write('Z');
        writeToMaster();
        Serial.write('S');
        isSendingData = false;
      }
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

    Serial.print ("Fahrenheit : ");
    Serial.println (fahrenheit);
    Serial.print ("Humidity : ");
    Serial.println (humidity);
    Serial.print ("Heat Index Factor: ");
    Serial.println (hif);

    Serial.print("LPG : ");
    Serial.print(lpg);
    Serial.print("ppm");
    Serial.print("    ");
    Serial.print("CO : ");
    Serial.print(co);
    Serial.print("ppm");
    Serial.print("    ");
    Serial.print("SMOKE : ");
    Serial.print(smoke);
    Serial.print("ppm");
    Serial.println("");
    Serial.print("CO2 PPM : ");
    Serial.print(co2);
    Serial.print("ppm");
    Serial.print("    ");
    Serial.print("Corrected CO2 PPM : ");
    Serial.print(correctedCo2);
    Serial.print("ppm");
    Serial.println("");
    Serial.print("Dust : ");
    Serial.print(dustDensity);
    Serial.println("mg/m^3");
    Serial.print(dustVoMeasured);
    Serial.println("\n");
    debugInit = false;
  }
}

void calibrate()
{
  //calibrate photocell resistor
  readPhotocell();
  //calibrate rain sensor
  readRainSensor();
  //calibrate dust sensor
  readDustSensor();
  //calibrateDHT
  for (byte i = 0; i < CALIBRATION_PERIOD; i++)
  {
    readTempAndHumid();
    delay(CALIBRATION_DELAY_TIMER);
  }
  //calibrate MQ2
  MQCalibration();
  isCalibrated = true;
  updateLCD();
}

/*
  sendData()
*/
void sendData()
{
  if (!send_init)
  {
    send_init = true;
    send_timer = millis();
  }
  else if ((millis() - send_timer) >= send_delay)
  {
    syn_state = ACTIVE_DATA;
    sendSyn();
    isSendingData = true;
    send_delay = SEND_INTERVAL_DELAY;
    send_init = false;
  }
  else if (onDemand)
  {
    syn_state = ACTIVE_DATA;
    sendSyn();
    isSendingData = true;
    onDemand = false;
  }
}

void tokenizeSendInt(unsigned int tempData)
{
  //Gases
  String s = String(tempData);
  for (int i = 0; i < s.length(); i ++ )
  {
    Serial.write( s.charAt(i));
  }
}

void tokenizeSendFloat(float tempData, char type)
{
  if (type == 'H')
  {
    //using 0 for Temp and Humidity
    String s = String(tempData, 0);
    for (byte i = 0; i < s.length(); i ++ )
    {
      Serial.write( s.charAt(i));
    }
  }
  else if (type == 'D')
  {
    //using 2 for Dust
    String s = String(tempData, 2);
    for (byte i = 0; i < 4; i ++ )
    {
      Serial.write( s.charAt(i));
    }
  }
}

/*
  writeToMaster()
*/
void writeToMaster()
{
  Serial.write('L');
  switch (light_state)
  {
    case DARK:
      Serial.write('0');
      break;
    case DIM:
      Serial.write('1');
      break;
    case LIGHT:
      Serial.write('2');
      break;
    case BRIGHT:
      Serial.write('3');
      break;
    case VERY_BRIGHT:
      Serial.write('4');
  }
  Serial.write('R');
  switch (rainS_state)
  {
    case HEAVY_RAIN:
      Serial.write('0');
      break;
    case LIGHT_RAIN:
      Serial.write('1');
      break;
    case DRY:
      Serial.write('2');
  }

  Serial.write('T');
  tokenizeSendFloat(fahrenheit, 'H');

  Serial.write('H');
  tokenizeSendFloat(humidity, 'H');

  Serial.write('G');
  tokenizeSendInt(lpg);

  Serial.write('C');
  tokenizeSendInt(co);

  Serial.write('E');
  tokenizeSendInt(smoke);

  Serial.write('N');
  tokenizeSendInt(correctedCo2);

  Serial.write('D');
  tokenizeSendFloat(dustDensity, 'D');
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
  dustBoot();
}

/*
   Main Loop
*/
void loop()
{
  if (!isCalibrated)
  {
    calibrate();
  }
  else
  {
    checkMsg();
    //clear message buffer to prevent collision and weird behavior
    checkMsgBuffer();
    checkButton();
    readPhotocell();
    readRainSensor();
    readTempAndHumid();
    readAirSensors();
    readDustSensor();
    readPIR();
    sendMsg();
    resetLCD();
    sendData();
    /** Uncomment prior to testing **/
    //showData();
    //isDebugging = true;
    /** Uncomment prior to testing **/
  }
}

