#include <SPI.h>
#include <RH_RF95.h>

/* for feather32u4*/ 
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7

#define RF95_FREQ 915.0

// Blinky on sent
#define LED 13

char buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t i;

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  // put your setup code here, to run once:
  pinMode(RFM95_RST, OUTPUT);
  digitalWrite(RFM95_RST, HIGH);

  // manual reset
  digitalWrite(RFM95_RST, LOW);
  delay(10);
  digitalWrite(RFM95_RST, HIGH);
  delay(10);

  while (!rf95.init()) {
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
    rf95.send((char *) buf, i); // Send data over radio
    rf95.waitPacketSent(); // Wait to finish transmission
  }
  
  //////////////////////////////
  ////////// RECIEVER  /////////
  //////////////////////////////
  // Listen to the radio and if data is recieved, send it to serial.
  if (rf95.available())
  {
    //char buf[RH_RF95_MAX_MESSAGE_LEN];
    memset(buf, 0, RH_RF95_MAX_MESSAGE_LEN);
    uint8_t len = sizeof(buf);
    // Should be a message for us now    
    if (rf95.recv(buf, &len))
    {
      //RH_RF95::printBuffer("\nReceived (for diagnostic use): ", buf, len);
      Serial.print("Recieved: ");
      for(int i = 0; i < len; i++)
      {
        Serial.print(buf[i]);
      }
      
      //Serial.print("RSSI: ");
      //Serial.println(rf95.lastRssi(), DEC);
    }
  }
}
