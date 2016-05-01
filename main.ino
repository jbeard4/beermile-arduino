/**************************************************************************/
/*! 
    This example will attempt to connect to an ISO14443A
    card or tag and retrieve some basic information about it
    that can be used to determine what type of card it is.   
   
    Note that you need the baud rate to be 115200 because we need to print
    out the data and read from the card at the same time!

    To enable debug message, define DEBUG in PN532/PN532_debug.h
    
*/
/**************************************************************************/

#if 0
  #include <SPI.h>
  #include <PN532_SPI.h>
  #include "PN532.h"

  PN532_SPI pn532spi(SPI, 10);
  PN532 nfc(pn532spi);
#elif 0
  #include <PN532_HSU.h>
  #include <PN532.h>
      
  PN532_HSU pn532hsu(Serial1);
  PN532 nfc(pn532hsu);
#else 
  #include <Wire.h>
  #include <PN532_I2C.h>
  #include <PN532.h>
  #include <NfcAdapter.h>
  
  PN532_I2C pn532i2c(Wire);
  PN532 nfc(pn532i2c);
#endif

#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

const int pinLed    = 4;
const int speakerPin = 3;                  // Grove Buzzer connect to D3
const int BREATH_DELAY = 5; // milliseconds

  
void setup(void) {
  Serial.begin(115200);

  pinMode(pinLed, OUTPUT);
  pinMode(speakerPin, OUTPUT);

  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);

  Serial.println("Hello!");
  lcd.setRGB(255, 255, 255);
  lcd.print("Hello!");

  nfc.begin();

  uint32_t versiondata = nfc.getFirmwareVersion();
  if (! versiondata) {
    Serial.print("Didn't find PN53x board");
    lcd.print("Didn't find PN53x board");
    while (1); // halt
  }
  
  // Got ok data, print it out!
  Serial.print("Found chip PN5"); Serial.println((versiondata>>24) & 0xFF, HEX); 
  Serial.print("Firmware ver. "); Serial.print((versiondata>>16) & 0xFF, DEC); 
  Serial.print('.'); Serial.println((versiondata>>8) & 0xFF, DEC);
  
  // Set the max number of retry attempts to read from a card
  // This prevents us from waiting forever for a card, which is
  // the default behaviour of the PN532.
  nfc.setPassiveActivationRetries(0xFF);
  
  // configure board to read RFID tags
  nfc.SAMConfig();
    
  Serial.println("Waiting for an ISO14443A card");
  lcd.print("Waiting for card");
}

void loop(void) {
  boolean success;
  uint8_t uid[] = { 0, 0, 0, 0, 0, 0, 0 };  // Buffer to store the returned UID
  uint8_t uidLength;                        // Length of the UID (4 or 7 bytes depending on ISO14443A card type)
  char c;

  digitalWrite(speakerPin, LOW);

  while(Serial.available()){
    c = Serial.read();
    Serial.println("Received character");
    Serial.println(c);
    lcd.setCursor(0,1);
    switch(c){
      case 'E':
        lcd.setRGB(255, 0, 0);
        lcd.print("Error: fs"); 
        break;
      case 'F':
        lcd.setRGB(255, 0, 0);
        lcd.print("Error: cloud"); 
        break;
      case 'S':
        lcd.print("Saved to file"); 
        lcd.setRGB(0, 0, 255);
        break;
      case 'T':
        lcd.print("Saved to cloud"); 
        lcd.setRGB(0, 255, 0);
        break;
    }
  }
  
  // Wait for an ISO14443A type cards (Mifare, etc.).  When one is found
  // 'uid' will be populated with the UID, and uidLength will indicate
  // if the uid is 4 bytes (Mifare Classic) or 7 bytes (Mifare Ultralight)
  success = nfc.readPassiveTargetID(PN532_MIFARE_ISO14443A, &uid[0], &uidLength);
  
  if (success) {
    Serial.println("Found a card!");
    Serial.print("UID Length: ");Serial.print(uidLength, DEC);Serial.println(" bytes");
    Serial.print("UID Value: ");
    Serial.print(" 0x");
    for (uint8_t i=0; i < uidLength; i++) 
    {
      Serial.print(uid[i], HEX); 
    }
    Serial.println("");

    analogWrite(pinLed, 255);
    // Wait 1 second before continuing
    //delay(1000);

    // Print a message to the LCD.
    lcd.setCursor(0,0);
    for (uint8_t i=0; i < uidLength; i++) 
    {
      lcd.print(uid[i], HEX); 
    }

    digitalWrite(speakerPin, HIGH);
    delay(100);
  }
  else
  {
    // PN532 probably timed out waiting for a card
    Serial.println("Timed out waiting for a card");
    analogWrite(pinLed, 0);
    lcd.clear();
    lcd.setRGB(0, 0, 0);
    lcd.print("Waiting for card");
  }
}

int main(void) {
    init();
    setup();
    for (;;) {
       loop();
    }
}
