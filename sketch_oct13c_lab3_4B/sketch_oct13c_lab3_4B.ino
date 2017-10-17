/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab3 Ex4B
 * Board used: Arduino Mega
 */
 
#include <Keypad.h>

const long BLINK_INTERVAL = 100;  //LED blink interval
const unsigned long TIME_LIMIT = 4000; //user has 4 seconds to enter input or input resets
const unsigned long DEBOUNCE_DELAY = 50; //button debounce delay
const String CODE = "2017427*"; //code to set the alarm
const int BUTTON_PIN = 10;

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

unsigned long previousMillisLED;        // will store last time LED was updated
unsigned long previousMillisKey;        // will store last time a key was pressed
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggle

char key;
boolean blinkState;
boolean alarmAck;
int buttonState;
int ledState; 
int lastButtonState;
String input;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
	
void setup()
{
	blinkState = false;
	alarmAck = false;
	buttonState = LOW;
	ledState = LOW;
	input = "";
	lastButtonState = LOW;
	previousMillisLED = 0;
	previousMillisKey = 0;
	Serial.begin(9600);
	pinMode(LED_BUILTIN, OUTPUT);
	keypad.addEventListener(keypadEvent);
	digitalWrite(LED_BUILTIN, LOW);
	pinMode(BUTTON_PIN, INPUT);
}

void blink()
{
	unsigned long currentMillisLED = millis();
	if (currentMillisLED - previousMillisLED >= BLINK_INTERVAL)
	{
		previousMillisLED = currentMillisLED;

		// if the LED is off turn it on and vice-versa:
		if (ledState == LOW) 
		{
			ledState = HIGH;
		} 
		else 
		{
			ledState = LOW;
		}

		// set the LED with the ledState of the variable:
		digitalWrite(LED_BUILTIN, ledState);
	}
}

void resetInput()
{
	previousMillisKey = 0;
	Serial.println("");
	input = "";
}

void loop()
{
	char key = keypad.getKey();
	if(blinkState)
	{
		Serial.println("ALARM!");
		blink();
	}
	
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
				if(!alarmAck)
				{
					alarmAck = true;
					Serial.println("Alarm Acknowledged!");
					blinkState = false;
					ledState = LOW;
					digitalWrite(LED_BUILTIN, ledState);
				}
			}
		}
	}

	// save the reading. Next time through the loop, it'll be the lastButtonState:
	lastButtonState = reading;
	
	if(previousMillisKey != 0)
	{
		if(millis() - previousMillisKey >= TIME_LIMIT)
		{
			Serial.println();
			Serial.println("TIMEOUT: Resetting Input");
			resetInput();
		}
	}
}

void keypadEvent(KeypadEvent key)
{
	switch (keypad.getState())
	{
		case PRESSED:
			if(!blinkState)
			{
				previousMillisKey = millis();
				input += String(key);
				Serial.print(key);
				if(input.equals(CODE))
				{
					alarmAck = false;
					blinkState = true;
					resetInput();
				}
			}
	}

}

