/*
This example runs tests on the LoRaWANDataSecurity implementation to verify correct behaviour.
*/

/* Include the library that was created */
#include <LoRaWANDataSecurity.h>

void setup() {
  /* Set up serial communication data rate */
  Serial.begin (9600);
  Serial.println("Behaviour Test:");
  /* Define encryption key */
  String plaintext = "25";
  byte encrypted [10] = {0x00, 0x00, 0x00, 0x01, 0x26, 0x2b, 0x9c, 0x24, 0x56, 0x23};
  byte LoRaWANData [10];
  byte key[16] = {0xAE, 0x68, 0x52, 0xF8, 0x12, 0x10, 0x67, 0xCC,0x4B, 0xF7, 0xA5, 0x76, 0x55, 0x77, 0xF3, 0x9E};

  memset(LoRaWANData, 0, sizeof(LoRaWANData));

  /* Call function for building payload with the sensor value encrypted by AES-CTR */
  buildEncryptedPayload (plaintext, LoRaWANData, key); 

  if (memcmp(LoRaWANData, encrypted, 10) == 0)
    Serial.println("Passed");
  else
    Serial.println("Failed");    
  }

void loop() { 

}
