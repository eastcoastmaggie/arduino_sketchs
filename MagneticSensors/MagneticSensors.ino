/** 
* 
* Magnetic Sensor
* 
* This is a simple sketch using hall sensors 
*/

const byte numSensors = 1;
const byte sensorPins[numSensors] = {A1};
void setup() {
  // Serial connection to use for monitoring
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for (int i=0; i< numSensors; i++){
    int val = analogRead(sensorPins[i]);
    Serial.print(val);
    Serial.print(",");
  }
  Serial.println();
}
