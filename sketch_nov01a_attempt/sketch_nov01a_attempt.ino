#include <SoftwareSerial.h>
#define TIMEOUT 5000 // ms

SoftwareSerial mySerial(52, 53); // RX, TX

void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  SendCommand("AT+RST", "Ready");
  SendCommand("AT+CWJAP=\"Starxf\",\"Carsomyr\"", "OK");
  delay(5000);
  SendCommand("AT+CWMODE=1", "OK");
  SendCommand("AT+CIFSR", "OK");
  SendCommand("AT+CIPMUX=1", "OK");
  SendCommand("AT+CIPSERVER=1,80", "OK");
}

void loop() {
  if (Serial.available())
  {
    Serial.readString();
    SendCommand("AT+CIPSEND=0,9", ">");
    mySerial.print("<h1>Hello</h1>");
    delay(2000);
    SendCommand("AT+CIPCLOSE=0", "OK");
  }

}

boolean SendCommand(String cmd, String ack) {
  mySerial.println(cmd); // Send "AT+" command to module
  if (!echoFind(ack)) // timed out waiting for ack string
    return true; // ack blank or ack found
}

boolean echoFind(String keyword) {
  byte current_char = 0;
  byte keyword_length = keyword.length();
  long deadline = millis() + TIMEOUT;
  while (millis() < deadline) {
    if (mySerial.available()) {
      char ch = mySerial.read();
      Serial.write(ch);
      if (ch == keyword[current_char])
        if (++current_char == keyword_length) {
          Serial.println();
          return true;
        }
    }
  }
  return false; // Timed out
}
