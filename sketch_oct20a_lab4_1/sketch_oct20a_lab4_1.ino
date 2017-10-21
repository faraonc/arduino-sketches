 /* Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab4 Ex1
 * Board used: Arduino Mega, Micro
 */

#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

#include <Keypad.h>
#include <SoftwareSerial.h>

enum {
  UNO,
  MICRO,
  MEGA
};

// determine board type
#if defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  int boardName = MICRO;
  Keypad keypad = Keypad( NULL, 0, 0, 0, 0 );
  SoftwareSerial mySerial(8,9);
  
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
  byte rowPins[ROWS] = {5, 4, 3, 2}; //connect to the row pinouts of the keypad
  byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad
  Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
  SoftwareSerial mySerial(10,11);
  
#else 
  int boardName = -1;

#endif

void setup() 
{

  mySerial.begin(9600);
  Serial.begin(9600);
 
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
      Serial.println(key);
      mySerial.write(key);
    }
  }
}

