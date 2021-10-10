#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>


PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

void setup(void) {
    Serial.begin(115200);
    Serial.println("NDEF Formatter");

    nfc.begin();
}

void loop(void) {
    
    Serial.println("\nPlace an unformatted Mifare Classic tag on the reader.");
    if (nfc.tagPresent()) {

        bool success = nfc.format();
        if (success) {
          Serial.println("\nSuccess, tag formatted as NDEF.");
        } else {
          Serial.println("\nFormat failed.");
        }

    }
    

    for (int i=0; i <= 500; i++){
          delay(10);
    }

  //  delay(5000);
}
