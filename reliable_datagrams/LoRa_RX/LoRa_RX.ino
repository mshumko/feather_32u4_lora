// Feather9x_RX
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messaging client (receiver)
// with the RH_RF95 class. RH_RF95 class does not provide for addressing or
// reliability, so you should only use RH_RF95 if you do not need the higher
// level messaging abilities.
// It is designed to work with the other example Feather9x_TX

#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

// for Feather32u4 RFM9x
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
#define LED 13

// Define radio paramaters. This is the server
#define RF95_FREQ 915.0
#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
RHReliableDatagram rf95_manager(rf95, SERVER_ADDRESS);

// Global variables. BAD!
uint8_t data[] = "And hello back to you";
uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];

void setup()
{
  pinMode(LED, OUTPUT);
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  Serial.begin(9600);

  Serial.println("Feather LoRa RX Test!");

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  if (!rf95_manager.init())
    Serial.println("init failed");
  Serial.println("LoRa radio init OK!");

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  Serial.print("Set Freq to: "); Serial.println(RF95_FREQ);
  rf95.setTxPower(23, false);
}

void loop()
{
  if (rf95_manager.available())
  {
    // Wait for a message addressed to us from the client
    uint8_t len = sizeof(buf);
    uint8_t from;
    if (rf95_manager.recvfromAck(buf, &len, &from))
    {
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
      Serial.println((char*)buf);

      // Send a reply back to the originator client
      if (!rf95_manager.sendtoWait(data, sizeof(data), from))
        Serial.println("sendtoWait failed");
    }
  }
}
