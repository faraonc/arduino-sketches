


#include <SoftwareSerial.h>
#define TIMEOUT 5000 // mS
const byte LED = 13;
const byte BUTTON = 11;

SoftwareSerial espSerial(7, 6); // RX, TX

byte button_state = 0;

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(BUTTON, INPUT);

  Serial.begin(9600);
  espSerial.begin(9600);

  SendCommand("AT+RST", "Ready");
  delay(5000);
  SendCommand("AT+CWMODE=1", "OK");
  SendCommand("AT+CIFSR", "OK");
  SendCommand("AT+CIPMUX=1", "OK");
  SendCommand("AT+CIPSERVER=1,80", "OK");
}

void loop() {
  button_state = digitalRead(BUTTON);

  if (button_state == HIGH)
  {
    espSerial.println("AT+CIPSEND=0,23");
    espSerial.println("Button was pressed!");
    delay(1000);
    SendCommand("AT+CIPCLOSE=0", "OK");
  }

  String IncomingString = "";
  boolean StringReady = false;

  while (espSerial.available())
  {
    IncomingString = espSerial.readString();
    StringReady = true;
  }

  if (StringReady)
  {
    Serial.println("Received String: " + IncomingString);

    if (IncomingString.indexOf("LED=ON") != -1) {
      digitalWrite(LED, HIGH);
    }

    if (IncomingString.indexOf("LED=OFF") != -1) {
      digitalWrite(LED, LOW);
    }
  }
}

boolean SendCommand(String cmd, String ack)
{
  espSerial.println(cmd); // Send "AT+" command to module
  if (!echoFind(ack)) // timed out waiting for ack string
    return true; // ack blank or ack found
}

boolean echoFind(String keyword)
{
  byte current_char = 0;
  byte keyword_length = keyword.length();
  long deadline = millis() + TIMEOUT;
  while (millis() < deadline)
  {
    if (espSerial.available()) 
    {
      char ch = espSerial.read();
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
