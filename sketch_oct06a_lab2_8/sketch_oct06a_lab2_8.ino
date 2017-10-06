/*
 * Conard James B. Faraon
 * Pouria Ghadimi
 * CSS427 Fall 2017 Lab2 Ex8
 * Board used: Arduino Micro
 */

const int TEMP_PIN = A0;
const int MS = 1000;
const String DEF_MSG = "DEFAULT";
const String INT_MSG = "INTERNAL";
const String INT1V1_MSG = "INTERNAL1V1";
const String INT2V56_MSG = "INTERNAL2V56";
const String EXT_MSG = "EXTERNAL";
int temperatureR;
int incomingByte;
int volt_ref;
String string_ref;


void setup() 
{
  Serial.begin(9600);
  pinMode(TEMP_PIN, INPUT);
  incomingByte = 5; 
  volt_ref = DEFAULT;  //starting voltage reference is DEFAULT
  string_ref = DEF_MSG;
}

void loop() 
{
  if(Serial.available() > 0)
  {
    // read the incoming byte:
    incomingByte = Serial.parseInt();
    if(incomingByte >= 1 && incomingByte <= 5)
    {
      if(incomingByte == 1)
      {
        volt_ref = DEFAULT;
        string_ref = DEF_MSG;
      }
      else if (incomingByte == 2)
      {
        volt_ref = INTERNAL;
        string_ref = INT_MSG;
      } //Arduino Micro does not support the following types:
  //    else if (incomingByte == 3)
  //    {
  //      volt_ref = INTERNAL1V1;
  //      string_ref = INT1V1_MSG;
  //    }
  //    else if (incomingByte == 4)
  //    {
  //      volt_ref = INTERNAL2V56;
  //      string_ref = INT2V56_MSG;
  //    }
      else if (incomingByte == 5)
      {
        volt_ref = EXTERNAL;
        Serial.print("volt_ref = ");
        Serial.println(volt_ref);
        string_ref = EXT_MSG;
      }
      
      //provide an external voltage reference to this pin
       analogReference(volt_ref);
       //call analogRead() at least once to allow the voltage to settle down in the ADC circuitry
       analogRead(TEMP_PIN);
       Serial.print("\nVoltage Reference set to ");
       Serial.println(string_ref);
       delay(MS);
    }
  }

  temperatureR = analogRead(TEMP_PIN);
  float voltage = temperatureR * 5;
  voltage /= 1024.0;
  Serial.println();
  Serial.print(temperatureR); 
  Serial.println(" reading");
  Serial.println();
  Serial.print(voltage); 
  Serial.println(" volts");

  float temperatureC = (voltage - 0.5) * 100;
  Serial.print(temperatureC);
  Serial.println(" deg C"); 

  float temperatureF = (temperatureC * 9.0 / 5.) + 32.0;
  Serial.print(temperatureF);
  Serial.println(" deg F");
  delay(MS);

}


