#include <SPI.h>
#include <RH_RF95.h>

/* for feather32u4*/ 
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7

#define RF95_FREQ 915.0

// Blinky on sent
#define LED 13

//String serial_data = "";
char serial_data[RH_RF95_MAX_MESSAGE_LEN];
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

  // Defaults after init are 434.0MHz, modulation GFSK_Rb250Fd250, +13dbM
  if (!rf95.setFrequency(RF95_FREQ)) {
    Serial.println("setFrequency failed");
    while (1);
  }

  rf95.setTxPower(23, false);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0) // if we get a valid byte, read analog ins:
  { 
    // clear serial_data char array.
    memset(serial_data, 0, RH_RF95_MAX_MESSAGE_LEN);
    
    i = 0; // Counter to get length of transmission.
    while (Serial.available() > 0) 
    {  
      //serial_data += char(Serial.read());
      serial_data[i] = char(Serial.read());
      i++;
      delay(2);
    }
    Serial.print("Sending data: ");
    Serial.print(serial_data);
    // Send data over radio
    rf95.send((char *) serial_data, i);
    //rf95.send(&serial_data, i);
    rf95.waitPacketSent();
  }
}
