#include <Servo.h>

#define servo_pin 8
Servo gripper;


void setup() {
  Serial.begin(9600); 
  gripper.attach(servo_pin);  // attaches the servo on pin 9 to the servo object
  gripper.write(45);
}

void loop() {
    if (Serial.available() > 0) {
    char flag = Serial.read();
      Serial.available() ==0;
      float jaja = Serial.read();
    Serial.println(flag);
    if (flag == 'g'){

      gripper.write(90);  // moves the servo to the 90 degree position
      delay(1000);        // waits for 1 second
      gripper.write(0);   // moves the servo to the 0 degree position
      delay(1000);        // waits for 1 second
    }
  }
}
