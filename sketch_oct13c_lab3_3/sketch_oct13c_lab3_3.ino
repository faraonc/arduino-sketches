/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab3 Ex3
 * Board used: Arduino Mega
 */
 
#include <Keypad.h>

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
String equation;

char key;
boolean isFirstRound;
long product;

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
	
void setup()
{
	Serial.begin(9600);
	equation = "";
	isFirstRound = true;
	product = 1;
}

void multiply()
{
	String multiplicand = "";
	product = 1;
	for(int i = 0; i < equation.length(); i++)
	{
		if(equation.charAt(i) != '*')
		{
			multiplicand = multiplicand + String(equation.charAt(i));
		}
		else  //only when the * is hit
		{
			product = product * multiplicand.toInt();
			multiplicand = "";
		}
			
		if(i == (equation.length()-1))
		{
			product = product * multiplicand.toInt();
		}
	}
}

void loop()
{
	char key = keypad.getKey();

	if (key != NO_KEY)
	{
		if(isFirstRound)
		{
			if(key == '#' || key == '*')
			{
				Serial.println("We got # or *, first character should be an int.");
			}

			isFirstRound = false;
		}
		else if (key == '#')
		{
			if(equation.charAt(equation.length()-1) == '*')
			{
				Serial.println(equation);
				Serial.println("Invalid equation: Last character is *");
			}
			else
			{
				multiply();
				Serial.print(equation + String('=') + String(product));
			}
			
			equation = "";
			isFirstRound = true;
		}
		else
		{
			equation = equation + String(key);
			Serial.println(equation);
		}
	}
}
