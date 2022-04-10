/*
 * Press a button to increase the count
 * Count is represented in binary on four LEDS

  The circuit:
  - LED attached from pins 13, 12, 11, 10 to ground
  - pushbutton attached to pin 2 from +5V
  - 10K resistor attached to pin 2 from ground

  - Note: on most Arduinos there is already an LED on the board
    attached to pin 13.
*/


const int ceiling = 9;    // max value of the to be represented 
const int buttonPin = 2;  // number of the button input pin
const int ledPins[] = {13, 12, 11, 10};       // an array of pin numbers to which LEDs are attached
int ledValues[] = {0, 0, 0, 0};     // an array of values for the LEDs
int pinCount = 4;
int buttonState = 0;
int lastButtonState = buttonState;
int count = 0; 

void setup() {
  // the array elements are numbered from 0 to (pinCount - 1).
  // use a for loop to initialize each pin as an output:
  for (int thisPin = 0; thisPin < pinCount; thisPin++) {
    pinMode(ledPins[thisPin], OUTPUT);
  }
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT);
}

void loop() {

  
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  if (buttonState != lastButtonState){

    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (buttonState == HIGH) {  
      int randomNumber = random(0,10);
   /* count++;
      if (count <= ceiling){
        // increase count
        lightUpConfig(0);
      } else {
        // reset to zero
        count = 0;
        resetLights();
      } 
      lightUp();*/
      resetLights();
      for (int i =0; i<randomNumber; i++){
        lightUpConfig(0);
        lightUp();
        delay(150);
      }
    }
    lastButtonState = buttonState;
  }
}
void lightUpConfig (int x){
  if (ledValues[x] == 0){
    ledValues[x] = 1;
  } else {
    ledValues[x] = 0;
    x++;
    lightUpConfig(x);
  }
}

void lightUp (){
  for (int thisPin = 0; thisPin < pinCount; thisPin++){
    if (ledValues[thisPin] == 1 ){
      // turn the pin on: 
      digitalWrite(ledPins[thisPin], HIGH);
    } else {
      // turn the pin off:
      digitalWrite(ledPins[thisPin], LOW); 
    }
  }
}
void resetLights(){
  // reset the LEDs to off.
  for (int thisPin = 0; thisPin < pinCount; thisPin++){
    ledValues[thisPin] = 0;
  }
  lightUp();
}
