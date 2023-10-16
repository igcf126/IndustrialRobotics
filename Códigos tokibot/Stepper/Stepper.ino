#include <AccelStepper.h>

#define DIR_PIN 18 
#define STEP_PIN 17
// Change these values to match your motor and setup
const float STEP_ANGLE = 1.8;  // Step angle in degrees
const float GEAR_RATIO = 1; // Gear ratio (if applicable)

// Calculate the number of steps required to move the motor to the desired angle
const float DESIRED_ANGLE = 45.0;
const float STEPS_PER_REV = 360.0 / STEP_ANGLE * GEAR_RATIO;
const float STEPS_TO_ANGLE = STEPS_PER_REV / 360.0;
const int STEPS_TO_MOVE = DESIRED_ANGLE * STEPS_TO_ANGLE;


AccelStepper stepper(AccelStepper::DRIVER, STEP_PIN, DIR_PIN);

void setup() {
  stepper.setMaxSpeed(1000);
  stepper.setAcceleration(500);

  stepper.setCurrentPosition(0);
  stepper.moveTo(0);
  stepper.setSpeed(500);
  while (stepper.run() != 0) {}

}

void loop() {
  // Move the motor to the desired angle
  stepper.moveTo(STEPS_TO_MOVE);
  stepper.setSpeed(500);
  while (stepper.run() != 0) {}
  stepper.moveTo(0);
  stepper.setSpeed(500);
  while (stepper.run() != 0) {}

  // // Move the motor to the desired angle smoothly
  // stepper.moveTo(STEPS_TO_MOVE);
  // stepper.setSpeed(1000);
  //   while (stepper.run() != 0) {}
  // // while (stepper.distanceToGo() != 0) {
  // //   stepper.run();
  // // }
  // stepper.moveTo(0);
  //   while (stepper.run() != 0) {}


}
