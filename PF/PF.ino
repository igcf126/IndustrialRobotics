//#include <AccelStepper.h>
#include <math.h>
#include <stdio.h>
#include <Servo.h>

// Change these values to match your motor and setup
const float STEP_ANGLE = 1.8;  // Step angle in degrees
const float GEAR_RATIO_1 = 1; // Gear ratio (if applicable)
const float GEAR_RATIO_2 = 51; // Gear ratio (if applicable)
const float GEAR_RATIO_3 = 48/20; // Gear ratio (if applicable)


#define DIR_PIN_1 18 
#define STEP_PIN_1 17
#define H1_PIN 25
// Calculate the number of steps required to move the motor to the desired angle
const float DESIRED_ANGLE_1 = -94;
const float STEPS_PER_REV_1 = 360.0 / STEP_ANGLE * GEAR_RATIO_1;
const float STEPS_TO_ANGLE_1 = STEPS_PER_REV_1 / 360.0;
const int STEPS_TO_MOVE_1 = DESIRED_ANGLE_1 * STEPS_TO_ANGLE_1;

#define DIR_PIN_2 13
#define STEP_PIN_2 16
#define H2_PIN 24
// Calculate the number of steps required to move the motor to the desired angle
const float DESIRED_ANGLE_2 = 45.4610;
const float STEPS_PER_REV_2 = 360.0 / STEP_ANGLE * GEAR_RATIO_2;
const float STEPS_TO_ANGLE_2 = STEPS_PER_REV_2 / 360.0;
const int STEPS_TO_MOVE_2 = DESIRED_ANGLE_2 * STEPS_TO_ANGLE_2;

#define DIR_PIN_3 19
#define STEP_PIN_3 20
#define H3_PIN 23
// Calculate the number of steps required to move the motor to the desired angle
const float DESIRED_ANGLE_3 = 67.6083;
const float STEPS_PER_REV_3 = 360.0 / STEP_ANGLE * GEAR_RATIO_3;
const float STEPS_TO_ANGLE_3 = STEPS_PER_REV_3 / 360.0;
const int STEPS_TO_MOVE_3 = DESIRED_ANGLE_3 * STEPS_TO_ANGLE_3;


#define servo_pin 8
Servo gripper;

float points[7][3] = {
  // {-25, 20.6107, 111},
  // {-14, 45.4610, 65.6083},
  // {-33, 45., 65.6083},
  // {-51, 45.4610, 65.6083},
  // {-72.5, 45.4610, 65.6083},
  // {-93, 45.4610, 65.6083},
  {-15, 18, 110},
  {-90, 15, 111},
  {-15, 41, 66},
  {-45, 38, 65},
  {-75, 39, 64},
  {-105, 39, 63},

  {-135, 30, 70}
};

int pasos = 100;

// variables incrementales que guardan la posición en grados del robot.
float pos_cintura = 0;
float pos_hombro = 0;
float pos_codo = 0;

float angCint;
float angHomb;
float angCodo;

bool abrido = true;

void setup() {
  Serial.begin(9600);
  pinMode(H1_PIN, INPUT_PULLUP);
  pinMode(H2_PIN, INPUT_PULLUP);
  pinMode(H3_PIN, INPUT_PULLUP);

  pinMode(STEP_PIN_1, OUTPUT); pinMode(DIR_PIN_1, OUTPUT); // Pines de cintura
  pinMode(STEP_PIN_2, OUTPUT); pinMode(DIR_PIN_2, OUTPUT); // Pines de hombro
  pinMode(STEP_PIN_3, OUTPUT); pinMode(DIR_PIN_3, OUTPUT); // Pines de codo

  gripper.attach(servo_pin);  // attaches the servo on pin 9 to the servo object
  gripper.write(45);

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
      AbreGripper(true);
      Serial.println(DESIRED_ANGLE_1);
      moverCint(DESIRED_ANGLE_1);
      Serial.println(DESIRED_ANGLE_3);
      moverCodo(DESIRED_ANGLE_3);
      Serial.println(DESIRED_ANGLE_2);
      moverHomb(DESIRED_ANGLE_2-2);
    } else if(flag == 'h'){
      homing();
    } else if(flag == 'g'){
      abrido = !abrido;
      AbreGripper(abrido);
    } else if(flag == 'o'){
      AbreGripper(true);
      Serial.println("uno");
      moverCint(points[0][0]+3);
      moverCodo(points[0][2]+3);
      moverHomb(points[0][1]);
      AbreGripper(false);

      moverHomb(points[0][1]-5);     
      //moverCodo(points[0][2]+5);
      moverHomb(0);     
      moverCodo(points[6][2]);
      moverCint(points[6][0]);
      moverHomb(points[6][1]);     
      AbreGripper(true);

      moverCodo(10);
      moverHomb(25);     

      AbreGripper(true);
      Serial.println("uno");
      moverCint(points[5][0]);
      moverCodo(points[5][2]);
      moverHomb(points[5][1]);
      AbreGripper(false);

      moverHomb(points[5][1]);     
      //moverCodo(points[1][2]+5);
      moverHomb(0);     
      moverCodo(points[6][2]);
      moverCint(points[6][0]);
      moverHomb(points[6][1]);     
      AbreGripper(true);



      moverCodo(10);
      moverHomb(25);     

      AbreGripper(true);
      Serial.println("uno");
      moverCint(points[4][0]);
      moverCodo(points[4][2]);
      moverHomb(points[4][1]);
      AbreGripper(false);

      //moverCodo(points[1][2]+5);
      moverHomb(0);     
      moverCodo(points[6][2]);
      moverCint(points[6][0]);
      moverHomb(points[6][1]);     
      AbreGripper(true);



      moverCodo(10);
      moverHomb(25);     

      AbreGripper(true);
      Serial.println("uno");
      moverCint(points[3][0]);
      moverCodo(points[3][2]);
      moverHomb(points[3][1]);
      AbreGripper(false);

      //moverCodo(points[1][2]+5);
      moverHomb(0);     
      moverCodo(points[6][2]);
      moverCint(points[6][0]);
      moverHomb(points[6][1]);     
      AbreGripper(true);



      moverCodo(10);
      moverHomb(25);     

      AbreGripper(true);
      Serial.println("uno");
      moverCint(points[2][0]);
      moverCodo(points[2][2]);
      moverHomb(points[2][1]);
      AbreGripper(false);

      //moverCodo(points[1][2]+5);
      moverHomb(0);     
      moverCodo(points[6][2]);
      moverCint(points[6][0]);
      moverHomb(points[6][1]);     
      AbreGripper(true);



      moverCodo(10);
      moverHomb(25);     

      AbreGripper(true);
      Serial.println("uno");
      moverCint(points[1][0]);
      moverCodo(points[1][2]);
      moverHomb(points[1][1]);
      AbreGripper(false);

      //moverCodo(points[1][2]+5);
      moverHomb(0);     
      moverCodo(points[6][2]);
      moverCint(points[6][0]);
      moverHomb(points[6][1]);     
      AbreGripper(true);


    } else if(flag == 'r'){
      moverCodo(DESIRED_ANGLE_3-5);
      moverHomb(0);
    } else if(flag == 'c'){
      moverCodo(45);
    }

    // moverCodo(ang);
  }

}

void homing(){
  pasos = 10;
  gironegativo(STEP_PIN_1, DIR_PIN_1, 10000);  
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
    giropositivo(STEP_PIN_1, DIR_PIN_1, 10000);
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
       giropositivo(STEP_PIN_1,DIR_PIN_1,4000);
     }else{
      pasos = x;
       gironegativo(STEP_PIN_1,DIR_PIN_1,4000);
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
       giropositivo(STEP_PIN_2,DIR_PIN_2,10000);
     }else{
      pasos = x;
       gironegativo(STEP_PIN_2,DIR_PIN_2,10000);
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

   float x = Codo*STEPS_TO_ANGLE_3*1.125*1.125;
     if (ax == 1){ 
      pasos = x;
       giropositivo(STEP_PIN_3,DIR_PIN_3,10000);
     }else{
      pasos = x;
       gironegativo(STEP_PIN_3,DIR_PIN_3,10000);
     }  
  delay(500); 
}

void PruebaGripper(){
  gripper.write(90);  // moves the servo to the 90 degree position
  delay(1000);        // waits for 1 second
  gripper.write(0);   // moves the servo to the 0 degree position
  delay(1000);        // waits for 1 second
}

void AbreGripper(bool abre){
  if (abre){
    gripper.write(90);  // moves the servo to the 90 degree position
    delay(1000);        // waits for 1 second  
  }else if(!abre){
    gripper.write(0);   // moves the servo to the 0 degree position
    delay(1000);        // waits for 1 second
  }
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