#include "Arduino.h" // Include the core Arduino library
#include <SPI.h>
#include <MFRC522.h>
#include <SoftwareSerial.h> // Include SoftwareSerial library for non-ESP32 boards
#include "DFRobotDFPlayerMini.h" // Include the DFRobot DFPlayer Mini library

// Define the pins
#define SS_PIN 5
#define RST_PIN 4

MFRC522 rfid(SS_PIN, RST_PIN);
#ifdef ESP32
  #define FPSerial Serial1  // For ESP32, use hardware serial port 1
#else
  #include <SoftwareSerial.h> // Include SoftwareSerial library for non-ESP32 boards
  SoftwareSerial FPSerial(2, 1); // Define SoftwareSerial on pins GPIO2 (RX) and GPIO3 (TX)
#endif

DFRobotDFPlayerMini myDFPlayer; // Create an instance of the DFRobotDFPlayerMini class

void setup() {
  #ifdef ESP32
  FPSerial.begin(9600, SERIAL_8N1, 2, 1); // Start serial communication for XIAO ESP32C3 with 9600 baud rate, 8 data bits, no parity, and 1 stop bit
  #else
  FPSerial.begin(9600); // Start serial communication for other boards with 9600 baud rate
  #endif
  Serial.begin(115200);
  // Init serial port for DFPlayer Mini

  SPI.begin();  // Init SPI bus
  rfid.PCD_Init();  // Init MFRC522
  Serial.println("Place your RFID card near the reader...");

  Serial.println(F("DFRobot DFPlayer Mini Demo")); // Print a demo start message
  Serial.println(F("Initializing DFPlayer ... (May take 3~5 seconds)")); // Print initialization message
  
  if (!myDFPlayer.begin(FPSerial)) { // Initialize the DFPlayer Mini with the defined serial interface
    Serial.println(F("Unable to begin:")); // If initialization fails, print an error message
    Serial.println(F("1.Please recheck the connection!")); // Suggest rechecking the connection
    Serial.println(F("2.Please insert the SD card!")); // Suggest checking for an inserted SD card
    while(true); // Stay in an infinite loop if initialization fails
  }
  Serial.println(F("DFPlayer Mini online.")); // Print a success message if initialization succeeds
  myDFPlayer.volume(30);  // Set the DFPlayer Mini volume to 30 (max is 30)
  myDFPlayer.play(3);
}

void loop() {
  // Look for new cards
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial()) {
    return;
  }
    // Set volume to maximum (0 to 30).
   // player.volume(30);
    // Play the first MP3 file on the SD card
   // player.play(1);
  // Show UID on serial monitor
  Serial.print("UID tag: ");
  for (byte i = 0; i < rfid.uid.size; i++) {
    Serial.print(rfid.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(rfid.uid.uidByte[i], HEX);
  }
  Serial.println();
    if(rfid.uid.uidByte[0] == 99 && rfid.uid.uidByte[1] == 49 && rfid.uid.uidByte[2] == 134 && rfid.uid.uidByte[3] == 15)
    {
      Serial.println("play 1");
      delay(2000);
        // Play the first MP3 file on the SD card
        myDFPlayer.play(1);
    }else if(rfid.uid.uidByte[0] == 115 && rfid.uid.uidByte[1] == 146 && rfid.uid.uidByte[2] == 111 && rfid.uid.uidByte[3] == 9)
    {
      Serial.println("play 2");
      delay(2000);
       // myDFPlayer.next();
        // Play the first MP3 file on the SD card
        myDFPlayer.play(2);     
    }  
  // Halt PICC
  rfid.PICC_HaltA();
}