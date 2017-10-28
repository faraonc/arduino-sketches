void setup()
{
  Serial.begin(9600);
}

void loop()
{
  Serial.print('H');
  delay(200);
  Serial.print('L');
  delay(200);
}
