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
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  int boardName = UNO;
  
#elif defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  int boardName = MICRO;
  
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
  
#else 
  int boardName = -1;

#endif

//between MICRO and MEGA
SoftwareSerial mySerial(10,11);

void setup() 
{
  //MICRO to PC
  if(boardName == MICRO)
  {
    Serial.begin(9600);
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
  }
  
  if(boardName == MICRO)
  {
    if(mySerial.available() > 0)
    {
      Serial.print("MICRO TO PC: ");
      Serial.println(mySerial.read());
    }
  }
}






