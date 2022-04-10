/*
 * --------------------------------------------------------------------------------------------------------------------
 * Four RFID card readers looking for correct placement of tags
 * --------------------------------------------------------------------------------------------------------------------
 * This uses a MFRC522 for further details and other examples see: https://github.com/miguelbalboa/rfid
 * 
 
 * 
 * @author Maggie p
 *
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno           Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */
#define DEBUG
#include <SPI.h>
#include<MFRC522.h>


//Globals
// number of rfid readers
#define numReaders 4
//shared reset pin
#define resetPin 8

// each reader has a unique slave select pin
byte ssPins[] = {2, 3, 4, 5};

// initialise an array of MFRC522 instances
MFRC522 mfrc522[numReaders];

// The sequence of NFC tag IDs required to solve the puzzle
const String correctIDs[] =  {"F69F34F9","B68332F9","56c0ffa2","46405cd9"};
// The tag IDs currently detected by each reader
String currentIDs[numReaders];
// this pin will be driven LOW to release a lock when puzzle is solved
const byte lockPin = A0;


// initialisation
void setup() {

  #ifdef DEBUG
  // initialise serial communications channel with the PC
  Serial.begin(9600);
  Serial.println(F("Serial communication started"));
  #endif
  
  // Set the lock pin as output and secure the lock
  pinMode(lockPin, OUTPUT);
  digitalWrite(lockPin, HIGH);

  // Initialise the SPI bus
  SPI.begin();         

  for (uint8_t i =0; i < numReaders; i++) {
    // Initialise the reader
    // Note that SPI pins on the reader must always be connected to certain
    // Arduino pins see chart at the top. 
    mfrc522[i].PCD_Init(ssPins[i], resetPin);

    // Set the gain to max - not sure this make any difference ...
    // mfrc522[i].PCD_setAntennaGain(MFRC522::PCD_RxGain::RxGain_max);
    // Dump some debug info to the serial monitor
    Serial.print (F("Reader #"));
    Serial.print (i);
    Serial.print (F(" initialised on pin "));
    Serial.print (String(ssPins[i]));
    Serial.print (F(". Antenna strength: "));
    Serial.print (mfrc522[i].PCD_GetAntennaGain());
    Serial.print (F(". Version : "));
    mfrc522[i].PCD_DumpVersionToSerial();

    // Slight delay before activating next reader
    delay(100);
  }
  Serial.println (F("-- END SETUP --"));
  
 }

// main loop
void loop() {
  
  // Assume that the puzzle has been solved
  boolean puzzleSolved = true;
  
  // Assume that the tags have not changed since last reading
  boolean changedValue = false;

  // Loop through each reader
  for (uint8_t i = 0; i < numReaders; i++) {
    // Initialise the sensor
    mfrc522[i].PCD_Init();

    // String to hold the ID detected by each sensor
    String readRFID = "";

    // If the sensor detects a tag and is able to read is
    if(mfrc522[i].PICC_IsNewCardPresent() && mfrc522[i].PICC_ReadCardSerial()){
      // Extract the ID from the tag
     readRFID = dump_byte_array(mfrc522[i].uid.uidByte, mfrc522[i].uid.size);
     Serial.println("tag detected");
    }
    
  // If the current reading is different from the last known reading 
  if(readRFID != currentIDs[i]){
    // Set the flag to show that the puzzle state has changed
    changedValue = true;
    // Update the stored value for this sensor
    currentIDs[i] = readRFID;
  }

  // If the reading fails to match the correct ID for this sensor
  if(currentIDs[i] != correctIDs[i]) {
    // The puzzle has not been solved
    puzzleSolved = false;
  }
  
  // Halt PICC
  mfrc522[i].PICC_HaltA();
  // Stop encryption on PCD
  mfrc522[i].PCD_StopCrypto1();
  }

  // If the changedValue flag has been set, at least one sensor has changed 
  if(changedValue){
    // Dump to serial the current state of all sensors
    for (uint8_t i=0; i<numReaders; i++){
      Serial.print (F("Reader #"));
      Serial.print (String(i));
      Serial.print(F(" on Pin #"));
      Serial.print(String((ssPins[i])));
      Serial.print(F(" detected tag: "));
      Serial.println (currentIDs[i]);
    }
    Serial.println(F("---"));
  }
  // If the puzzleSolved flag is set, all sensors detected the correct ID 
  if(puzzleSolved){
    onSolve();
  }
}
/**
 * Helper routine to dump a byte array as hex values to Serial.
 */
String dump_byte_array(byte *buffer, byte bufferSize) {
  String buff = "";
  for (byte i = 0; i < bufferSize; i++) {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    buff= buff + (buffer[i], HEX);
  }
  return buff;
}
  // Add a short delay before next polling sensors
  // delay(100);
  
  // Called when correct
  void onSolve(){
    #ifdef DEBUG
    // Print debugging message
    Serial.println(F("Puzzle Solved!"));
    #endif

    // Release the lock
    digitalWrite(lockPin, LOW);

    while(true){}
  }
  
