//http://www.google.de/imgres?imgurl=http%3A%2F%2Fwww.norwegiancreations.com%2Fwp-content%2Fuploads%2F2014%2F12%2FArduino-tutorial-Stepper-Tutorial1.jpg&imgrefurl=http%3A%2F%2Fwww.norwegiancreations.com%2F2014%2F12%2Farduino-tutorial-stepper-motor-with-easydriver%2F&h=1157&w=1524&tbnid=wayeocomBmuSQM%3A&zoom=1&docid=R8DRjTtWG323ZM&ei=DlORVc7sM4KrswG4o4TwCQ&tbm=isch&iact=rc&uact=3&dur=2780&page=1&start=0&ndsp=14&ved=0CDAQrQMwBQ
//http://popovic.info/html/arduino/arduinoUno_1.html
//http://www.phidgets.com/products.php?product_id=3521
//
#define BUTTONPIN 4
#define BUTTONPIN2 5
int counter = 0;
int smDirectionPin = 2; //Direction pin
int smStepPin = 3; //Stepper pin
int smEnablePin = 7; //Motor enable pin
int smSleepPin = 8;
const int irPin = 1; // analoger Eingang für den Abstandssensor
float distance = 100; // Objektentfernung

boolean inaction = false;

boolean waiting_for_input = true;
void setup() {
  /*Sets all pin to output; the microcontroller will send them(the pins) bits, it will not expect to receive any bits from thiese pins.*/
  pinMode(BUTTONPIN, INPUT_PULLUP);
  pinMode(smDirectionPin, OUTPUT);
  pinMode(smStepPin, OUTPUT);
  pinMode(smEnablePin, OUTPUT);
  pinMode(smSleepPin, OUTPUT);
  pinMode(irPin, INPUT); // declare infrared sensor as input

  digitalWrite(smSleepPin, HIGH); //Disables sleep pin
  digitalWrite(smEnablePin, HIGH); //Disbales the motor, so it can rest untill it is called uppond

  Serial.begin(9600);
}

void loop() {


  if (waiting_for_input == true) {
    distance = 4800 * pow(analogRead(irPin), -0.935) - 11; // Achtung: Sensor eichen!
  }
  Serial.println(distance);
  
  /*Here we are calling the rotate function to turn the stepper motor*/
  
  if (distance <= 10) {  //check if the input is HIGH
    waiting_for_input = false;
    rotate(840, 0.11);
    delay(45000);
    while ((digitalRead(BUTTONPIN) == HIGH)) {
      rotate(-8, 0.11);
      //delay(1000);
    } // close while dRead BP
    if (digitalRead(BUTTONPIN) == LOW) {
      waiting_for_input = true;
    } // end of if dRead BP == LOW
  } // end if if distance
} // end of llop

/*The rotate function turns the stepper motor. Tt accepts two arguments: 'steps' and 'speed'*/
void rotate(int steps, float speed) {
  digitalWrite(smSleepPin, HIGH); // Disables sleep pin
  digitalWrite(smEnablePin, LOW); //Enabling the motor, so it will move when asked to

  /*This section looks at the 'steps' argument and stores 'HIGH' in the 'direction' variable if */
  /*'steps' contains a positive number and 'LOW' if it contains a negative.*/
  int direction;

  if (steps > 0) {
    direction = HIGH;
  } else {
    direction = LOW;
  }

  speed = 1 / speed * 70; //Calculating speed
  steps = abs(steps); //Stores the absolute value of the content in 'steps' back into the 'steps' variable

  digitalWrite(smDirectionPin, direction); //Writes the direction (from our if statement above), to the EasyDriver DIR pin

  /*Steppin'*/
  for (int i = 0; i < steps; i++) {
    digitalWrite(smStepPin, HIGH);
    delayMicroseconds(speed);
    digitalWrite(smStepPin, LOW);
    delayMicroseconds(speed);
  }
  digitalWrite(smEnablePin, HIGH); //Disbales the motor, so it can rest untill the next time it is called uppond
  digitalWrite(smSleepPin, LOW); //Enables sleep pin
}
