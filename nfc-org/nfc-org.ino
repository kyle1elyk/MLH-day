//Kyle Stead 
//Hunter

#include "SPI.h"
#include "MFRC522.h"

#define SS_PIN 10
#define RST_PIN 9
#define SP_PIN 8

#define RLED 4
#define GLED 3
#define BLED 2


MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;

void setup() {
    pinMode(RLED, OUTPUT);
    pinMode(GLED, OUTPUT);
    pinMode(BLED, OUTPUT);
    
    Serial.begin(9600);
    SPI.begin();
    rfid.PCD_Init();
}

void loop() {
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  // Serial.print(F("PICC type: "));
  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  // Serial.println(rfid.PICC_GetTypeName(piccType));

  // Check is the PICC of Classic MIFARE type
  if (piccType != MFRC522::PICC_TYPE_MIFARE_MINI &&
    piccType != MFRC522::PICC_TYPE_MIFARE_1K &&
    piccType != MFRC522::PICC_TYPE_MIFARE_4K) {
    Serial.println(F("Your tag is not of type MIFARE Classic."));
    //return;
  }

  String strID = "";
  for (byte i = 0; i < 4; i++) {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();
  
  Serial.print("Tap card key: ");
  Serial.println(strID);

  if (strID.equals("E0:23:17:1B")) {
    digitalWrite(GLED,HIGH);
    digitalWrite(BLED,LOW);
    digitalWrite(RLED,LOW);
    
  } else if(strID.equals("04:8C:3D:1A")){
      digitalWrite(BLED,HIGH);
      digitalWrite(GLED,LOW);
    digitalWrite(RLED,LOW);
  }else {
    digitalWrite(GLED,LOW);
    digitalWrite(RLED,HIGH);
    digitalWrite(BLED,LOW);
  }
  
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}