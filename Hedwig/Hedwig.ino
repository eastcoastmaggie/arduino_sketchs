//www.elegoo.com
//2016.12.08

#include "pitches.h"
 
// notes in the melody:
int melody[] = {
  NOTE_B5, NOTE_E6, NOTE_G6, NOTE_FS6, NOTE_E6, NOTE_B6, NOTE_A6, NOTE_FS6, 0, NOTE_E6, NOTE_G6, NOTE_FS6, NOTE_DS6, NOTE_F6, NOTE_B5, 0, NOTE_B5, 
  NOTE_E6, NOTE_G6, NOTE_FS6, NOTE_E6, NOTE_B6, NOTE_D7, NOTE_CS7, NOTE_C7, NOTE_GS6, NOTE_C7, NOTE_B6, NOTE_AS6, NOTE_AS5, NOTE_G6, NOTE_E6, 0, NOTE_G6, NOTE_B6, NOTE_G6,
  NOTE_B6, NOTE_G6, NOTE_C7, NOTE_B6, NOTE_AS6, NOTE_FS6, NOTE_G6, NOTE_C7, NOTE_A6, NOTE_AS5, NOTE_A5, NOTE_B6, NOTE_G6, NOTE_B6, NOTE_G6,
  NOTE_B6, NOTE_G6, NOTE_D7, NOTE_CS7, NOTE_C7, NOTE_GS6, NOTE_C7, NOTE_B6, NOTE_AS6, NOTE_AS5, NOTE_G6, NOTE_E6, 0};
int duration[] = {4, 6, 2, 4, 8, 4, 12, 12, 4, 6, 2, 4, 8, 4, 20, 4, 4,
                  6, 2, 4, 8, 4, 8, 4, 8, 4, 6, 2, 4, 8, 4, 20, 4, 4, 8, 4,
                  8, 4, 8, 4, 8, 4, 6, 2, 4, 8, 4, 20, 4, 8, 4,
                  8, 4, 8, 4, 8, 4, 6, 2, 4, 8, 4, 20, 4};  // 500 miliseconds
 
void setup() {
 
}
 
void loop() {  
  for (int thisNote = 0;           , < 64; thisNote++) {
    // pin8 output the voice, every scale is 0.5 sencond
    tone(8, melody[thisNote], duration[thisNote]*40);
     
    // Output the voice after several minutes
    delay(500);
  }
   
  // restart after two seconds 
  delay(2000);
}
