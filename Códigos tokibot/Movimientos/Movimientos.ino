// alto: h=25, r=32 a 33, teta = -15, -45, -75 // q1 = teta, q2 = 54.3673, q3 = 27.6972 // 64.2249, 12.8386
// medio: h=14, r=32 a 33, teta = variable // q1 = teta, q2 = 45.4610, q3 = 65.6083
// bajo: h=14, r=32 a 33, teta = variable // q1 = teta, q2 = 20.6107, q3 = 111

//#include <AccelStepper.h>
#include <math.h>
#include <stdio.h>
#include <Servo.h>

///////////////////// Sobre los steppers /////////////////////
// Change these values to match your motor and setup
const float STEP_ANGLE = 1.8;  // Step angle in degrees
const float GEAR_RATIO_1 = 1; // Gear ratio (if applicable)
const float GEAR_RATIO_2 = 51; // Gear ratio (if applicable)
const float GEAR_RATIO_3 = 48/20; // Gear ratio (if applicable)


#define DIR_PIN_1 18 
#define STEP_PIN_1 17
#define H1_PIN 25
// Calculate the number of steps required to move the motor to the desired angle
const float DESIRED_ANGLE_1 = -45;
const float STEPS_PER_REV_1 = 360.0 / STEP_ANGLE * GEAR_RATIO_1;
const float STEPS_TO_ANGLE_1 = STEPS_PER_REV_1 / 360.0;
const int STEPS_TO_MOVE_1 = DESIRED_ANGLE_1 * STEPS_TO_ANGLE_1;

#define DIR_PIN_2 13
#define STEP_PIN_2 16
#define H2_PIN 24
// Calculate the number of steps required to move the motor to the desired angle
const float DESIRED_ANGLE_2 = 60;
const float STEPS_PER_REV_2 = 360.0 / STEP_ANGLE * GEAR_RATIO_2;
const float STEPS_TO_ANGLE_2 = STEPS_PER_REV_2 / 360.0;
const int STEPS_TO_MOVE_2 = DESIRED_ANGLE_2 * STEPS_TO_ANGLE_2;

#define DIR_PIN_3 19
#define STEP_PIN_3 20
#define H3_PIN 23
// Calculate the number of steps required to move the motor to the desired angle
const float DESIRED_ANGLE_3 = 0;
const float STEPS_PER_REV_3 = 360.0 / STEP_ANGLE * GEAR_RATIO_3;
const float STEPS_TO_ANGLE_3 = STEPS_PER_REV_3 / 360.0;
const int STEPS_TO_MOVE_3 = DESIRED_ANGLE_3 * STEPS_TO_ANGLE_3;

///////////////////// GRIPPER /////////////////////
#define servo_pin 8
Servo gripper;

/////////////////////
int pasos = 100;

///////////////////// variables incrementales que guardan la posición en grados del robot.
float pos_cintura = 0;
float pos_hombro = 0;
float pos_codo = 0;

float angCint;
float angHomb;
float angCodo;

bool abrido = true;

///////////////////// Sobre el sensor de color /////////////////////
// TCS230 or TCS3200 pins wiring to Arduino
#define S0 49
#define S1 48
#define S2 47
#define S3 46
#define sensorOut 45

// Stores frequency read by the photodiodes
int colorFrequency = 0;

// Para guardar las frecuencias de los fotodiodos
int frecuenciaRojo = 0;
int frecuenciaVerde = 0;
int frecuenciaAzul = 0;
int frecuenciaNatural = 0;

const int limWhite = 20;
const int limBlack = 100; //200

char color;
// char receivedString[10];  // array to store received string
int charCount = 0; // counter for the target character
char receivedString[10];  // array to store received string

char colorMedido = "";

///////////////////// Sobre colores en pirámide /////////////////////
int R_pos, G_pos, B_pos, K_pos;
int W_pos[6];
char colPiram[10];  // array to store received string

int R_prev, G_prev, B_prev, K_prev;
int W_prev[6];
char colPrev[10];  // array to store received string
int W_turno = 0;

///////////////////// Posiciones de los puntos clave /////////////////////
// float points[7][3] = {
//   {-25, 20.6107, 111},
//   {-14, 45.4610, 65.6083},
//   {-33, 45., 65.6083},
//   {-51, 45.4610, 65.6083},
//   {-72.5, 45.4610, 65.6083},
//   {-93, 45.4610, 65.6083},
//   {-135, 30, 70}
// };

///////////////////// configurar para posiciones de agarre 
float supPoints[10][3] = { /// 
  {-15, 18, 110},
  {-47, 18, 111},
  {-90, 15, 111},
  {-15, 41, 66},
  {-45, 38, 65},
  {-75, 39, 64},
  {-105, 39, 63},

  {-20, 20+9, 60-19}, // configurar a partir de q2 = 20, y q3 = 61
  {-45, 20+9, 60-18}, // 64.2249, 12.8386
  {-75, 20+9, 60-27}, // 54.3673, 27.69
};

///////////////////// configurar para posiciones de descanso  
float lowRestPoints[10][3] = { 
  ///       moverCodo(supPoints[efe][2]-3); para la planta baja y moverHombro(0), 
  // luego moverCodo(45)
  {-15.1, 0, 110-3},
  {-47.1, 0, 111-3},
  {-90.1, 0, 111-3},
  {-15.1, 0, 66-3},
  {-45.1, 0, 65-3},
  {-75.1, 0, 64-3},
  {-105.1, 0, 63-3},

  {-20.1, 20+11, 60-11}, // configurar a partir de q2 = 20, y q3 = 61
  {-45.1, 20+7, 60-11}, // 64.2249, 12.8386
  {-75.1, 20+1, 60-15}, // 54.3673, 27.69
};

///////////////////// configurar para posiciones de descanso  
float highRestPoints[11][3] = { 
  ///       moverCodo(supPoints[efe][2]-3); para la planta baja y moverHombro(0), 
  // luego moverCodo(45)
  {-15.2, 0, 110-30},
  {-47.2, 0, 111-30},
  {-90.2, 0, 111-30},
  {-15.2, 0, 66-30},
  {-45.2, 0, 65-30},
  {-75.2, 0, 64-30},
  {-105.2, 0, 63-30},

  {-20.2, 20, 60-9}, // configurar a partir de q2 = 20, y q3 = 61
  {-45.2, 20, 60-9},
  {-75.2, 20, 60-9},

  {-90, 0, 10},

};

///////////////////// configurar para posiciones de pirámide  
float pyrPoints[10][3] = { /// 
  // {-135, 60.02, 83.08},
  // {-135, 60.02, 83.08},
  // {-135, 60.02, 83.08},
  // {-135, 60.02, 83.08},

  {-125, 33.57, 85},
  {-135, 33.57, 85},
  {-145, 33.57, 85},
  {-155, 33.57, 85},

  {-125, 35, 75},
  {-135, 35, 75},
  {-145, 35, 75},

  {-130, 40, 55},
  {-140, 40, 55},

  {-135, 35, 45},
};

// int efe = 3;
int efe = 9;
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

    // Setting the outputs
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  
  // Setting the sensorOut as an input
  pinMode(sensorOut, INPUT);
  
  // Setting frequency scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

}

void loop() {

  if (Serial.available() > 0) {
    char flag = Serial.read();
      Serial.available() ==0;
      float jaja = Serial.read();
    Serial.println(flag);
    
    if (flag == 'p'){
      // AbreGripper(true);

      // Serial.println(highRestPoints[10][2]);
      // moverCodo(highRestPoints[10][2]);
      // Serial.println(highRestPoints[10][1]);
      // moverHomb(highRestPoints[10][1]);    

      // AbreGripper(false);

      Serial.println(pyrPoints[efe][0]);
      moverCint(pyrPoints[efe][0]);
      Serial.println(pyrPoints[efe][2]);
      moverCodo(pyrPoints[efe][2]);
      Serial.println(pyrPoints[efe][1]);
      moverHomb(pyrPoints[efe][1]);    

      // AbreGripper(true);
      Serial.println(pyrPoints[efe][0]);
      moverCint(pyrPoints[efe][0]);

      efe++;
      if(efe>=10){
        efe=0;
      }
      Serial.println("ya");

    } else if(flag == 'h'){
      homing();
    } else if(flag == 'g'){
      //PruebaGripper();
      abrido = !abrido;
      AbreGripper(abrido);
    } else if(flag == 'r'){
      moverCodo(supPoints[efe][2]-3);
      moverHomb(0);
      efe++;
    } else if(flag == 'c'){
      moverCodo(45);
    } else if(flag=='m'){
      colorMedido = mideColor();
      Serial.println(colorMedido);
    } else if(flag=='i'){
      configPos();
    } else if(flag=='s'){
      scanColors();
    } else if(flag=='t'){
      strncpy(colPrev, "wbwwwgwwkr", 10); // Copy maximum of 10 characters to colPiram  
      strncpy(colPiram, "wrwgwbwwwk", 10); // Copy maximum of 10 characters to colPiram  
      R_pos = 1;
      G_pos = 3;
      B_pos = 5; 
      K_pos = 9;

      R_prev = 7;
      G_prev = 5;
      B_prev = 1;
      K_prev = 8;

      W_prev[0] = 0;
      W_prev[1] = 2;
      W_prev[2] = 3;
      W_prev[3] = 4;
      W_prev[4] = 6;
      W_prev[5] = 9;

      W_turno = 0;
      makePyram();
    } else if(flag=='l'){

    } else if(flag == 'z'){
      homing();
      configPos();
      scanColors();
      buscaW();
      makePyram();

    }

    // moverCodo(ang);
  }

}

void homing(){
  
  pasos = 100;
  giropositivo(STEP_PIN_2, DIR_PIN_2, 10000);
  delay(1000);
  while(digitalRead(H2_PIN) == HIGH){
    pasos = 10 ;
    gironegativo(STEP_PIN_2, DIR_PIN_2, 10000);
    // Serial.println(digitalRead(H2_PIN));
    delay(10);
  }  


  pasos = 15; 
  giropositivo(STEP_PIN_3, DIR_PIN_3, 15000);
  delay(1000);
  while(digitalRead(H3_PIN) == HIGH){
    pasos = 5 ;
    gironegativo(STEP_PIN_3, DIR_PIN_3, 15000);
    // Serial.println(digitalRead(H3_PIN));
    delay(10);
  }   

  pasos = 10;
  gironegativo(STEP_PIN_1, DIR_PIN_1, 5000);  
  delay(1000);
  while(digitalRead(H1_PIN) == HIGH){
    pasos = 5 ;
    giropositivo(STEP_PIN_1, DIR_PIN_1, 5000);
    // Serial.println(digitalRead(H1_PIN));
    delay(100);
  }   

pos_cintura = 0;
pos_hombro = 0;
pos_codo = 0;

}
void calibraCintura(){
  moverCint(-45);
  delay(500);

  while(digitalRead(H1_PIN) == HIGH){
    pasos = 5 ;
    giropositivo(STEP_PIN_1, DIR_PIN_1, 4000);
    Serial.println(digitalRead(H1_PIN));
    delay(100);
  }   

  pos_cintura = 0;
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
  gripper.write(45);  // moves the servo to the 90 degree position
  delay(500);        // waits for 1 second
  // gripper.write(0);   // moves the servo to the 0 degree position
  // delay(1000);        // waits for 1 second
}

void AbreGripper(bool abre){
  if (abre){
    gripper.write(90);  // moves the servo to the 90 degree position
  }else if(!abre){
    gripper.write(0);   // moves the servo to the 0 degree position
  }
    delay(500);        // waits for half a second
}


bool allCharactersSame(String str) {
  char firstChar = str.charAt(0);
  //Serial.println(str.length());
  for (int i = 1; i < 10; i++) {
    if (str.charAt(i) != firstChar) {
      return false;
    }
  }
  return true;
}


char mideColor(){
  // receivedString[0]='a';
  // receivedString[1]='a';
  // receivedString[2]='s';
  // receivedString[3]='s';
  // receivedString[4]='d';
  // receivedString[5]='d';
  // receivedString[6]='f';
  // receivedString[7]='f';
  // receivedString[8]='g';
  // receivedString[9]='g';
  strncpy(receivedString, "aassddffgg", 10); // Copy maximum of 10 characters to colPiram  

  // receivedString[1]='b';
  while(!allCharactersSame(receivedString)){
    charCount++; // increment the counter if the received character is the target character
    charCount = charCount%10;
    // Definiendo la lectura de los fotodiodos con filtro rojo
    digitalWrite(S2,LOW);
    digitalWrite(S3,LOW);
    
    // Leyendo la frecuencia de salida del sensor
    frecuenciaRojo = pulseIn(sensorOut, LOW);
    
    // Mostrando por serie el valor para el rojo (R = Red)
    Serial.print("R = ");
    Serial.print(frecuenciaRojo);
    delay(100);
    
    // Definiendo la lectura de los fotodiodos con filtro verde
    digitalWrite(S2,HIGH);
    digitalWrite(S3,HIGH);
    
    // Leyendo la frecuencia de salida del sensor
    frecuenciaVerde = pulseIn(sensorOut, LOW);
    
    // Mostrando por serie el valor para el verde (G = Green)
    Serial.print(" G = ");
    Serial.print(frecuenciaVerde);
    delay(100);
  
    // Definiendo la lectura de los fotodiodos con filtro azul
    digitalWrite(S2,LOW);
    digitalWrite(S3,HIGH);
    
    // Leyendo la frecuencia de salida del sensor
    frecuenciaAzul = pulseIn(sensorOut, LOW);
    
    // Mostrando por serie el valor para el azul (B = Blue)
    Serial.print(" B = ");
    Serial.println(frecuenciaAzul);
    delay(100);
  
    if(frecuenciaRojo < limWhite && frecuenciaVerde < limWhite && frecuenciaAzul < limWhite){
      color = 'w';
    }else if(frecuenciaRojo > limBlack && frecuenciaVerde > limBlack && frecuenciaAzul > limBlack){
      color = 'k';
    }else if(frecuenciaRojo<frecuenciaVerde && frecuenciaRojo<frecuenciaAzul){
      color = 'r';
    }else if (frecuenciaAzul<frecuenciaRojo && frecuenciaAzul<frecuenciaVerde){
      color = 'b';
    }else{
      color = 'g';
    }
    Serial.println(color);
    receivedString[charCount]=color;
    //Serial.println(charCount);
    Serial.println("\n");
    Serial.println(receivedString);

  }
  return receivedString[5];
}

void configPos(){
  R_pos = -1;
  G_pos = -1;
  B_pos = -1;
  K_pos = -1;
  
  strncpy(colPiram, "wwwwwwwwww", 10); // Copy maximum of 10 characters to colPiram  
  char text[100];
  delay(500);
  
  // Ask user for positions of letters
  Serial.println("Please enter the position for the red object (0-9): ");
  R_inv:
  int xD = Serial.parseInt(); // Read user input as integer
  while (Serial.available() == 0); // Wait for user input
  R_pos = Serial.parseInt(); // Read user input as integer
  Serial.println(R_pos);
    // check if the position is inside the range
    if (R_pos < 0 || R_pos > 9) {
      Serial.println("Invalid position. Please enter a value between 0 and 9.");
      goto R_inv;
    }
  colPiram[R_pos] = 'r';

  delay(500);
  Serial.println("Please enter the position for the green object (0-9): ");
  G_inv:
  xD = Serial.parseInt(); 
  while (Serial.available() == 0); // Wait for user input
  G_pos = Serial.parseInt(); // Read user input as integer
  Serial.println(G_pos);
      // check if the position is inside the range
    if (G_pos < 0 || G_pos > 9) {
      Serial.println("Invalid position. Please enter a value between 0 and 9.");
      goto G_inv;
    } else if (G_pos == R_pos){
      sprintf(text, "Position already taken (%d). Please enter a non-taken value between 0 and 9.", R_pos);
      Serial.println(text);
      goto G_inv;
    }
  colPiram[G_pos] = 'g';

  delay(500);
  Serial.println("Please enter the position for the blue object (0-9): ");
  B_inv:
  xD = Serial.parseInt(); 
  while (Serial.available() == 0); // Wait for user input
  B_pos = Serial.parseInt(); // Read user input as integer
  Serial.println(B_pos);
      // check if the position is inside the range
    if (B_pos < 0 || B_pos > 9) {
      Serial.println("Invalid position. Please enter a value between 0 and 9.");
      goto B_inv;
    } else if (B_pos == G_pos || B_pos == R_pos){
      sprintf(text, "Position already taken (%d, %d). Please enter a non-taken value between 0 and 9.", R_pos, G_pos);
      Serial.println(text);
      goto B_inv;
    }
  colPiram[B_pos] = 'b';

  delay(500);
  Serial.println("Please enter the position for the black object (0-9): ");
  K_inv:
  xD = Serial.parseInt(); 
  while (Serial.available() == 0); // Wait for user input
  K_pos = Serial.parseInt(); // Read user input as integer
  Serial.println(K_pos);
      // check if the position is inside the range
    if (K_pos < 0 || K_pos > 9) {
      Serial.println("Invalid position. Please enter a value between 0 and 9.");
      goto K_inv;
    } else if (K_pos == B_pos || K_pos == G_pos || K_pos == R_pos){
      sprintf(text, "Position already taken (%d, %d, %d). Please enter a non-taken value between 0 and 9.", R_pos, G_pos, B_pos);
      Serial.println(text);
      goto K_inv;
    }
  colPiram[K_pos] = 'k';

  Serial.println(colPiram);
}

void scanColors(){
  //int stand = 0;
  R_prev = -1;
  G_prev = -1;
  B_prev = -1;
  K_prev = -1;
  
  strncpy(colPrev, "wwwwwwwwww", 10); // Copy maximum of 10 characters to colPiram  
  
  for(int stand = 0; stand<10; stand++){
    // homing,    
    if(stand == 0){
      homing();
    }

      AbreGripper(true);

      moverCint(highRestPoints[stand][0]);
      moverCodo(highRestPoints[stand][2]);
      moverHomb(highRestPoints[stand][1]);    
      
      moverCint(lowRestPoints[stand][0]);
      moverCodo(lowRestPoints[stand][2]);
      moverHomb(lowRestPoints[stand][1]);    

      moverCint(supPoints[stand][0]);
      moverCodo(supPoints[stand][2]); // para los niveles altos, hombro luego codo, para los demás al revés
      moverHomb(supPoints[stand][1]);

      PruebaGripper();
      colorMedido = mideColor();
      if(colorMedido !='w'){
        colPrev[stand] = colorMedido;
      }
      if (colorMedido == 'r') {
        R_prev = stand;
      } else if (colorMedido == 'g') {
        G_prev = stand;
      } else if (colorMedido == 'b') {
        B_prev = stand;
      } else if (colorMedido == 'k') {
        K_prev = stand;
      }


      if(stand>=7){
        moverCint(lowRestPoints[stand][0]);
        moverHomb(lowRestPoints[stand][1]);    
        moverCodo(lowRestPoints[stand][2]);

        moverCint(highRestPoints[stand][0]);
        moverHomb(highRestPoints[stand][1]);    
        moverCodo(highRestPoints[stand][2]);

        moverHomb(highRestPoints[10][1]);    
        moverCodo(highRestPoints[10][2]);
        moverCint(highRestPoints[10][0]);
      }else{
        moverCint(lowRestPoints[stand][0]);
        moverCodo(lowRestPoints[stand][2]);
        moverHomb(lowRestPoints[stand][1]);    

        moverCint(highRestPoints[stand][0]);
        moverCodo(highRestPoints[stand][2]);
        moverHomb(highRestPoints[stand][1]);    

        moverCodo(highRestPoints[10][2]);
        moverHomb(highRestPoints[10][1]);    
        moverCint(highRestPoints[10][0]);
      }

    if(stand%3 == 0 && stand != 0){
      calibraCintura();
    }
  }
  
}

void makePyram(){
  // lógica para resolver
  for(int posPyr=0;posPyr<10;posPyr++){
    if(posPyr == R_pos){
      buscaRGBK(R_prev, posPyr);
    } else if(posPyr == G_pos){
      buscaRGBK(G_prev, posPyr);
    } else if(posPyr == B_pos){
      buscaRGBK(B_prev, posPyr);
    } else if(posPyr == K_pos){
      buscaRGBK(K_prev, posPyr);
    } else{
      buscaRGBK(W_prev[W_turno], posPyr);
      W_turno++;
    }
    
    if(posPyr%3 == 0 && posPyr != 0){
      homing();
    }
  }

  homing();
}

void buscaRGBK(int de, int hasta){
      AbreGripper(true);  
      Serial.println(de);
      Serial.println(hasta);

      moverCint(highRestPoints[de][0]);
      moverCodo(highRestPoints[de][2]);
      moverHomb(highRestPoints[de][1]);

      moverCint(lowRestPoints[de][0]);
      moverCodo(lowRestPoints[de][2]);
      moverHomb(lowRestPoints[de][1]);

      moverCint(supPoints[de][0]);
      moverCodo(supPoints[de][2]);
      moverHomb(supPoints[de][1]);

      AbreGripper(false);  
      Serial.println("pabajo");

      moverCint(lowRestPoints[de][0]);
      if (de<7){
        moverCodo(lowRestPoints[de][2]);
      }
      moverHomb(lowRestPoints[de][1]);

      Serial.println("parriba");


      moverCint(highRestPoints[de][0]);
      if (de<7){
        moverCodo(highRestPoints[de][2]);    
      }
      moverHomb(highRestPoints[de][1]);


        moverCodo(highRestPoints[10][2]);
        moverHomb(highRestPoints[10][1]);    
        moverCint(highRestPoints[10][0]);


      moverCint(pyrPoints[hasta][0]);
      moverCodo(pyrPoints[hasta][2]);
      moverHomb(pyrPoints[hasta][1]);

      AbreGripper(true);  

        moverCodo(highRestPoints[10][2]);
        moverHomb(highRestPoints[10][1]);    
        moverCint(highRestPoints[10][0]);


}

void buscaW(){
  int contador = 0;
  for (int i = 0; i < 10; i++) {
    if (colPrev[i] == 'w') {
      // Si encontramos la letra, guardar la posición en el array y aumentar el contador
      W_prev[contador] = i;
      contador++;
    }
  }
}


