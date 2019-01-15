#include <SPI.h>
#include <RH_RF95.h>

// Radio specific pins and radio frequency
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
#define RF95_FREQ 915.0

#define LED 5

uint8_t outputValue = 0;//variable to store the output value
// Previous value of the pot. Used to look for a change in pot as 
// times when to print to serial.

uint8_t buf[RH_RF95_MAX_MESSAGE_LEN];
uint8_t len = sizeof(buf);

// Singleton instance of the radio driver
RH_RF95 rf95(RFM95_CS, RFM95_INT);

void setup() {
  // Radio initialization
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

  rf95.setTxPower(23, false);

  // pot_led initialization
  Serial.begin(9600); // start serial port at 9600 bps:
  pinMode(LED, OUTPUT);
}

void loop() {
  if (rf95.available())
  {
    // Should be a message for us now    
    if (rf95.recv(buf, &len))
    {
      RH_RF95::printBuffer("\nReceived (for diagnostic use): ", buf, len);
      Serial.println(*buf, DEC);
      Serial.print("RSSI: ");
      Serial.println(rf95.lastRssi(), DEC);
      analogWrite(LED, buf[0]);
    }
  }
}
