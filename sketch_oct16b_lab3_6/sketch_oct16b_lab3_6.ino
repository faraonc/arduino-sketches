/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * Author: Yang Peng
 * CSS427 Fall 2017 Lab3 Ex6
 * Board used: Arduino Mega
 */
 
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
	{'1','2','3'},
	{'4','5','6'},
	{'7','8','9'},
	{'*','0','#'}
};

volatile byte key_row;
volatile byte key_col;
volatile bool isFound;

byte rowPins[ROWS] = {4, 5, 6, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {21, 2, 3}; //connect to the column pinouts of the keypad


void keyCol0()
{
	noInterrupts();

	int tmp = -1;
	for (byte i = 0; i < ROWS; i++)
	{
		pinMode(rowPins[i], INPUT);

		//delay to wait for the pin to change mode.
		//breaks without this delay
		delayMicroseconds(1000);
	
		if (digitalRead(rowPins[i]) == HIGH)
		{
			pinMode(rowPins[i], OUTPUT);
			tmp = i;
		}
		else
		{
			pinMode(rowPins[i], OUTPUT);
		}
	}

	if(tmp != -1)
	{
		key_row = tmp;
		key_col = 0;
		isFound = true;
		
	}
	interrupts();
}


void keyCol1()
{
	noInterrupts();

	int tmp = -1;
	for (byte i = 0; i < ROWS; i++)
	{
		pinMode(rowPins[i], INPUT);

		//delay to wait for the pin to change mode.
		//breaks without this delay
		delayMicroseconds(1000);
	
		if (digitalRead(rowPins[i]) == HIGH)
		{
			pinMode(rowPins[i], OUTPUT);
			tmp = i;
		}
		else
		{
			pinMode(rowPins[i], OUTPUT);
		}
	}

	if(tmp != -1)
	{
		key_row = tmp;
		key_col = 1;
		isFound = true;
		
	}
	interrupts();
}

void keyCol2()
{  
	noInterrupts();

	int tmp = -1;
	for (byte i = 0; i < ROWS; i++)
	{
		pinMode(rowPins[i], INPUT);

		//delay to wait for the pin to change mode.
		//breaks without this delay
		delayMicroseconds(1000);
	
		if (digitalRead(rowPins[i]) == HIGH)
		{
			pinMode(rowPins[i], OUTPUT);
			tmp = i;
		}
		else
		{
			pinMode(rowPins[i], OUTPUT);
		}
	}

	if(tmp != -1)
	{
		key_row = tmp;
		key_col = 2;
		isFound = true;
		
	}
	interrupts();
}

void setup()
{
	isFound = false;
	key_row = -1;
	key_col = -1;
	Serial.begin(9600);

	for(byte i = 0; i < COLS; i++)
	{
		pinMode(colPins[i], INPUT_PULLUP);
		digitalWrite(colPins[i], HIGH);
	 }

	for(byte i = 0; i < ROWS; i++)
	{
		pinMode(rowPins[i], OUTPUT);
		digitalWrite(rowPins[i], LOW);
	}

	attachInterrupt(digitalPinToInterrupt(colPins[0]), keyCol0, CHANGE);
	attachInterrupt(digitalPinToInterrupt(colPins[1]), keyCol1, CHANGE);
	attachInterrupt(digitalPinToInterrupt(colPins[2]), keyCol2, CHANGE);
	
}

void keyScan()
{
	if (isFound)
	{
		Serial.println(keys[key_row][key_col]);
		isFound = false;
	} 
}

void loop()
{
	keyScan();
}



