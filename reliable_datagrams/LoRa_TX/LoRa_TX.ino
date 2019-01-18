// Feather9x_TX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (transmitter)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_RX

#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

// Define feather32u4 pins
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
#define LED 13

// Define radio paramaters. This is the client
#define RF95_FREQ 915.0
#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
RHReliableDatagram rf95_manager(rf95, CLIENT_ADDRESS);

// Global variables. BAD!
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

void setup() 
{
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);

  Serial.println("Feather LoRa TX (client) reliable datagram Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95_manager.init())
    Serial.println("init failed");

//  while (!rf95.init()) {
//    Serial.println("LoRa radio init failed");
//    while (1);
//  }
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);
}

int16_t packetnum = 0;  // packet counter, we increment per xmission

void loop()
{
  Serial.println("Sending to rf95_reliable_datagram_server");
  delay(500); // Wait 1 second between transmits, could also 'sleep' here!
  digitalWrite(LED, LOW); // Turn off LED.
  delay(500); 
  
  char radiopacket[20] = "Hello World #      ";
  itoa(packetnum++, radiopacket+13, 10);
  Serial.print("Sending "); Serial.println(radiopacket);
  // Not sure why this is here, but it only shows up in the reciever.
  //radiopacket[19] = 0; 

  if (rf95_manager.sendtoWait(radiopacket, sizeof(radiopacket), SERVER_ADDRESS))
  {
    // Now wait for a reply from the server
    uint8_t len = sizeof(buf);
    uint8_t from;   
    if (rf95_manager.recvfromAckTimeout(buf, &len, 2000, &from))
    {
      digitalWrite(LED, HIGH);
      Serial.print("got reply from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);
    }
    else
    {
      Serial.println("No reply, is rf95_reliable_datagram_server running?");
    }
  }
  else
    Serial.println("sendtoWait failed");
  delay(500);
}
