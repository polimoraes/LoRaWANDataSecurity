Library Name
LoRaWANDataSecurity

Author 
Poliana de Moraes
poli_moraes@yahoo.com.br

Content
This distribution contains a library and an example application to perform cryptography 
operations on Arduino devices. 

Compatibility
This library is compactible with all the Arduino boards.

Applicability
This library is applicable for Internet of Things end device based on LoRaWAN specification 
versions 1.0 and 1.1.

Purpose
The functionality provided by this library is an proprietary payload for Internet of Things 
end device based on LoRaWAN protocol versions 1.0 and 1.1 that are using third-parties 
network server. The main objetive is protect the data against integrity and confidentiality 
attacks.

Proprietary payload
The input data is encrypted by AES-CTR algorithm following RFC3686 with the last 4 bytes of 
its SHA-256 hash. In order to enable the decryption the 4 last bytes of the IV is sent with 
the payload.

  4 last bytes of IV | 4 last bytes of SHA-256 | 1 to 96 bytes of data to protect


Function
The function buildEncryptedPayload encode an input buffer and writes the proprietary payload
to an output buffer.

Parameters
void buildEncryptedPayload (byte* plaintext, byte* data_payload, byte* key, int plaintext_length)
plaintext - the input buffer to read from with maximum 96 bytes.
data_payload - the buffer to write to the final result. The output buffer must have at least   
	       as many bytes as the input buffer.
key - the 16-bit key to use in cryptography.
int plaintext_length - the number of bytes in input.

Diagnostics
The library checks the size of plaintext and key, and if one of this parameters are out of the 
specification, the output will be a series of zero. In that case, it is possible to connect a 
monitor the arduino and view the error message.

Example
This library provide an example of a arduino firmware that test the proposed algorithm.
The example process an value and compare the output with the theoretical value.
If the value are the same the result is Passed, if there are different the result is Fail.