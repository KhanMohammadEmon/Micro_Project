#include <SPI.h>
#include <RFID.h>

/*#define SS_PIN 10
#define RST_PIN 9*/

#define LED_PIN 13

#define SS_PIN    21
#define RST_PIN   22
#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16
#define greenPin     12
#define redPin       32

RFID rfid(SS_PIN, RST_PIN);
int validCard[][5] = {
  {52, 131, 231, 246, 166},
  {145, 20, 209, 36, 112},
  {3,94,69,144,136}
};
int serNum[5];
bool isValid = false;

void setup() {
  Serial.begin(9600);
  SPI.begin();
  rfid.init();
  pinMode(LED_PIN, OUTPUT);
  Serial.println("Ready to scan RFID tag");
}

void loop() {
  if (rfid.isCard()) {
    //Serial.println("Tag detected");
    if (rfid.readCardSerial()) {
      Serial.print(rfid.serNum[0]);  /// read card ID
      Serial.print(" ");
      Serial.print(rfid.serNum[1]);
      Serial.print(" ");
      Serial.print(rfid.serNum[2]);
      Serial.print(" ");
      Serial.print(rfid.serNum[3]);
      Serial.print(" ");
      Serial.print(rfid.serNum[4]);
      Serial.println("");

      

      for (int x = 0; x < sizeof(validCard); x++) {
        for (int i = 0; i < sizeof(rfid.serNum); i++ ) {
          if (rfid.serNum[i] != validCard[x][i]) {
            isValid = false;
            break;
          }
          else {
            isValid = true;
          }
        }
        if (isValid) break;
      }

    }

    if (isValid) {
      Serial.println("LED ON");
      //digitalWrite(LED_PIN, HIGH);
      delay(500);
      Serial.println("LED OFF");
      //digitalWrite(LED_PIN, LOW);
    }
  }
  delay(500);
}
