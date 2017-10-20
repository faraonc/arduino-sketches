#if ARDUINO >= 100
  #include "Arduino.h"
#else
  #include "WProgram.h"
#endif

// determine board type
#if defined(__AVR_ATmega328P__) || defined(__AVR_ATmega168__)
  String boardName = "Arduino Uno or older";
  
#elif defined(__AVR_ATmega32U4__) || defined(__AVR_ATmega16U4__)
  String boardName = "Arduino Micro or Leonardo";
  
#elif defined(__AVR_ATmega1280__) || defined(__AVR_ATmega2560__)
  String boardName = "Arduino Mega";
  
#else 
  String boardName = "Unknown board.  Check wikipedia";

#endif

void setup() {
  Serial.begin(9600);
}

void loop() {
  Serial.print("Detected Arduino Board to be: ");
  Serial.println(boardName);
}
