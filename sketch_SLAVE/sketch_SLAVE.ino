/*
   SLAVE Arduino
   @Authors Pouria Ghadimi
            Conard James Faraon

*/

// include libraries
#include <LiquidCrystal.h>
#include<DHT.h>


#define DHTPIN 6     // what digital pin we're connected to
#define DHTTYPE DHT11   // DHT 11


/*--------------------------------------------------------------
 * CONST
 *--------------------------------------------------------------
 */
//initialize pins
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
const int BUTTON_PIN = 7;

//button
const unsigned long DEBOUNCE_DELAY = 50;


/*--------------------------------------------------------------
 * Unsigned
 *--------------------------------------------------------------
 */
// Communications
unsigned int SYN = 0;
unsigned int ACK_MASTER = 0;
unsigned int incomingByte = 0; 

//button
unsigned long previousMillisLED;        // will store last time LED was updated
unsigned long previousMillisKey;        // will store last time a key was pressed
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggle

/*--------------------------------------------------------------
 * INTS
 *--------------------------------------------------------------
 */
//PIR
int PIRPin = 22;               // choose the input pin (for PIR sensor)
int pirState = LOW;             // we start, assuming no motion detected
int val = 0;                    // variable for reading the pin status

//Button 
int buttonState = LOW;
int lastButtonState = LOW;

/*--------------------------------------------------------------
 * FLOATS
 *--------------------------------------------------------------
 */
//DHT
float TEMPERATURE, HUMIDITY, FAHRENHEIT;


/*--------------------------------------------------------------
 * BOOLEANS
 *--------------------------------------------------------------
 */
//PIR
bool motionDetected = false;

//button
bool isButtonPressed = false;


//Initializing needed components
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
DHT dht(DHTPIN, DHTTYPE);





/*--------------------------------------------------------------
 *Button
 *when pushed do a handshake and buzz on MASTER side
 *--------------------------------------------------------------
 */

void resetInput()
{
  previousMillisKey = 0;
}

void buttonPressed()
{

  // read the state of the pushbutton value:
  int reading = digitalRead(BUTTON_PIN);
  //Serial.println(reading);

  if (reading != lastButtonState)
  {
    // reset the debouncing timer
    lastDebounceTime = millis();

  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY)
  {
    //Serial.println(reading);

    if (reading != buttonState)
    {
      buttonState = reading;

      if (buttonState == HIGH)
      {
        isButtonPressed = true;
        //Serial.println("XXXX");
        sendSYN();
      }
    }
  }

  // save the reading. Next time through the loop, it'll be the lastButtonState:
  lastButtonState = reading;

}

/*
   A function to collect and show the data form
   DHT11 - Temperature and Humidity sensor
*/
void showTempAndHumid() {
  //delay(2000);

  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  HUMIDITY = dht.readHumidity();
  // Read temperature as Celsius (the default)
  TEMPERATURE = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  FAHRENHEIT = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(HUMIDITY) || isnan(TEMPERATURE) || isnan(FAHRENHEIT)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(FAHRENHEIT, HUMIDITY);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(TEMPERATURE, HUMIDITY, false);

}

/*
 * Motion Sensor PIR
 * Sends an 'M' to MASTER when detects motion
 */
void PIR(){
  
  val = digitalRead(PIRPin);  // read input value
  if (val == HIGH ) {            // check if the input is HIGH
    if (pirState == LOW) {
      // we have just turned on
      // We only want to print on the output change, not state
      pirState = HIGH;
      motionDetected = true;
      sendSYN();
    }
  } else {
    if (pirState == HIGH){
      // we have just turned of
      // We only want to print on the output change, not state
      pirState = LOW;
      motionDetected = false;
    }
  }
}



/*
   SENDING SYN
   a function to send SYN to MASTER
*/
void sendSYN() {
  Serial.write(++SYN);
}


/*
   CheckMSG
   a funvtion to receive an acknowlegment from MASTER
*/
void CheckMSG() {
  if (Serial.available() > 0)
  {
    if ( SYN == (unsigned int) incomingByte)
    {
      if (isButtonPressed)
      {
        Serial.write('B');
        Serial.write('S');
        isButtonPressed = false;
      }
      else if (motionDetected)
      {
        Serial.write('M');
        Serial.write('S');
        
      }
    }
    else {
      --SYN;
    }
  }
}

/*
   Setup
*/
void setup() {

  //testing if button turns BULTIN_LED on
  digitalWrite(LED_BUILTIN, LOW);

  pinMode(BUTTON_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);

  //Serial just to Test if receive any data
  Serial.begin(9600);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);


  dht.begin();
}


/*
   Main Loop
*/
void loop() {



  buttonPressed();

  // (note: line 1 is the second row, since counting begins with 0):
  showTempAndHumid();
  PIR();

  if (motionDetected){
    // set the cursor to column 0, line 0
    lcd.setCursor(0, 0);
    lcd.print ("Welcome");
  
    // set the cursor to column 0, line 1
    lcd.setCursor(0, 1);
    lcd.print("Buzz for answer");

    if (isButtonPressed){
      lcd.setCursor(0, 1);
      lcd.print("DING DONG :D !!");
    }
  }

  

  CheckMSG();

}

