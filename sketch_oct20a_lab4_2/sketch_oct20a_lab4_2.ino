 /*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab4 Ex2
 * Board used: Arduino Mega, Micro
 */

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <Keypad.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_LSM303_U.h>

enum {
  UNO,
  MICRO,
  MEGA
};

// determine board type
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  int boardName = UNO;
  Keypad keypad = Keypad( NULL, 0, 0, 0, 0 );
  
#elif defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  int boardName = MICRO;
  Keypad keypad = Keypad( NULL, 0, 0, 0, 0 );
  SoftwareSerial mySerial(8,9);
  /* Assign a unique ID to this sensor at the same time */
  Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(54321);
  Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(12345);
  
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  int boardName = MEGA;
  const byte ROWS = 4; //four rows
  const byte COLS = 3; //three columns
  char keys[ROWS][COLS] = {
    {'1','2','3'},
    {'4','5','6'},
    {'7','8','9'},
    {'*','0','#'}
  };
  byte rowPins[ROWS] = {5, 4, 3, 2};   //connect to the row pinouts of the keypad
  byte colPins[COLS] = {8, 7, 6};     //connect to the column pinouts of the keypad
  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
  SoftwareSerial mySerial(10,11);
  Adafruit_LSM303_Accel_Unified accel = Adafruit_LSM303_Accel_Unified(-1);
  Adafruit_LSM303_Mag_Unified mag = Adafruit_LSM303_Mag_Unified(-1);
  
#else 
  int boardName = -1;

#endif

void displaySensorDetails(void)
{
  sensor_t accel_sensor;
  accel.getSensor(&accel_sensor);
  
  sensor_t mag_sensor;
  mag.getSensor(&mag_sensor);
  
  Serial.println("------------------------------------");
  Serial.println("-------- LSM303 CONNECTED !!--------");
  Serial.print  ("Accelometer Sensor Unique ID:    "); Serial.println(accel_sensor.sensor_id);
  Serial.print  ("Magnetometer Sensor Unique ID:    "); Serial.println(mag_sensor.sensor_id);
  Serial.println("------------------------------------");
  Serial.println("");
  delay(500);
}
void readMag()
{
  /* Get a new sensor event */
  sensors_event_t event;
  mag.getEvent(&event);

  /* Display the results (magnetic vector values are in micro-Tesla (uT)) */
  Serial.print("Magnetic Reading -> X: "); Serial.print(event.magnetic.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.magnetic.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.magnetic.z); Serial.print("  ");Serial.println("uT");

  /* Delay before the next sample */
  delay(500);
}
void readAccel()
{
  /* Get a new sensor event */
  sensors_event_t event;
  accel.getEvent(&event);
  /* Display the results (acceleration is measured in m/s^2) */
  Serial.print("Acceleration Reading -> X: "); Serial.print(event.acceleration.x); Serial.print("  ");
  Serial.print("Y: "); Serial.print(event.acceleration.y); Serial.print("  ");
  Serial.print("Z: "); Serial.print(event.acceleration.z); Serial.print("  ");Serial.println("m/s^2 ");
  /* Delay before the next sample */
  delay(500);
  
}

void setup() 
{

  mySerial.begin(9600);
  Serial.begin(9600);
  
  if(boardName == MICRO)
  {
//       #ifndef ESP8266
//          while (!Serial);     // will pause Zero, Leonardo, etc until serial console opens
//       #endif
  
    /* Enable auto-gain */
    mag.enableAutoRange(true);
    
  /* Initialise the sensor */
    if(!accel.begin() || !mag.begin())
    {
        /* There was a problem detecting the ADXL345 ... check your connections */
       Serial.println("Ooops, no LSM303 detected ... Check your wiring!");
       while(1);
    }

    /* Display some basic information on this sensor */
     displaySensorDetails();
  }
 
}

void loop() 
{
  if(boardName == MEGA)
  {
    char key = keypad.getKey();

    if (key != NO_KEY)
    {
      mySerial.write(key);
    }
    
    if(mySerial.available())
    {
      Serial.print("MEGA TO PC, MICRO(ACK): ");
      Serial.println(char(mySerial.read()));
    }
  }
  else if(boardName == MICRO)
  {
    if(mySerial.available())
    {
      Serial.print("MICRO TO PC: ");
      char key = char(mySerial.read());
      
      switch(key)
      {
        case '1':
        {
          readAccel();
          Serial.println(key);
          mySerial.write(key);
          break;
        }
        case '2':
        {
          readMag();
          Serial.println(key);
          mySerial.write(key);
          break;
        }
        case '3':
        {
          readAccel();
          readMag();
          Serial.println(key);
          mySerial.write(key);
          break;
        }
      
      }
    }
  }
}






