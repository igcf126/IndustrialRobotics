//#include <AccelStepper.h>
#include <math.h>
#include <stdio.h>


// Change these values to match your motor and setup
const float STEP_ANGLE = 1.8;  // Step angle in degrees
const float GEAR_RATIO_1 = 1; // Gear ratio (if applicable)
const float GEAR_RATIO_2 = 51; // Gear ratio (if applicable)
const float GEAR_RATIO_3 = 48/20; // Gear ratio (if applicable)


#define DIR_PIN_1 18 
#define STEP_PIN_1 17
#define H1_PIN 25
// Calculate the number of steps required to move the motor to the desired angle
const float DESIRED_ANGLE_1 = -135;
const float STEPS_PER_REV_1 = 360.0 / STEP_ANGLE * GEAR_RATIO_1;
const float STEPS_TO_ANGLE_1 = STEPS_PER_REV_1 / 360.0;
const int STEPS_TO_MOVE_1 = DESIRED_ANGLE_1 * STEPS_TO_ANGLE_1;

#define DIR_PIN_2 13
#define STEP_PIN_2 16
#define H2_PIN 24
// Calculate the number of steps required to move the motor to the desired angle
const float DESIRED_ANGLE_2 = 21.7133;
const float STEPS_PER_REV_2 = 360.0 / STEP_ANGLE * GEAR_RATIO_2;
const float STEPS_TO_ANGLE_2 = STEPS_PER_REV_2 / 360.0;
const int STEPS_TO_MOVE_2 = DESIRED_ANGLE_2 * STEPS_TO_ANGLE_2;

#define DIR_PIN_3 19
#define STEP_PIN_3 20
#define H3_PIN 23
// Calculate the number of steps required to move the motor to the desired angle
const float DESIRED_ANGLE_3 = 106.7333;
const float STEPS_PER_REV_3 = 360.0 / STEP_ANGLE * GEAR_RATIO_3;
const float STEPS_TO_ANGLE_3 = STEPS_PER_REV_3 / 360.0;
const int STEPS_TO_MOVE_3 = DESIRED_ANGLE_3 * STEPS_TO_ANGLE_3;



int pasos = 100;

// variables incrementales que guardan la posición en grados del robot.
float pos_cintura = 0;
float pos_hombro = 0;
float pos_codo = 0;

float angCint;
float angHomb;
float angCodo;

void setup() {
  Serial.begin(9600);
  pinMode(H1_PIN, INPUT_PULLUP);
  pinMode(H2_PIN, INPUT_PULLUP);
  pinMode(H3_PIN, INPUT_PULLUP);

  pinMode(STEP_PIN_1, OUTPUT); pinMode(DIR_PIN_1, OUTPUT); // Pines de cintura
  pinMode(STEP_PIN_2, OUTPUT); pinMode(DIR_PIN_2, OUTPUT); // Pines de hombro
  pinMode(STEP_PIN_3, OUTPUT); pinMode(DIR_PIN_3, OUTPUT); // Pines de codo

  delay(1000);
  homing();
}

void loop() {
  if (Serial.available() > 0) {
    char flag = Serial.read();
      Serial.available() ==0;
      float jaja = Serial.read();
    Serial.println(flag);
    if (flag == 'p'){
      Serial.println(DESIRED_ANGLE_1);
      moverCint(DESIRED_ANGLE_1);
      Serial.println(DESIRED_ANGLE_2);
      moverHomb(DESIRED_ANGLE_2);
      Serial.println(DESIRED_ANGLE_3);
      moverCodo(DESIRED_ANGLE_3);
    } else if(flag == 'h'){
      
    }
    // moverCodo(ang);
  }

}

void homing(){
  pasos = 10;
  gironegativo(STEP_PIN_1, DIR_PIN_1, 2000);  
  delay(1000);
  
  pasos = 100;
  giropositivo(STEP_PIN_2, DIR_PIN_2, 10000);
  delay(1000);

  pasos = 15; 
  giropositivo(STEP_PIN_3, DIR_PIN_3, 10000);
  delay(1000);

  while(digitalRead(H3_PIN) == HIGH){
    pasos = 5 ;
    gironegativo(STEP_PIN_3, DIR_PIN_3, 10000);
    Serial.println(digitalRead(H3_PIN));
    delay(10);

  }   

  while(digitalRead(H2_PIN) == HIGH){
    pasos = 10 ;
    gironegativo(STEP_PIN_2, DIR_PIN_2, 10000);
    Serial.println(digitalRead(H2_PIN));
    delay(10);
  }  

  while(digitalRead(H1_PIN) == HIGH){
    pasos = 5 ;
    giropositivo(STEP_PIN_1, DIR_PIN_1, 2000);
    Serial.println(digitalRead(H1_PIN));
    delay(100);
  }   

pos_cintura = 0;
pos_hombro = 0;
pos_codo = 0;

}

void gironegativo(int paso_,int dire_,int retar) {
   digitalWrite(dire_, HIGH);   // direccion de giro 1
   for(int i=0;i<pasos;i++){  // da  pasos por un tiempo  
    digitalWrite(paso_, LOW);      
    delayMicroseconds(retar);          
    digitalWrite(paso_, HIGH);       
    delayMicroseconds(retar); 
   }
}


void giropositivo(int paso_,int dire_,int retar) {
  digitalWrite(dire_,LOW);   // direccion de giro 2
  for(int i=0;i<pasos;i++){   // da  pasos por un tiempo  
    digitalWrite(paso_, HIGH);      
    delayMicroseconds(retar);          
    digitalWrite(paso_, LOW);       
    delayMicroseconds(retar);  
  }
}


void moverCint(float Cint){
   int ax = 0;
   
   if (Cint > pos_cintura){
      Cint = Cint - pos_cintura;
      pos_cintura = pos_cintura + Cint;
      ax = 1;
   }else{ 
      Cint = pos_cintura - Cint;
      pos_cintura = pos_cintura - Cint;
   }   

    if (Cint < 0){
      Cint = Cint*-1;
      }

   /*
   if(ax == 1){
     digitalWrite(x_dire,HIGH);
   }else{
     digitalWrite(x_dire,LOW);
   }
   */ 

   float x = Cint*4*1.125*1.125;
     if (ax == 1){ 
      pasos = x;
       giropositivo(STEP_PIN_1,DIR_PIN_1,2000);
     }else{
      pasos = x;
       gironegativo(STEP_PIN_1,DIR_PIN_1,2000);
     }  
  delay(500); 
}


void moverHomb(float Homb){
   int ax = 0;
   
   if (Homb > pos_hombro){
      Homb = Homb - pos_hombro;
      pos_hombro = pos_hombro + Homb;
      ax = 1;
   }else{ 
      Homb = pos_hombro - Homb;
      pos_hombro = pos_hombro - Homb;
   }   

    if (Homb < 0){
      Homb = Homb*-1;
      }

   /*
   if(ax == 1){
     digitalWrite(x_dire,HIGH);
   }else{
     digitalWrite(x_dire,LOW);
   }
   */ 

   float x = Homb*STEPS_TO_ANGLE_2;
     if (ax == 1){ 
      pasos = x;
       giropositivo(STEP_PIN_2,DIR_PIN_2,1000);
     }else{
      pasos = x;
       gironegativo(STEP_PIN_2,DIR_PIN_2,1000);
     }  
  delay(500); 
}


void moverCodo(float Codo){
   int ax = 0;
   
   if (Codo > pos_codo){
      Codo = Codo - pos_codo;
      pos_codo = pos_codo + Codo;
      ax = 1;
   }else{ 
      Codo = pos_codo - Codo;
      pos_codo = pos_codo - Codo;
   }   

    if (Codo < 0){
      Codo = Codo*-1;
      }

   /*
   if(ax == 1){
     digitalWrite(x_dire,HIGH);
   }else{
     digitalWrite(x_dire,LOW);
   }
   */ 

   float x = Codo*STEPS_TO_ANGLE_3;
     if (ax == 1){ 
      pasos = x;
       giropositivo(STEP_PIN_3,DIR_PIN_3,10000);
     }else{
      pasos = x;
       gironegativo(STEP_PIN_3,DIR_PIN_3,10000);
     }  
  delay(500); 
}

// float IK_q1(float px, float py, float pz){
//   angCint = atan2(-py, -px);
//   return angCint;
// }

// float IK_q2(float px, float py, float pz){

//   return angHomb;
// }

// float IK_q3(float px, float py, float pz){

//   return angCodo;
// }