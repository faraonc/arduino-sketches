
const int SS_ECHO_PIN = 2;
const int SS_TRIG_PIN = 3;
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
  Serial.begin (9600);
  pinMode(SS_TRIG_PIN, OUTPUT);
  pinMode(SS_ECHO_PIN, INPUT);
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

  }
  else 
  {
    /* Send the distance to the computer using Serial protocol, and
    turn LED OFF to indicate successful reading. */
    
    Serial.print(distanceInch);
    Serial.print(" inches, ");
    Serial.print(distanceCm);
    Serial.println(" cm");
  }
  
  //Delay 50ms before next reading.
  delay(50);

}
