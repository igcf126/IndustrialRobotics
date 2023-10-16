// Define the pins for the encoder
#define encoderPinA 2
#define encoderPinB 3

// Define the pins for the stepper motor
#define in1 8
#define in2 9
#define in3 10
#define in4 11

bool CW = false;
int stepSign = 0;
int ratio = 32;

// Define a variable to keep track of the encoder state
volatile int encoderSteps = 0; // current position of the encoder
volatile int lastEncoded = 0; // previous state of the encoder

// Define the number of steps per revolution for the stepper motor
const int STEPS_PER_REV = 200;

int pasitoDado = 0;
int OtroDicoDeOmega = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Set the encoder pins as input
  pinMode(encoderPinA, INPUT_PULLUP);
  pinMode(encoderPinB, INPUT_PULLUP);

  // Enable pull-up resistors for the encoder pins
  digitalWrite(encoderPinA, HIGH);
  digitalWrite(encoderPinB, HIGH);

  // Attach an interrupt to the encoder pin A
  attachInterrupt(digitalPinToInterrupt(encoderPinA), encoderISR, CHANGE);
  attachInterrupt(digitalPinToInterrupt(encoderPinB), encoderISR, CHANGE);

  // Set the stepper motor pins as output
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
}

void loop() {
  // Nothing to do here
  int dif = OtroDicoDeOmega - pasitoDado;    
  Serial.println(dif);

  if (dif>0) {
    HeartBeat();
    CW = true;
  } else if (dif<0) {
    HeartBeat();
    CW = false;
  } else {
    //Serial.print("ya papito\n");
  }
}

void HeartBeat(){
  static int stepCount = 0;
  switch (stepCount) {
    case 0:
      digitalWrite(in1, HIGH);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      break;

    case 1:
      digitalWrite(in1, LOW);
      digitalWrite(in2, HIGH);
      digitalWrite(in3, LOW);
      digitalWrite(in4, LOW);
      break;

    case 2:
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, HIGH);
      digitalWrite(in4, LOW);
      break;

    case 3:
      digitalWrite(in1, LOW);
      digitalWrite(in2, LOW);
      digitalWrite(in3, LOW);
      digitalWrite(in4, HIGH);
      break;

  }
  if (CW) {
      stepCount++;
      pasitoDado++;
  } else if (!CW) {
      stepCount--;
      pasitoDado--;
  }

  if (stepCount >=4) {
    stepCount = 0;
  } else if (stepCount < 0) {
    stepCount = 3;
  }
}

void encoderISR() {
  // Read the state of the encoder pins and update the encoder steps
  int MSB = digitalRead(encoderPinA);
  int LSB = digitalRead(encoderPinB);

  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;

  if (sum == 0b1101 || sum == 0b0100 || sum == 0b0010 || sum == 0b1011) {
    encoderSteps--;
    OtroDicoDeOmega += (-1*ratio);
  }
  if (sum == 0b1110 || sum == 0b0111 || sum == 0b0001 || sum == 0b1000) {
    encoderSteps++;
    OtroDicoDeOmega += (1*ratio);
  }

  lastEncoded = encoded;
}

