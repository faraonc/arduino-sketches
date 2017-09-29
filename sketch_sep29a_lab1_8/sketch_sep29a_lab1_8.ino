int incomingByte = 0;   // for incoming serial data
int LED = 2;


void setup() {
  Serial.begin(9600);     // opens serial port, sets data rate to 9600 bps
  pinMode(LED, OUTPUT
}

void loop() {

  // send data only when you receive data:
  if (Serial.available() > 0) {
          // read the incoming byte:
          incomingByte = Serial.read();
  
          // say what you got:
          Serial.print("I received: ");
          Serial.println(incomingByte, DEC);

          if(incomingByte == 48)
          {
            
          }

          
  }
}
