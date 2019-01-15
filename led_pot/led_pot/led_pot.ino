#define LED 5
#define ANALOG_PIN A0

int inputValue = 0;//variable to store the value coming from sensor
int outputValue = 0;//variable to store the output value
// Previous value of the pot. Used to look for a change in pot as 
// times when to print to serial.
int previous_value = -999; 

void setup() {
  // put your setup code here, to run once:
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
  }  
  delay(100);
}
