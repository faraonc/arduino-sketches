// Custom keypad interrupt service routines
void keyCol0 // col 0 ISR
{
    noInterrupts();
    col0State = true;
    printBool = true;
    interrupts();
}
void keyscan() {
    for (byte r = 0; r < rows; r++) {
      digitalWrite(rowPins[r], HIGH);
      if (col0State) {
    if (digitalRead(colPins[0]) == HIGH) {
            printKey = keys[r][0]; // save the key
            col0State = false;
            break;
    }
      }
    }
    for(byte i = 0;i <ROWS;i++){
        digitalWrite(rowPins[i],LOW);
    }
}

void setup(){
    for(byte i = 0;i <ROWS;i++){
        pinMode(rowPins[i],OUTPUT);
        digitalWrite(rowPins[i],LOW);
    }
    for(byte i = 0;i <COLS;i++){
        pinMode(colPins[i],INPUT_PULLUP);
    }
    attachInterrupt(somepin,keyCol0,FALLING);
}

void loop(){
    if(printBool){
    detachInterrupt(somepin);
    keyScan();
    delay(100);
    attachInterrupt(somepin,keyCol0,FALLING);
    Serial.println(printKey);
    printBool = false;
    }
}
