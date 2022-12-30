#include <SPI.h>
#include <RFID.h>

#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN, RST_PIN);

void setup() {
 Serial.begin(9600);
 SPI.begin();
 rfid.init();
 Serial.println("Ready to scan RFID tag");
}

void loop() {
 if (rfid.isCard()) {
   Serial.println("Tag detected");
   if (rfid.readCardSerial()) {
     Serial.print("Card UID: ");
     for (byte i = 0; i < 5; i++) {
       //Serial.print(rfid.serNum[i] < 0x10 ? " 0" : " ");
       //Serial.println(rfid.serNum[i], HEX);
       Serial.print(rfid.serNum[i],DEC);
     }
     Serial.println();
   }
   delay(500);
 }
}
