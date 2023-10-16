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

int limWhite = 40;
int limBlack = 200;

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
  {-25, 20.6107, 111},
  {-14, 45.4610, 65.6083},
  {-33, 45., 65.6083},
  {-51, 45.4610, 65.6083},
  {-72.5, 45.4610, 65.6083},
  {-93, 45.4610, 65.6083},
  {-135, 30, 70}
};

///////////////////// configurar para posiciones de descanso  
float lowRestPoints[10][3] = { /// 
  {-25, 20.6107, 111},
  {-14, 45.4610, 65.6083},
  {-33, 45., 65.6083},
  {-51, 45.4610, 65.6083},
  {-72.5, 45.4610, 65.6083},
  {-93, 45.4610, 65.6083},
  {-135, 30, 70}
};

///////////////////// configurar para posiciones de descanso  
float highRestPoints[10][3] = { /// 
  {-25, 20.6107, 111},
  {-14, 45.4610, 65.6083},
  {-33, 45., 65.6083},
  {-51, 45.4610, 65.6083},
  {-72.5, 45.4610, 65.6083},
  {-93, 45.4610, 65.6083},
  {-135, 30, 70}
};

///////////////////// configurar para posiciones de pirámide  
float pyrPoints[10][3] = { /// 
  {-25, 20.6107, 111},
  {-14, 45.4610, 65.6083},
  {-33, 45., 65.6083},
  {-51, 45.4610, 65.6083},
  {-72.5, 45.4610, 65.6083},
  {-93, 45.4610, 65.6083},
  {-135, 30, 70}
};


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
      Serial.println(DESIRED_ANGLE_1);
      moverCint(DESIRED_ANGLE_1);
      Serial.println(DESIRED_ANGLE_2);
      moverHomb(DESIRED_ANGLE_2);
      Serial.println(DESIRED_ANGLE_3);
      moverCodo(DESIRED_ANGLE_3);
    } else if(flag == 'h'){
      homing();
    } else if(flag == 'g'){
      //PruebaGripper();
      abrido = !abrido;
      AbreGripper(abrido);
    } else if(flag == 'r'){
      moverCodo(DESIRED_ANGLE_3-5);
      moverHomb(0);
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

  pasos = 15; 
  giropositivo(STEP_PIN_3, DIR_PIN_3, 10000);
  delay(1000);
  while(digitalRead(H3_PIN) == HIGH){
    pasos = 5 ;
    gironegativo(STEP_PIN_3, DIR_PIN_3, 10000);
    Serial.println(digitalRead(H3_PIN));
    delay(10);
  }   
  
  pasos = 100;
  giropositivo(STEP_PIN_2, DIR_PIN_2, 10000);
  delay(1000);
  while(digitalRead(H2_PIN) == HIGH){
    pasos = 10 ;
    gironegativo(STEP_PIN_2, DIR_PIN_2, 10000);
    Serial.println(digitalRead(H2_PIN));
    delay(10);
  }  

  pasos = 10;
  gironegativo(STEP_PIN_1, DIR_PIN_1, 2000);  
  delay(1000);
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
void calibraCintura(){
  moverCint(-45);
  delay(500);

  while(digitalRead(H1_PIN) == HIGH){
    pasos = 5 ;
    giropositivo(STEP_PIN_1, DIR_PIN_1, 2000);
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

    //descanso bajo, posicion, 
    AbreGripper(true);
    moverCint(lowRestPoints[stand][0]);
    moverCodo(lowRestPoints[stand][2]);
    moverHomb(lowRestPoints[stand][1]);

    moverCint(supPoints[stand][0]);
    moverCodo(supPoints[stand][2]);
    moverHomb(supPoints[stand][1]);

    colorMedido = mideColor();
    if(colorMedido !='w'){
      colPrev[stand] = colorMedido;
    }
    if (mideColor == 'r') {
      R_prev = stand;
    } else if (mideColor == 'g') {
      G_prev = stand;
    } else if (mideColor == 'b') {
      B_prev = stand;
    } else if (mideColor == 'k') {
      K_prev = stand;
    }

    //descanso bajo, descanso alto
    moverCint(lowRestPoints[stand][0]);
    moverCodo(lowRestPoints[stand][2]);
    moverHomb(lowRestPoints[stand][1]);

    moverCint(highRestPoints[stand][0]);
    moverCodo(highRestPoints[stand][2]);
    moverHomb(highRestPoints[stand][1]);

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
  }

  homing();
}

void buscaRGBK(int de, int hasta){
      AbreGripper(true);  

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

      moverCint(lowRestPoints[de][0]);
      moverCodo(lowRestPoints[de][2]);
      moverHomb(lowRestPoints[de][1]);

      moverCint(highRestPoints[de][0]);
      moverCodo(highRestPoints[de][2]);
      moverHomb(highRestPoints[de][1]);

      moverCint(pyrPoints[hasta][0]);
      moverCodo(pyrPoints[hasta][2]);
      moverHomb(pyrPoints[hasta][1]);

      AbreGripper(true);  

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


