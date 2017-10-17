const byte rows = 4; //four rows
const byte cols = 3; //three columns
char keys[rows][cols] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[rows] = {4, 5, 6, 7}; //connect to the row pinouts of the keypad
byte colPins[cols] = {21, 2, 3}; //connect to the column pinouts of the keypad

volatile byte key[2];
volatile bool displayKey;

// Setup
void setup()
{
  //https://www.arduino.cc/en/Reference/AttachInterrupt
  //http://rasas.me/projects/arduino/keypad-activated-interrupt
  //https://www.parallax.com/sites/default/files/downloads/27899-4x4-Matrix-Membrane-Keypad-v1.2.pdf

  displayKey = false;
  
  // prepare keypad
  pinMode(colPins[0], INPUT_PULLUP);
  pinMode(colPins[1], INPUT_PULLUP);
  pinMode(colPins[2], INPUT_PULLUP);

  digitalWrite(colPins[0], HIGH);
  digitalWrite(colPins[1], HIGH);
  digitalWrite(colPins[2], HIGH);

  pinMode(rowPins[0], OUTPUT);
  pinMode(rowPins[1], OUTPUT);
  pinMode(rowPins[2], OUTPUT);
  pinMode(rowPins[3], OUTPUT);

  digitalWrite(rowPins[0], LOW);
  digitalWrite(rowPins[1], LOW);
  digitalWrite(rowPins[2], LOW);
  digitalWrite(rowPins[3], LOW);

  attachInterrupt(digitalPinToInterrupt(colPins[0]), col0isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(colPins[1]), col1isr, CHANGE);
  attachInterrupt(digitalPinToInterrupt(colPins[2]), col2isr, CHANGE);
  
  // prepare console output
  Serial.begin(9600);
}

// Loop
void loop()
{
  if (displayKey)
  {
    String output = String(keys[key[0]][key[1]]);
    Serial.println("got input from: " + output);
    displayKey = false;
  }
}

// Column 0 Interrupt Service Routine
void col0isr()
{
  noInterrupts();
  int row = getRow();

  if (row >= 0)
  {
    key[0] = row;
    key[1] = 0;
    displayKey = true;
  }
  interrupts();
}

// Column 1 Interrupt Service Routine
void col1isr()
{
  noInterrupts();
  int row = getRow();
  
  if (row >= 0)
  {
    key[0] = row;
    key[1] = 1;
    displayKey = true;
  }
  interrupts();
}

// Column 2 Interrupt Service Routine
void col2isr()
{
  noInterrupts();
  int row = getRow();
  
  if (row >= 0)
  {
    key[0] = row;
    key[1] = 2;
    displayKey = true;
  }
  interrupts();
}

// Get Row
int getRow()
{
  for (int i = 0; i < rows; i++)
  {
    pinMode(rowPins[i], INPUT);
    delay(1);
    
    if (digitalRead(rowPins[i]) == HIGH)
    {
      pinMode(rowPins[i], OUTPUT);
      return i;
    }
    else
    {
      pinMode(rowPins[i], OUTPUT);
    }
  }

  return -1;
}
