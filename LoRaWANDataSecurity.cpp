/* Copyright (C) 2021 Universidade Federal de São Paulo

Permission is hereby granted, free of charge, to any person obtaining a
copy of this software and associated documentation files (the "Software"),
to deal in the Software without restriction, including without limitation
the rights to use, copy, modify, merge, publish, distribute, sublicense,
and/or sell copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included
in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS
OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
DEALINGS IN THE SOFTWARE. */

/* Include default Arduino library */
#include <Arduino.h>
/* Include the header file */
#include "LoRaWANDataSecurity.h"
/* Include library for encryption in application layer */
#include <AES.h>
#include <CTR.h>
#include <SHA256.h>

void buildEncryptedPayload (byte* plaintext, byte* data_payload, byte* key, int plaintext_length) {
  
  /* Define buffers */
  byte hash[32];
  byte encrypt_key[16];
  byte decrypted_data[100];  
  byte encrypted_data[100];
  byte iv[16];

  static long counter = 0;

  /* Construct the cryptography object */
  CTR<AESTiny128> aesctr128;
  /* Constructs a SHA-256 hash object */
  SHA256 hashSHA256;

  /* Clear buffer */
  memset(decrypted_data, 0, sizeof(decrypted_data));
  memset(encrypted_data, 0, sizeof(encrypted_data));
  
  /* Validate plaintext size */
  if (sizeof(plaintext) > 96){
    /* Diagnostic message */
    Serial.println("Data has more than 96 bytes.");
    /* Clear buffer */
    memset(data_payload, 0, sizeof(data_payload));
    return;
  }  

  /* Clear the hash state, removing all sensitive data, and then resets 
  the hash ready for a new hashing process */
  hashSHA256.reset();
  /* Update the hash with more data */
  hashSHA256.update(plaintext, plaintext_length);
  /* Finalize the hashing process and returns the hash */
  hashSHA256.finalize(hash, 32);
 
  /* Copy the last 4 bytes from SHA256 results to decrypoted data buffer */  
  for(int i=0; i < 4; i++){
    decrypted_data [i] = hash [28+i];
  }

  /* Copy data to decrypoted data buffer */
  for(int i=0; i < plaintext_length; i++){
    decrypted_data [i+4] = plaintext [i];
  }

  /* Update iv for cryptopgraphy - counter*/
   if (counter < 0xFFFFFFFF){
      counter++;
      for (int i=0; i < 12; i++){
         iv [i] = 0;
      }
      iv[12] = counter  >> 24;
      iv[13] = counter  >> 16 % 256;
      iv[14] = counter  >> 8 % 256;
      iv[15] = counter  % 256;
    }
    else{
      counter = 0;
    }

  /* Set key in a local variable */
  for (int i = 0; i < 16; i++){
    encrypt_key [i] = key [i];
  }
  
  /* Validate key size */  
  if (sizeof(encrypt_key) != 16){
    /* Diagnostic message */
    Serial.println("Key does not have 16 bytes.");
    /* Clear buffer */
    memset(decrypted_data, 0, sizeof(decrypted_data));
    memset(data_payload, 0, sizeof(data_payload));
    return;
  }
  
  /* Clear object for next cryphtography */
  aesctr128.clear();
  /* Set key for cryptography */
  aesctr128.setKey(encrypt_key, 16);
  /* Set initialization vector */
  aesctr128.setIV(iv, 16);
  /* Calculate the decrypted_data buffer cryptography and storage the result in encrypted_data buffer */
  aesctr128.encrypt(encrypted_data, decrypted_data, 4 + plaintext_length);
  
  /* Build the final payload = iv last 4 bytes + encryption (data + hash last 4 bytes) */
  for(int i = 0; i < 4; i++){
      data_payload [i] = iv [12+i];
  }
  for(int i = 0; i < 4 + plaintext_length; i++){
      data_payload [i+4] = encrypted_data [i];
  }

  return;
}