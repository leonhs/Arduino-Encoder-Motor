/*
    This code uses a rotary encoder to set the speed of a motor.
    If rotated clockwise the motor will slowly increase from 0 to 100%
    If rotated anticlockwise it will count down.
    If the motor speed goes into negativs it would rotate the motor backwards.
    This would act like a car pedel but if you pull up it would go backwards.
    The encoder would quickly ramp the motor speed but if you hold the button it would
    be fine-tunable.

*/



#include "EncoderStepCounter.h"

// Define pins for encoder
#define ENCODER_PIN1 2
#define ENCODER_INT1 digitalPinToInterrupt(ENCODER_PIN1)
#define ENCODER_PIN2 3
#define ENCODER_INT2 digitalPinToInterrupt(ENCODER_PIN2)

// Define button pin
#define but_pin 10

// setup the variables for motor and button
int motor_speed;
int motor_direction;
int but_val;

// Sets the step counter for encoder
EncoderStepCounter encoder(ENCODER_PIN1, ENCODER_PIN2);
void setup() {
  // Begin Serial
  Serial.begin(9600);
  // Start the encoder
  encoder.begin();

  // Sets the pins for the encoder
  attachInterrupt(ENCODER_INT1, interrupt, CHANGE);
  attachInterrupt(ENCODER_INT2, interrupt, CHANGE);

  // Starts the button pin to input from ground
  pinMode(but_pin, INPUT_PULLUP);
}

// Call tick on every change interrupt
void interrupt() {
  encoder.tick();
}
// Sets the position of encoder to start at 0
signed long position = 0;
void loop() {
  // sets the variable but_val to = the button state
  but_val = digitalRead(but_pin);
  // sets the position to either be +1, -1 or 0
  signed char pos = encoder.getPosition();
  // stops the position from going under -80 (motor max speed backwards)
  if ((position < -79) && (pos < 0)) {
    position = -79;
    pos = -1;
  }
  // stops the position from going over 80 (motor max speed)
  if ((position > 79) && (pos > 0)) {
    position = 79;
    pos = 1;
  }
  // stops the position from going under -80 when the button is pressed
  if ((but_val != LOW) && (position < -70) && (pos < 0)) {
    position = -70;
    pos = -1;
  }
  // stops the position from going over 80 when the button is pressed
  if ((but_val != LOW) && (position > 70) && (pos > 0)) {
    position = 70;
    pos = 1;
  }
  // if the position of the encoder has changed set make the
  // encoder position = how many clicks it moved up
  // this goes up or down in tens
  if (pos != 0) {
    if (but_val != LOW) {
      position += (pos * 10);
      encoder.reset();
    }
    // if the position of the encoder has changed and button is pressed
    // set make the encoder position = how many clicks it moved up
    // this goes up or down by one
    if (but_val == LOW) {
      position += pos;
      encoder.reset();
    }
  }


  // if the encoder value is positive
  if (position > 0) {
    // map the value of the encoder into something
    // the motor can read
    motor_speed = map(position, 0, 80, 0, 255);
    // sets the motor to go forwards
    motor_direction = LOW;
  }


  // if the encoder value is negative
  if (position < 0) {
    // map the value of the encoder into something
    // the motor can read
    motor_speed = map(position, 0, -80, 255, 0);
    // sets the motor to go backwards
    motor_direction = HIGH;
  }

  // if the encoder == 0
  if (position == 0) {
    // set the motor to 0
    motor_speed = 0;
    // set direction to forwards
    motor_direction = LOW;
  }

  // neatly prints the data of both the motor and encoder
  // to the serial monitor to be read
  Serial.print(motor_speed);
  Serial.print(" - ");
  Serial.println(position);
  analogWrite(6, motor_speed);
  digitalWrite(5, motor_direction);
  
}
