#include <SPI.h>
#include <RH_RF95.h>
#include <RHReliableDatagram.h>

// Configure feather32u4 pins to talk to LoRa and blink LED
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
#define LED 13

//////////////////////////////
//////// RADIO CONFIG  ///////
//////////////////////////////
#define RF95_FREQ 915.0
#define CLIENT_ADDRESS NULL //1
#define SERVER_ADDRESS NULL //2
#define MAX_RETRIES 10

/*Modem config options
To access these enumerators, use a RH_RF95 scope resolution
Bw125Cr45Sf128:   Bw = 125 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. 
                  Default medium range.
Bw500Cr45Sf128:   Bw = 500 kHz, Cr = 4/5, Sf = 128chips/symbol, CRC on. 
                  Fast+short range.
Bw31_25Cr48Sf512: Bw = 31.25 kHz, Cr = 4/8, Sf = 512chips/symbol, CRC on. 
                  Slow+long range.
Bw125Cr48Sf4096:  Bw = 125 kHz, Cr = 4/8, Sf = 4096chips/symbol, CRC on. 
                  Slow+long range
*/
#define MODEM_CONFIG RH_RF95::Bw500Cr45Sf128 


char buf[RH_RF95_MAX_MESSAGE_LEN]; // RH_RF95_MAX_MESSAGE_LEN = 251 bytes.
uint8_t i;
uint8_t len = sizeof(buf);
uint8_t from;

const bool verbose = false;
unsigned long start_time;
const uint8_t led_timeout = 5; // Seconds
const uint8_t radio_timeout = 2; // Seconds

// Function prototypes.
void serial2radio();
void radio2serial();

// Instance of the radio driver and manager
RH_RF95 rf95(RFM95_CS, RFM95_INT);
RHReliableDatagram rf95_manager(rf95, SERVER_ADDRESS);

uint8_t ack[] = "Message recieved\n";

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

  rf95_manager.setRetries(MAX_RETRIES);
  rf95.setModemConfig(MODEM_CONFIG);
  
  Serial.begin(9600);
}

void loop() {
  serial2radio();
  radio2serial();
}

void serial2radio() 
{
  //////////////////////////////
  ///////// TRANSMITTER  ///////
  //////////////////////////////
  // Listen to the serial port and if data shows up, send it to radio.  
  if(Serial.available() > 0)
  {     
    // clear buf char array.
    memset(buf, 0, RH_RF95_MAX_MESSAGE_LEN);
    // Start counter to turn off LED after led_timeout
    start_time = millis();
    
    i = 0; // Counter to calculate the length of the transmission.
    // While serial data is avaliable, add bytes to the buffer.
    while (Serial.available() > 0) 
    {  
      buf[i] = char(Serial.read());
      i++;
    }

    if (verbose)
    {
      Serial.print("Sending data: ");
      Serial.print(buf);
    }

    // Send data and wait for an acknowledgement message.
    if (rf95_manager.sendtoWait(buf, i, SERVER_ADDRESS))
    {
      // Now wait for a reply from the server
      uint8_t len = sizeof(buf);
      uint8_t from;   
      // If reply was heard.
      if (rf95_manager.recvfromAckTimeout(buf, &len, 1000*radio_timeout, &from))
      {
        digitalWrite(LED, HIGH);
        Serial.println((char*)buf);

        if (verbose)
        {
          Serial.print("got reply from : 0x");
          Serial.print(from, HEX);
          Serial.print(": ");
        } 

      }
      else {Serial.println("No reply, is the other unit running?");}
    }
    else {Serial.println("sendtoWait failed");}
  }
}

void radio2serial() 
{
  //////////////////////////////
  ////////// RECIEVER  /////////
  //////////////////////////////
  // Listen to the radio and if data is recieved, send it to serial.
  if (rf95_manager.available())
  {
    start_time = millis();
    memset(buf, 0, RH_RF95_MAX_MESSAGE_LEN);
    // Should be a message for us now    
    if (rf95_manager.recvfromAck(buf, &len, &from))
    {
      digitalWrite(LED, HIGH);      
      for(int i = 0; i < len; i++) {Serial.print(buf[i]);}

      if (verbose) 
      {
        Serial.print("RSSI: ");
        Serial.println(rf95.lastRssi(), DEC);
      }

      // Try to send an recieve acknowledgement message.
      if (!rf95_manager.sendtoWait(ack, sizeof(ack), from))
        Serial.println("sendtoWait failed");
    }
  }
  
  // Turn off LED if no transmission heard in timeout
  if (millis() - start_time > 1000*led_timeout) 
  {
    digitalWrite(LED, LOW);
  }
}
