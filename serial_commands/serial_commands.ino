
#define LED 13

String serial_data = "";

void setup() {
  // put your setup code here, to run once:
  pinMode(LED, OUTPUT); //initialize the LED as output
  Serial.begin(9600); // start serial port at 9600 bps:
  Serial.println("Ready to turn LED on/off.");
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available() > 0) // if we get a valid byte, read analog ins:
  { 
    serial_data = "";
    while (Serial.available() > 0) 
    {  
      serial_data += char(Serial.read());
      delay(2);
    }
    Serial.print(serial_data);
  if(serial_data == "on\n")
  {
    digitalWrite(LED, HIGH);//turn the led on
  }
  else if(serial_data == "off\n")
  {
    digitalWrite(LED, LOW);//turn the led off
  }
  }
}
