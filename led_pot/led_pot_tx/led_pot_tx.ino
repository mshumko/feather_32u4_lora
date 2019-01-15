#include <SPI.h>
#include <RH_RF95.h>

// Radio specific pins and radio frequency
#define RFM95_CS 8
#define RFM95_RST 4
#define RFM95_INT 7
#define RF95_FREQ 915.0

// For the LED code.
#define LED 5
#define ANALOG_PIN A0

int inputValue = 0;//variable to store the value coming from sensor
uint8_t outputValue = 0;//variable to store the output value
// Previous value of the pot. Used to look for a change in pot as 
// times when to print to serial.
int previous_value = -999; 

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
  pinMode(ANALOG_PIN, INPUT);
  pinMode(LED, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  inputValue = analogRead(ANALOG_PIN);//read the value from the sensor
  //Convert from 0-1023 proportional to the number of a number of from 0 to 255
  outputValue = map(inputValue,0,1023,0,255);

  // Check if the pot value has changed.
  if (abs(outputValue - previous_value) > 10)
  {
    previous_value = outputValue;
    Serial.print("pot value: ");
    Serial.println(outputValue);
    //turn the led on depend on the output value
    analogWrite(LED,outputValue);

    // Send output value over radio 
    rf95.send(&outputValue, sizeof(outputValue));
    rf95.waitPacketSent();
  }  
  delay(100);
}
