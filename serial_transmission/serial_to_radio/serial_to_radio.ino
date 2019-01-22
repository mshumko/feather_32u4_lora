#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

/* for feather32u4*/ 
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7

#define RF95_FREQ 915.0
#define CLIENT_ADDRESS 1
#define SERVER_ADDRESS 2

// Blinky on sent
#define LED 13

char buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t i;
uint8_t len = sizeof(buf);
uint8_t from;

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);
RHReliableDatagram rf95_manager(rf95, SERVER_ADDRESS);

uint8_t data[] = "Got it!";

void setup() {
  // put your setup code here, to run once:
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95_manager.init()) {
    Serial.println("LoRa radio init failed");
    while (1);
  }

  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }
  // Set output power. For feather valid values between 5 and 23
  rf95.setTxPower(5, false);
  
  Serial.begin(9600);
}

// For reference, RH_RF95_MAX_MESSAGE_LEN = 251 bytes.
void loop() {
  //////////////////////////////
  ///////// TRANSMITTER  ///////
  //////////////////////////////
  // Listen to the serial port and if data shows up, send it to radio.  
  if(Serial.available() > 0)
  {     
    // clear buf char array.
    memset(buf, 0, RH_RF95_MAX_MESSAGE_LEN);
    
    i = 0; // Counter to calculate the length of the transmission.
    // While serial data is avaliable, add bytes to the buffer.
    while (Serial.available() > 0) 
    {  
      buf[i] = char(Serial.read());
      i++;
      //delay(2);
    }
    Serial.print("Sending data: ");
    Serial.print(buf);

    if (rf95_manager.sendtoWait(buf, i, SERVER_ADDRESS))
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
    
    //rf95.send((char *) buf, i); // Send data over radio
    //rf95.waitPacketSent(); // Wait to finish transmission
  }
  
  //////////////////////////////
  ////////// RECIEVER  /////////
  //////////////////////////////
  // Listen to the radio and if data is recieved, send it to serial.
  if (rf95_manager.available())
  {
    //char buf[RH_RF95_MAX_MESSAGE_LEN];
    memset(buf, 0, RH_RF95_MAX_MESSAGE_LEN);
    uint8_t len = sizeof(buf);
    // Should be a message for us now    
    if (rf95_manager.recvfromAck(buf, &len, &from))
    {
      //RH_RF95::printBuffer("\nReceived (for diagnostic use): ", buf, len);
      Serial.print("got request from : 0x");
      Serial.print(from, HEX);
      Serial.print(": ");
//      Serial.println((char*)buf);
      
      //Serial.print("Recieved: ");
      for(int i = 0; i < len; i++) Serial.print(buf[i]);
      //{
        
      //}
      
      //Serial.print("RSSI: ");
      //Serial.println(rf95.lastRssi(), DEC);
      Serial.print("Sending: ");
      Serial.println((char*)data);
      if (!rf95_manager.sendtoWait(data, sizeof(data), from))
        Serial.println("sendtoWait failed");
    }
  }
}
