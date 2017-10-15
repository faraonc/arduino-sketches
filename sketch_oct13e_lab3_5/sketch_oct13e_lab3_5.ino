/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab3 Ex5
 * Board used: Arduino Mega
 */

const byte SS_ECHO_PIN = 2;
const byte SS_TRIG_PIN = 3;
const int MAX_RANGE_INCH = 7; 
const int MIN_RANGE_INCH = 0; 

volatile long echoStart;
volatile long echoEnd; 
volatile long duration; 
int distanceCm;
int distanceInch;
volatile boolean hasToRead;

void setup() 
{
  hasToRead = false;
  echoStart = 0;
  echoEnd = 0; 
  duration = 0;
  distanceCm = 0;
  distanceInch = 0;
  Serial.begin (9600);
  pinMode(SS_TRIG_PIN, OUTPUT);
  pinMode(SS_ECHO_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(SS_ECHO_PIN), ISR_echo, CHANGE);
}

void loop() 
{
  digitalWrite(SS_TRIG_PIN, LOW); 
  delayMicroseconds(2); 
  digitalWrite(SS_TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  digitalWrite(SS_TRIG_PIN, LOW);

  noInterrupts();
  if(hasToRead)
  {
    //Calculate the distance (in cm) based on the speed of sound.
    distanceCm= duration * 0.034/2;
    distanceInch = duration * 0.0133/2;
    
    if (distanceInch <= MAX_RANGE_INCH && distanceInch >= MIN_RANGE_INCH)
    {
      Serial.print(distanceInch);
      Serial.print(" inches, ");
      Serial.print(distanceCm);
      Serial.println(" cm");
    }
    hasToRead = false;
    
  }
  //Arduino Documentation: Serial data received while in the ISR may be lost.
  //better to enable interrupts after Serial.prints
  interrupts();  

  delay(60); 
}

void ISR_echo()
{
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
      duration = echoEnd - echoStart; // Calculate the pulse duration
      hasToRead = true;
      break;
  }
 
}


