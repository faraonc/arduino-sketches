
int analoginput = A0; // our analog pin
int analogamount = 0; // stores incoming value
float percentage = 0; // used to store our percentage value
float voltage =0; // used to store voltage value
 
void setup()
{
  Serial.begin(9600);
  analogReference(EXTERNAL); // use AREF for reference voltage
}
 
void loop()
{
  analogamount=analogRead(analoginput);
  percentage=(analogamount/1024.00)*100;
  voltage=analogamount*3.222; // in millivolts
  Serial.print("% of AREF: ");
  Serial.print(percentage,2);

  Serial.print("A0 (mV): ");
  Serial.println(voltage,2);
  delay(250);
}
