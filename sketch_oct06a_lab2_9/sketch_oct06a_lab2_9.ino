/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab2 Ex9
 * Board used: Arduino Micro
 */

#include <pt.h>

const int TEMP_PIN = A0;
const int PHOTOR_PIN = A1;
const int MS = 1000;
const String TEMP_PIN_STR = "A0";
const String PHOTOR_PIN_STR = "A1";
const String HEADER_BR = "**************************************************************************";

int temperatureR;
float temperatureC;
float temperatureF;
float temperatureF_threshold;
float temp_sampling_rate;
int brightness;
int brightness_threshold;
float photor_sampling_rate;
int incomingByte;
int report_interval;
static struct pt pt1, pt2, pt3; // each protothread needs one of these

void setup() 
{
  Serial.begin(9600);
  pinMode(TEMP_PIN, INPUT);
  pinMode(PHOTOR_PIN, INPUT);
  temperatureR = 0;
  temperatureC = 0.0;
  temperatureF = 0.0;
  temperatureF_threshold = 100.0; // 100.0 deg F
  temp_sampling_rate = 0.25; // 0.25hz
  brightness = 0;
  brightness_threshold = 400;
  photor_sampling_rate = 10; //0.5hz
  incomingByte = 0;
  report_interval = 10000; //initial report interval

  PT_INIT(&pt1);  // initialise the 3
  PT_INIT(&pt2);  // protothread variables
  PT_INIT(&pt3); 
}

void reportTemperature()
{
  Serial.println("Temperature Sensor's Current Values:");
  Serial.print("Pin: ");
  Serial.println(TEMP_PIN_STR);
  Serial.print(temperatureC);
  Serial.println(" deg C");
  Serial.print(temperatureF);
  Serial.println(" deg F");
  Serial.print("Threshold: ");
  Serial.print(temperatureF_threshold);
  Serial.println(" deg F\n");
  
}

void reportBrightness()
{
  Serial.println("Photocell Resistor Sensor's Current Values:");
  Serial.print("Pin: ");
  Serial.println(PHOTOR_PIN_STR);
  Serial.print("Brightness: ");
  Serial.println(brightness);
  Serial.print("Threshold: ");
  Serial.print("Brightness Threshold: ");
  Serial.println(brightness_threshold);
  Serial.println();
}

static int readTemperatureSensor(struct pt *pt)
{
  static unsigned long timestamp = 0;
  int interval = MS / temp_sampling_rate;
  PT_BEGIN(pt);
  while(1) 
  { // never stop 
    /* each time the function is called the second boolean
    *  argument "millis() - timestamp > interval" is re-evaluated
    *  and if false the function exits after that. */
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis(); // take a new timestamp
    temperatureR = analogRead(TEMP_PIN);
    float voltage = temperatureR * 5;
    voltage /= 1024.0;
    temperatureC = (voltage - 0.5) * 100;
    temperatureF = (temperatureC * 9.0 / 5.) + 32.0;
    //Serial.println("Reading Temperature");
  }
  PT_END(pt);
}

static int readPhotorSensor(struct pt *pt)
{
  static unsigned long timestamp = 0;
  int interval = MS / photor_sampling_rate;
  PT_BEGIN(pt);
  while(1) 
  { // never stop 
    /* each time the function is called the second boolean
    *  argument "millis() - timestamp > interval" is re-evaluated
    *  and if false the function exits after that. */
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis(); // take a new timestamp
    brightness = analogRead(PHOTOR_PIN);
    //Serial.println("Reading Brightness");
  }
  PT_END(pt);
  
}

static int report(struct pt *pt, int interval)
{
  static unsigned long timestamp = 0;
  PT_BEGIN(pt);
  while(1) 
  { // never stop 
    /* each time the function is called the second boolean
    *  argument "millis() - timestamp > interval" is re-evaluated
    *  and if false the function exits after that. */
    PT_WAIT_UNTIL(pt, millis() - timestamp > interval );
    timestamp = millis(); // take a new timestamp
    Serial.println("PERIODIC REPORTING:");
    Serial.println(HEADER_BR);
    reportTemperature();
    reportBrightness();
    Serial.print("FYI:Report Interval is ");
    Serial.print(interval / MS);
    Serial.println(" sec.");
    Serial.println("Input positive integer to change the report interval.\n");
    Serial.println(HEADER_BR);
  }
  PT_END(pt);
  
}

void loop() 
{
  readTemperatureSensor(&pt1);
  readPhotorSensor(&pt2);
  
  if(Serial.available() > 0)
  {
    incomingByte = Serial.parseInt();

    //on deman reporting
    if(incomingByte <= 0)
    {
      Serial.println("ON-DEMAND REPORTING:");
      Serial.println(HEADER_BR);
      reportTemperature();
      reportBrightness();
      Serial.println(HEADER_BR);
    }
    else
    {
      report_interval = incomingByte * MS;
    }
  }

  report(&pt3, report_interval);

  
}



