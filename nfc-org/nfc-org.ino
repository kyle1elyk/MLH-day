//Kyle Stead 
//Hunter

#include "SPI.h"
#include "MFRC522.h"
#include <math.h>



#define SS_PIN 10
#define RST_PIN 9
#define SP_PIN 8

#define RLED 4
#define GLED 3
#define BLED 2

#define ABUT 6
#define CBUT 7

#define BLINK_DUR 500
#define WAIT_DUR 50


MFRC522 rfid(SS_PIN, RST_PIN);

MFRC522::MIFARE_Key key;
bool button_pressed;

void setup()
{
    randomSeed(analogRead(0));
    pinMode(RLED, OUTPUT);
    pinMode(GLED, OUTPUT);
    pinMode(BLED, OUTPUT);
    
    pinMode(ABUT, INPUT);
    pinMode(CBUT, INPUT);
    
    Serial.begin(9600);
    
    
    SPI.begin();
    rfid.PCD_Init();
}

void loop()
{
  int abut = digitalRead(ABUT);
  if (abut == LOW) button_pressed = false;
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return;

  MFRC522::PICC_Type piccType = rfid.PICC_GetType(rfid.uid.sak);
  Serial.println(rfid.PICC_GetTypeName(piccType));
  
  // Build ID of Tag
  String strID = "";
  for (byte i = 0; i < 4; i++)
  {
    strID +=
    (rfid.uid.uidByte[i] < 0x10 ? "0" : "") +
    String(rfid.uid.uidByte[i], HEX) +
    (i!=3 ? ":" : "");
  }
  strID.toUpperCase();
  Serial.print("Tap card key: ");
  Serial.println(strID);
  
  if (abut == HIGH)
  {
     if (button_pressed) {
      digitalWrite(BLED,HIGH);
      delay(BLINK_DUR);
      digitalWrite(BLED,LOW);
     }
     else
     {
       digitalWrite(GLED,HIGH);
       delay(BLINK_DUR);
       digitalWrite(GLED,LOW);
     }
  }
  else if(digitalRead(CBUT))
  {
     for (int i = 0; i < 2; i++)
     {
      digitalWrite(RLED,HIGH);
      delay(WAIT_DUR);
      digitalWrite(RLED,LOW);
      delay(WAIT_DUR);
     }
  }
  else
  {
      if (strID.equals("E0:23:17:1B"))
      {
        int temp = random(0,81);
        Serial.println(temp);
        blinkPattern(temp);
        
      }
      else if(strID.equals("04:8C:3D:1A"))
      {
        digitalWrite(BLED,HIGH);
        delay(BLINK_DUR);
        digitalWrite(BLED,LOW);
      }
      else
      {
        digitalWrite(RLED,HIGH);
        delay(BLINK_DUR);
        digitalWrite(RLED,LOW);
      }
  }
  if (abut == HIGH)
  {
    button_pressed = true;
  }
  else
  {
    button_pressed = false;
  }
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void blinkPattern(int patternCode)
{
    // patternCode is a base-3 number representing 4 values
    // 0:R; 1:G; 2:B
    for (int i = 0; i < 4; i++)
    {
        
        float color = (fmod((patternCode / (pow(3, i))), 3));
        Serial.print(" ");
        if (closeEnough(color,2.0))
        {
          Serial.print("2");
          blinkCol(2, BLINK_DUR);
        }
        else if (closeEnough(color,1.0))
        {
          Serial.print("1");
          blinkCol(1, BLINK_DUR);
        }
        else
        {
          Serial.print("0");
          blinkCol(0, BLINK_DUR);
        }
        delay(WAIT_DUR);
    }
    Serial.println();
    
}

void blinkCol(int col, int dur)
{
  int led = RLED;
  if (col == 2) led = BLED;
  if (col == 1) led = GLED;
  digitalWrite(led, HIGH);
  delay(dur);
  digitalWrite(led, LOW);
  
}
bool closeEnough(float a, float b)
{
  return abs(a - b) < 0.0001 || floor(a) == b;
}

