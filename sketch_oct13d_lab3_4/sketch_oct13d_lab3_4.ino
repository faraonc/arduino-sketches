/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab3 Ex4
 * Board used: Arduino Mega
 */
 
#include <Keypad.h>

unsigned long interval = 2000; // the time we need to wait
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

char key;
boolean blink = false;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
  
void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  keypad.addEventListener(keypadEvent);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop()
{
  char key = keypad.getKey();

  if (key != NO_KEY)
  {
    Serial.println(key);
  }
}

void keypadEvent(KeypadEvent key)
{
  switch (keypad.getState())
  {
    case PRESSED:
    switch (key)
    {
      case '*': 
        digitalWrite(LED_BUILTIN, LOW);
        break;
    }
    break;
    
    case HOLD:
      switch (key)
      {
        case '#': 
          // grab current time
          unsigned long prevMillis = millis();
          
          // check if "interval" time has passed (2000 milliseconds)
          while (keypad.getState() == HOLD  && ((unsigned long)(millis() - prevMillis)) < interval) 
          {
            if(((unsigned long)(millis() - prevMillis)) >= interval)
            {
              digitalWrite(LED_BUILTIN, HIGH);
              break;
              }
           }
           
          break;
      }
    break;
  }

}
