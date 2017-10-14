// constants won't change. They're used here to set pin numbers:
const int buttonPin = 10;     // the number of the pushbutton pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status

void setup() 
{
  // initialize the pushbutton pin as an input:
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(buttonPin, INPUT);
  
}

void loop() 
{
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);

  // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
    // turn LED on:
    digitalWrite(LED_BUILTIN, HIGH);
  } else {
    // turn LED off:
    digitalWrite(LED_BUILTIN, LOW);
  }
}
