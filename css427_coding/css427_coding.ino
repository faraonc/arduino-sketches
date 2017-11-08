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
const byte SS_ECHO_PIN = 2;
const byte SS_TRIG_PIN = 3;
const int MAX_RANGE_INCH = 7;
const int MIN_RANGE_INCH = 0;

volatile long echoStart;
volatile long echoEnd;
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

volatile QueueList<byte> key_row;
volatile QueueList<byte> key_col;

byte rowPins[ROWS] = {4, 5, 6, 7}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {21, 20, 19}; //connect to the column pinouts of the keypad
/*******************************/
/*******************************/


void keyPadTask()
{
  noInterrupts();
  Serial.println(keys[key_row.pop()][key_col.pop()]);
  //Arduino Documentation: Serial data received while in the ISR may be lost.
  //better to enable interrupts after Serial.prints
  interrupts();
}

void ultraSonicTask()
{
  noInterrupts();
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
  //Arduino Documentation: Serial data received while in the ISR may be lost.
  //better to enable interrupts after Serial.prints
  interrupts();
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
      break;
  }

  Task t;
  t.function = ultraSonicTask;
  task_queue.push(t);
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
    key_row.push(tmp);
    key_col.push(0);

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
    key_row.push(tmp);
    key_col.push(1);

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
    key_row.push(tmp);
    key_col.push(2);

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
  digitalWrite(SS_TRIG_PIN, LOW);
  delayMicroseconds(2);
  digitalWrite(SS_TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite(SS_TRIG_PIN, LOW);

  while (!task_queue.isEmpty())
  {
    Task t = task_queue.pop();
    t.function();
  }

}

