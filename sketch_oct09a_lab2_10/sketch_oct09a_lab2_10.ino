const int LED_PIN = 2;
const int SS_ECHO_PIN = 3;
const int SS_TRIG_PIN = 4;
const int BUZZER_PIN = 10;
const int maximumRangeInch = 7; 
const int minimumRangeInch = 0; 
long duration; 
int distanceCm;
int distanceInch;
int sound;

void setup() 
{
  duration = 0;
  distanceCm = 0;
  distanceInch = 0;
  sound = 0;
  Serial.begin (9600);
  pinMode(SS_TRIG_PIN, OUTPUT);
  pinMode(SS_ECHO_PIN, INPUT);
  pinMode(BUZZER_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT); // Use LED indicator (if required)

}

void buzzDistance()
{
  switch(distanceInch)
  {
    case 1:
      sound = 1500;
      break;
    case 2:
      sound = 1400;
      break;
    case 3:
      sound = 1300;
      break;
    case 4:
      sound = 1200;
      break;
    case 5:
      sound = 1100;
      break;
    case 6:
      sound = 1000;
  }

  tone(BUZZER_PIN, sound);
  Serial.print("Sound Level at ");
  Serial.println(sound);
  
}

void beep(unsigned char delayms){
  analogWrite(BUZZER_PIN, 10);      // Almost any value can be used except 0 and 255               
                           // experiment to get the best tone
  delay(delayms);          // wait for a delayms ms
  analogWrite(BUZZER_PIN, 0);       // 0 turns it off
  delay(delayms);          // wait for a delayms ms   
}  


void buzzBeep()
{
  unsigned char delay = 10 * distanceInch;
  beep(delay);
}

void loop() 
{
  /* The following trigPin/echoPin cycle is used to determine the
  distance of the nearest object by bouncing soundwaves off of it. */ 
  digitalWrite(SS_TRIG_PIN, LOW); 
  delayMicroseconds(2); 
  
  digitalWrite(SS_TRIG_PIN, HIGH);
  delayMicroseconds(10); 
  
  digitalWrite(SS_TRIG_PIN, LOW);
  duration = pulseIn(SS_ECHO_PIN, HIGH);
  
  //Calculate the distance (in cm) based on the speed of sound.
  distanceCm= duration * 0.034/2;
  distanceInch = duration * 0.0133/2;
    
  if (distanceInch >= maximumRangeInch || distanceInch <= minimumRangeInch)
  {
    /* Send a negative number to computer and Turn LED ON 
    to indicate "out of range" */
    Serial.println("-1");
    noTone(BUZZER_PIN);
    digitalWrite(LED_PIN, HIGH); 
  }
  else 
  {
    /* Send the distance to the computer using Serial protocol, and
    turn LED OFF to indicate successful reading. */
    
    Serial.print(distanceInch);
    Serial.println(" inches");
    Serial.print(distanceCm);
    Serial.println(" cm");
    digitalWrite(LED_PIN, LOW); 
    //buzzDistance();
    buzzBeep();
  }
  
  //Delay 50ms before next reading.
  delay(50);

}
