
const byte pinCount = 6;           // the number of pins (i.e. the length of the array)
const byte totalTime = 60;
const byte steps = 3;
const byte ballPin = 7;
byte previousVal = 0;
byte currentVal = 0;
byte sandLevel = 0;
byte ledPins[] = {
  13, 12, 11, 10, 9, 8
};       // an array of pin numbers to which LEDs are attached


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin LED_BUILTIN as an output.
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(ledPins[thisPin], OUTPUT);
    digitalWrite(ledPins[thisPin], LOW);
  }
  // initialize Ball switch
  pinMode(ballPin, INPUT);
  digitalWrite(ballPin, HIGH);
  previousVal = 0;
  sandLevel = 0;

}

// the loop function runs over and over again forever
void loop() {
  // Check to see if rotated
  int digitalVal = digitalRead(ballPin);
  if (digitalVal == HIGH) {
    currentVal = 1;
  }
  else {
    currentVal = 0;
  }
  
  if (currentVal != previousVal) {
    // if rotated reverse array
    sandLevel = steps - sandLevel;
    byte tempArray[pinCount];
    memcpy(tempArray, ledPins, sizeof(ledPins));
    for (int thisPin = 0; thisPin < pinCount; thisPin++) {
      ledPins[thisPin] = tempArray[pinCount - 1 - thisPin];
    }
  }
  switch (sandLevel){
   // all dots at the top
    case 0: {
      for (int thisPin = 0; thisPin < pinCount / 2; thisPin++) {
        digitalWrite(ledPins[thisPin], HIGH);   // turn the LED on (HIGH is the voltage level)
      }
      for (int thisPin = pinCount / 2; thisPin < pinCount; thisPin++) {
        digitalWrite(ledPins[thisPin], LOW);   // turn the LED on (HIGH is the voltage level)
      }
      break;
    }
    case 1: {
      // 10 seconds, move first dot
      delay(1000);
      digitalWrite(ledPins[0], LOW);
      digitalWrite(ledPins[3], HIGH);
      delay(1000);
      digitalWrite(ledPins[3], LOW);
      digitalWrite(ledPins[4], HIGH);
      delay(1000);
      digitalWrite(ledPins[4], LOW);
      digitalWrite(ledPins[5], HIGH);
      break;
    }
    case 2: {
      // 10 seconds, move first dot
      delay(1000);
      delay(1000);
      digitalWrite(ledPins[1], LOW);
      digitalWrite(ledPins[3], HIGH);
      delay(1000);
      digitalWrite(ledPins[3], LOW);
      digitalWrite(ledPins[4], HIGH);
      break;
    }
    case 3: {
           // 10 seconds, move first dot
      delay(1000);
      delay(1000);
      delay(1000);
      digitalWrite(ledPins[2], LOW);
      digitalWrite(ledPins[3], HIGH);
      break;
    }
  }
  if (sandLevel < 3){
    sandLevel++;
  }
  previousVal = currentVal;
}
