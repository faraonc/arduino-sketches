/*
   Conard James B. Faraon
   CSS427 Coding Assignment
   Board used: Arduino Mega
*/

#include <QueueList.h>

/* TASK for function pointers. */
struct Task
{
  volatile void (*function)();
};

volatile QueueList<struct Task> task_queue;
/*******************************/
/*******************************/

/* ULTRASONIC variables */
const byte SS_ECHO_PIN = 20;
const byte SS_TRIG_PIN = 8;
const int MAX_RANGE_INCH = 7;
const int MIN_RANGE_INCH = 0;

volatile long echoStart;
volatile long echoEnd;
volatile bool isUSSReady;
volatile QueueList<long> duration;
/*******************************/
/*******************************/

/* KEYPAD variables */
const byte ROWS = 4;
const byte COLS = 3;

char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

volatile byte key_row;
volatile byte key_col;
volatile bool isFound;

byte rowPins[ROWS] = {4, 5, 6, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {21, 2, 3}; //connect to the column pinouts of the keypad
/*******************************/
/*******************************/

void emitWaves()
{
  digitalWrite(SS_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(SS_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SS_TRIG_PIN, LOW);
}

void keyPadTask()
{
  //isFound variable is also used for debouncing keypad
  if (isFound)
  {
    Serial.println(keys[key_row][key_col]);
    isFound = false;
  }
}

void ultraSonicTask()
{
  //Calculate the distance (in cm) based on the speed of sound.
  long dur = duration.pop();
  int distanceCm = dur * 0.034 / 2;
  int distanceInch = dur * 0.0133 / 2;
  if (distanceInch <= MAX_RANGE_INCH && distanceInch >= MIN_RANGE_INCH)
  {
    Serial.print(distanceInch);
    Serial.print(" inches, ");
    Serial.print(distanceCm);
    Serial.println(" cm");
  }
  delay(60);
}

void ISR_echo()
{
  noInterrupts();
  // Test to see if the signal is high or low
  switch (digitalRead(SS_ECHO_PIN))
  {
    // High so must be the start of the echo pulse
    case HIGH:
      echoEnd = 0; // Clear the end time
      echoStart = micros(); // Save the start time
      break;

    // Low so must be the end of hte echo pulse
    case LOW:
      echoEnd = micros(); // Save the end time
      duration.push(echoEnd - echoStart); // Calculate the pulse duration
      isUSSReady = true;
      break;
  }

  if (isUSSReady)
  {
    Task t;
    t.function = ultraSonicTask;
    task_queue.push(t);
    isUSSReady = false;
  }
  interrupts();
}

void ISR_keyCol0()
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

  if (tmp != -1)
  {
    key_row = tmp;
    key_col = 0;
    isFound = true;
  }
  Task t;
  t.function = keyPadTask;
  task_queue.push(t);
  interrupts();
}

void ISR_keyCol1()
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

  if (tmp != -1)
  {
    key_row = tmp;
    key_col = 1;
    isFound = true;
  }
  Task t;
  t.function = keyPadTask;
  task_queue.push(t);
  interrupts();
}

void ISR_keyCol2()
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

  if (tmp != -1)
  {
    key_row = tmp;
    key_col = 2;
    isFound = true;
  }
  Task t;
  t.function = keyPadTask;
  task_queue.push(t);
  interrupts();
}

void setup()
{

  Serial.begin (9600);

  /* ULTRASONIC variables */
  echoStart = 0;
  echoEnd = 0;
  isUSSReady = false;
  pinMode(SS_TRIG_PIN, OUTPUT);
  pinMode(SS_ECHO_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SS_ECHO_PIN), ISR_echo, CHANGE);
  /*******************************/
  /*******************************/

  /* KEYPAD variables */
  for (byte i = 0; i < COLS; i++)
  {
    pinMode(colPins[i], INPUT_PULLUP);
    digitalWrite(colPins[i], HIGH);
  }

  for (byte i = 0; i < ROWS; i++)
  {
    pinMode(rowPins[i], OUTPUT);
    digitalWrite(rowPins[i], LOW);
  }

  attachInterrupt(digitalPinToInterrupt(colPins[0]), ISR_keyCol0, CHANGE);
  attachInterrupt(digitalPinToInterrupt(colPins[1]), ISR_keyCol1, CHANGE);
  attachInterrupt(digitalPinToInterrupt(colPins[2]), ISR_keyCol2, CHANGE);
  /*******************************/
  /*******************************/
}

void loop()
{
  while (!task_queue.isEmpty())
  {
    Task t = task_queue.pop();
    t.function();
  }

  //does not make sense placing it in the function queue
  //because the ultra sonic sensor needs to run in the background.
  emitWaves();
}

