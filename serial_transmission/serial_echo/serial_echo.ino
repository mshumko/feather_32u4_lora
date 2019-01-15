// This script will echo LAFTR data sent over the serial port.

String serial_data = "";

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); // start serial port at 9600 bps:
  Serial.println("Arduino ready.");
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
  }
}
