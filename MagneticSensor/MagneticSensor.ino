/**
* Magnetic sensor controller
* 
* This is the basis for an escape room prop controller that is activiated by a magnet.
* It assumes the use of a linear, analogue Hall sensor (A3144), which is able
* to detect both proximity and polarity of a magnet.
* 
* The puzzle defines states which can be triggered. 
* 
* Input readings are smoothed using a rolling average functionwith a configurable number of samples,
* and an initial base reading is calibrated in the setup function to allow for background environmental noise.
*/
// inport libraries
#include <Stepper.h>

// Number of Hall Sensors
const byte numSensors = 2;
// Declair all pins 
const int buttonPin = 2;     // the number of the pushbutton pin
// The pin to which the linear, analogue hall sensor is attached
const byte sensorPins[numSensors] = {A1, A2};
// There are as many LEDs as there are sensors
const byte ledPins[numSensors] = {6, 7};
// There is one main light
const byte mainLightPin = 3;

// The motor which holds the key
const int stepsPerRevolution = 1500;  // change this to fit the number of steps per revolution
Stepper keyStepper(stepsPerRevolution, 8, 10, 9, 11);

// When the sketch first starts up, an average will be taken from a number of sample readings, from 
// the sensor, and this value will be updated to reflect the base
int baseReading = 1022;
byte buttonState = 0; 
bool lightsTurned = false;

// By how much must the average value deviate from the base reading to trigger a state change.
int sensitivity = 100;

// Define states for the puzzle, based on whether sensor is triggered , or not
enum State {Default, Triggered};
State triggerState[numSensors] = {State::Default};

// How many samples to include in the average. Increasing this will lead to a smoother value, 
// eliminating more noise or erroneous spikes in the output. However, it will also increase
// the time taken for the value to reflect any changes detected by the sensor - too high will 
// make the response feel 'laggy'
const int numSamples = 5;
// An array of readings from the sensor. When the array is full, new readings will replace the 
// oldest readings, acting like a circular buffer
int readings[numSamples] = {0,0,0,0,0};
// To calculate the average, we'll keep track of the total of all sample values currently in the array
int sampleTotal = 0;

// n is a counter to help us advance through the array and know which is the oldest reading (and hence next
// to be overwritten when  a new reading is received)
int n = 0;
// step count
int stepCount = 0;         // number of steps the motor has taken

// Adds a new sample to the rolling average
int AddSample(int newSample){
  // Take off the oldest reading, and add the new one to the total
  sampleTotal += (newSample - readings[n]);
  // Add the reading to the array, itself
  readings[n] = newSample;
  // Increment the counter
  n++;
  // If we hit the limit for the number of readings that can be stored,
  // Start again to overwrite the oldest values at the beginning of the array
  if (n >= numSamples) {
    n = 0;
  }
  // return the average of the newly-updated data

  return GetAverage();
}

// Returnes the current moving average from the array
int GetAverage(){
  return sampleTotal / numSamples;
}
bool solved = false;
void setup() { 
  // Begin a serial connection to use for monitoring
  Serial.begin(9600);
  // initialize pins
  pinMode(buttonPin, INPUT);
  
  // Turn all the leds on
  for (int i=0; i<numSamples; i++){
    pinMode(ledPins[i], OUTPUT); 
    pinMode(sensorPins[i], INPUT); 
    digitalWrite(ledPins[i], HIGH);
    }
    pinMode(mainLightPin, OUTPUT); 
    digitalWrite(mainLightPin, HIGH);

  // Fill the sample array with sample readings
  for (int i=0; i<numSamples; i++) {
    AddSample(analogRead(sensorPins));
  }

  Serial.print(F("Base Reading: "));
  Serial.println (baseReading);
    // set the speed at 20 rpm:
  keyStepper.setSpeed(20);
}

void loop() {
  if (lightsTurned == false){
    buttonState = digitalRead(buttonPin);
    if (buttonState == HIGH ){
      Serial.println("button pressed");
      delay(10000);
      for (int i=0; i<numSamples; i++){
        digitalWrite(ledPins[i], LOW);
      }
      digitalWrite(mainLightPin, LOW);
      lightsTurned = true;
      Serial.println("lights off!");
      delay(100);
    }
  }
  else if (solved == false){ 
    // pool for current reading averages for each sensor 
    for (int i=0; i < numSensors;i++){
       // Add the current sensor reading onto the array of samples
      int val = analogRead(sensorPins[i]);

      // If the average reading changes by more than tolerence

      if (val < baseReading - sensitivity || val > baseReading + sensitivity){
         // ... and wasn't previously 
         if (triggerState[i] != State::Triggered) {
         // Call the trigger function to act up on the state change
         onTrigger(i);
        }
        // Update the current state
        triggerState[i] = State::Triggered;
    }
    else {
      // Current average reading is within tolerance
      triggerState[i] = State::Default;
      Serial.println("not solved");
    }
    }
    // no need to check sensor every single frame, let's conserve power by adding a slight delay
    delay(10);
  }
  else{}
}

// This function is called when the average reading drops by more than specified sensitivity 
// You can use it to light an LED, trigger a relay, unlock a maglock etc.
void onTrigger(int currentLight){
  Serial.println("trigger activated!");
  digitalWrite(ledPins[currentLight], HIGH);
  checkSolution();

  // check to see if all the lights have been turned back on.
  // if so puzzle solved, drop key.
}
void checkSolution (){
  // if all lights are on the puzzle is solved
  Serial.println("checking solution.");
  bool lightOff = false;
  for (int i=0; i < numSensors; i++){
    if (digitalRead(ledPins[i]) == LOW){
      lightOff = true;
      return; 
    }
    else {
      lightOff = false;
    }
  }
  if (lightOff == false){
   onPuzzleSolved();
  }
}
void onPuzzleSolved(){
  // Turn on main light
  Serial.println("Solved.");
  digitalWrite(mainLightPin, HIGH);
  // drop key
  keyStepper.step(stepsPerRevolution);
  Serial.println("turning motor.");
  solved =  true;
   keyStepper.step(-stepsPerRevolution);


}
