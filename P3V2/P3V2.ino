#include <Servo.h>
Servo myservo;

const int trigPin = 3; // Ultrasonic sensor trigger pin
const int echoPin = 4; // Ultrasonic sensor echo pin
const int servoPin = 2; // Servo control pin
//const int minAngle = 0; // Minimum servo angle
//const int maxAngle = 270; // Maximum servo angle

int minDist = 10; // Minimum distance
int maxDist = 150; // Maximum distance
int DistanceFiltree = 0; // Maximum distance
//const int targetDist = 30; // Target distance in cm

long duration;


const int numSamples = 75; // Number of samples for moving average
int samples[numSamples]; // Array to store most recent sensor readings
int sampleIndex = 0; // Index of the most recent sample
int cant = 0;

// Conocimientos de controladores
// const int preescalador = 64; // preescalador de 64 para el reloj interno de 16 MHz
// const int frec = 50; // frecuencia deseada

void setup() {
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(servoPin, OUTPUT);
  Serial.begin(9600);

  // TCCR1A = 0;              // Set Timer1 to Normal mode
  // TCCR1B = 0;              // Clear TCCR1B register
  // TCCR1B |= (1 << CS11);   // Set prescaler to 8
  // OCR1A = 39999;           // Set TOP value for Timer1 (16MHz/8
  myservo.attach(2); // Attach servo to pin 2


}

void loop() {
  // Send ultrasonic pulse and calculate distance
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);
  float dist = duration / 58;
  

  // Filtrado de mediciones
  samples[sampleIndex] = dist;
  
  // Calculate moving average of the most recent samples
  int sum = 0;
  for (int i = 0; i < numSamples; i++) {
    sum += samples[i];
  }
  DistanceFiltree = sum / numSamples;

  sampleIndex = (sampleIndex + 1) % numSamples;
  cant = (cant + 1) % 100;


  // Saturation filter

  if (DistanceFiltree > maxDist) {
    DistanceFiltree = maxDist;
  }
  else if (DistanceFiltree < minDist) {
    DistanceFiltree = minDist;
  }
  else {
    DistanceFiltree = DistanceFiltree;
  }



  int dutyCycle = map(DistanceFiltree, minDist, maxDist, 0, 180); 
  myservo.write(dutyCycle);

  // Serial.println("\n");
  // Serial.println(dutyCycle);
  Serial.println(DistanceFiltree);
  delay(7);

  // Interrupción serial
  if (Serial.available() > 0) {
    // Read the incoming character
    char incomingByte = Serial.read();

    // Check if the incoming character is 'a'
    if (incomingByte == 'a') {
      // Call your function here
      Arrete();
    }
  }

}


void Arrete(){
  int jaja = Serial.parseInt();

  Serial.println("Jondea la distancia menor: ");
  // char jaja = Serial.read();
  Serial.println(Serial.available());
  while(Serial.available()==0) { }
  // delay(1000);  
  minDist = Serial.parseInt();
  delay(100);  


  Serial.println("Ahora la mayor miop: ");
  // jaja = Serial.read();
  jaja = Serial.parseInt();

  Serial.println(Serial.available());
  while(Serial.available()==0) { }
  maxDist = Serial.parseInt();

  if (maxDist < minDist){
    int taiguel = minDist;
    minDist = maxDist;
    maxDist = taiguel;
  } 
}