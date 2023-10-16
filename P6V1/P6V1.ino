// #include <AccelStepper.h>
#include <Servo.h>      // incluye libreria de Servo


const int stepsPerRevolution = 200;  // Change this to match the number of steps per revolution for your stepper motor
#define PI 3.1415926535897932384626433832795


#define x_paso 4    
#define y_paso 7  
#define z_paso 29
#define x_dire 3    
#define y_dire 6  
#define z_dire 28 
//#define habi 8   
#define x_end 23
#define y_end 24 

Servo servo; // crea objeto 
int retardox = 2075;
int retardoy = 2300;
int retardoz = 15000;
int pasos;
int var = 0;
char m;
int posicionx = 0;
int posiciony = 0;

int minX = 0;
int maxX = 2070*6/7;//*10/12;
int minY = 0;
int maxY = 1070*6/7;//*5/6;

// int CentroX = 60;
// int CentroY = 60;
// int Radio = 0;


// AccelStepper motor1(AccelStepper::DRIVER, motor1StepPin, motor1DirPin);
// AccelStepper motor2(AccelStepper::DRIVER, motor2StepPin, motor2DirPin);

// Circle parameters
double circleX = 60;  // X position of the center of the circle
double circleY = 60;  // Y position of the center of the circle
double radius = 25;   // Radius of the circle

int ratio = 1;
bool redFlag = false;
float theta = 0.0;

double resx;

double MaxPerc=0;
double Porcentaje=0;

const int stepsPerDegree = 5;  // Change this to adjust the resolution of the circle

bool sepodia = true;
bool sepuede = true;

const float calidad = 10.0;
float akiempieza = 0;
int cantPasos;


const float angArco = (3+439/445)/PI;
float delta = 0;

void setup() {
  Serial.begin(9600); 
  servo.attach(11,500,2500);
  servo.write(96);
  pinMode(x_paso, OUTPUT); pinMode(x_dire, OUTPUT); //pinMode(habi, OUTPUT);     
  pinMode(y_paso, OUTPUT); pinMode(y_dire, OUTPUT); 
  pinMode(z_paso, OUTPUT); pinMode(z_dire, OUTPUT); 
  pinMode(x_end,INPUT_PULLUP); pinMode(y_end,INPUT_PULLUP); 
  //digitalWrite(habi, LOW);  // Habilita el Driver 
  homing(); 
  int posiciox = 0;
  int posiiony = 0;

}

void loop() {
  
  // Interrupción serial
  if (Serial.available() > 0) {
    // Read the incoming character
    char incomingByte = Serial.read();
    Serial.available() ==0;
    // Check if the incoming character is 'a'
    if (incomingByte == 'a') {
      // Call your function here
      DisMoi();

      PercMax();

      goto invalide;

      Serial.available() ==0;
      Serial.println("Stop! I'm already dead.");
      while(Serial.available()==0){}
      redFlag = Serial.read();

      if (redFlag){
        Serial.println("Un gantel tu.");
        goto invalide;
        Serial.println("Un gantel tu.2");
     } 
     // homing(); 
      Serial.println("Ta bien, papito, ya va.");

      //Serial.println("con lapi");

      moverxy(circleX*ratio,circleY*ratio);
      Serial.println("en centro");
      // while (Serial.available()){

      // }
      moverxy((circleX+radius)*ratio,circleY*ratio);
      Serial.println("en primer punto");
      delay(1000);
    
      //circulo();
    }
  }
  //*/


  invalide:
  Serial.print("");


}

void circulo(int CX, int CY, int R, int PasitoAPasito){
  int angul = 0;
  double equis = CX + (R * cos(0));
  double ye = CY + (R * sin(0));
  double tetita = 0;


  for(int i=0; i<PasitoAPasito;i+=1){
    //Serial.println(cantPasos);
    angul+=calidad;
    
    tetita = angul * PI / 180;
    equis = CX + (R * cos(tetita));
    ye = CY + (R * sin(tetita));
    int steps1 = (int)(equis * stepsPerDegree);
    int steps2 = (int)(ye * stepsPerDegree);

    moverxy(equis,ye);
  }
}



/////////////////////////////////// Funciones de movimiento ///////////////////////////////////  

void moverX(float x){
   int ax = 0;
   
   if (x > posicionx){
      x = x - posicionx;
      posicionx = posicionx + x;
      ax = 1;
   }else{ 
      x = posicionx - x;
      posicionx = posicionx - x;
   }   

    if (x < 0){
      x = x*-1;
      }

   
   if(ax == 1){
     digitalWrite(x_dire,HIGH);
   }else{
     digitalWrite(x_dire,LOW);
   }
   
   x = x*6.125;
     if (ax == 1){ 
      pasos = x;
       giropositivo(x_paso,x_dire,retardox);
     }else{
      pasos = x;
       gironegativo(x_paso,x_dire,retardox);
     }  
  delay(500); 
}

void moverY(float y){
  int ay = 0;

      
   if (y > posiciony){
      y = y - posiciony;
      posiciony = posiciony + y;
      ay = 1;
   }else{ 
      y = posiciony - y;
      posiciony = posiciony - y;
    }   

      
   if (y < 0 ){
      y = y*-1;
     }
   
   if(ay == 1){
    digitalWrite(y_dire,HIGH);
   }else{
    digitalWrite(y_dire,LOW);
   }
   y = y*6.2; 
     if (ay == 1){
     pasos = y;
     giropositivo(y_paso,y_dire,retardoy);
     }else{
     pasos = y;
     gironegativo(y_paso,y_dire,retardoy);
     }
   delay(500);  
}


void moverxy(double x, double y){
  x = x*6/7;
  y = y*6/7;
    int ax = 0,ay = 0;
    float auxX = 1,auxY = 1; 
     if (x > posicionx){
      x = x - posicionx;
      posicionx = posicionx + x;
      ax = 1;
   }else{ 
      x = posicionx - x;
      posicionx = posicionx - x;
   }   
   
   if (y > posiciony){
      y = y - posiciony;
      posiciony = posiciony + y;
      ay = 1;
   }else{ 
      y = posiciony - y;
      posiciony = posiciony - y;
    }     

    // if(x>265 || x<0 || y>145 || y<0 ){

    //   Serial.println("Porcentaje de nuevo");

    //   while(Serial.available()==0){}
    //   //porcen=compro;
    //   //porcen = Serial.parseFloat();
    //   //double porcenntaje = porcen/100;

    // }    
   if (x < 0 ){
      y = y*-1;
     }
      
   if (y < 0 ){
      y = y*-1;
     }
        
   if(ay == 1){
    digitalWrite(y_dire,HIGH);
   }else{
    digitalWrite(y_dire,LOW);
   }
   if(ax == 1){
     digitalWrite(x_dire,HIGH);
   }else{
     digitalWrite(x_dire,LOW);
   } 
  float indexMax;
  float Pasos_para_x,Pasos_para_y;
      Pasos_para_x = x*6.2 ;
      Pasos_para_y = y*6.2; 
  
  
  if(Pasos_para_x >= Pasos_para_y){
    indexMax = Pasos_para_x;
  }else{
    indexMax = Pasos_para_y;
  }
  
  int Correcion_en_x_Soy_Vago = 0;
  int Correcion_en_y_Soy_Vago = 0;
    double Paso_en_x;
    double Paso_en_y;
   double error_real_x = (resx - Paso_en_x);
 
   Serial.println(error_real_x);
  for (int index = 1; index <= indexMax; index+=1) {
    Paso_en_x = index*(Pasos_para_x/indexMax) - Correcion_en_x_Soy_Vago;
    Paso_en_y = index*(Pasos_para_y/indexMax) - Correcion_en_y_Soy_Vago;
    //Serial.println(Paso_en_x);
    //Serial.println(Paso_en_y);
    if(Paso_en_x > 0.85){
      Correcion_en_x_Soy_Vago++;
      digitalWrite(x_paso, HIGH);      
      delayMicroseconds(retardox);          
      digitalWrite(x_paso, LOW);       
      delayMicroseconds(retardox); 
    }
    if(Paso_en_y > 0.85){
      Correcion_en_y_Soy_Vago++;
      digitalWrite(y_paso, HIGH);      
      delayMicroseconds(retardoy);          
      digitalWrite(y_paso, LOW);       
      delayMicroseconds(retardoy); 
    }
  }
}


void giropositivo(int paso_,int dire_,int retar) {
   digitalWrite(dire_, HIGH);   // direccion de giro 1
   for(int i=0;i<pasos;i++){  // da  pasos por un tiempo  
    digitalWrite(paso_, LOW);      
    delayMicroseconds(retar);          
    digitalWrite(paso_, HIGH);       
    delayMicroseconds(retar); 
   }
}


void gironegativo(int paso_,int dire_,int retar) {
  digitalWrite(dire_,LOW );   // direccion de giro 2
  for(int i=0;i<pasos;i++){   // da  pasos por un tiempo  
    digitalWrite(paso_, HIGH);      
    delayMicroseconds(retar);          
    digitalWrite(paso_, LOW);       
    delayMicroseconds(retar);  
  }
}



/////////////////////////////////// Movimientos predeterminados ///////////////////////////////////  

void homing(){

    pasos = 150;
    giropositivo(y_paso,y_dire,retardoy);
    delay(200);
    pasos = 150;
    giropositivo(x_paso,x_dire,retardox);
    while(digitalRead(x_end)== 1){
      pasos = 6 ;
      gironegativo(x_paso,x_dire,retardox);
    }   
    while(digitalRead(y_end)== 1){
      pasos = 6;
      gironegativo(y_paso,y_dire,retardoy);
    }  
    
    pasos = 100;
    giropositivo(y_paso,y_dire,retardoy);
    pasos = 100;
    giropositivo(x_paso,x_dire,retardox);
    
    delay(500);

}



/////////////////////////////////// KLK ///////////////////////////////////  


void DisMoi(){
  int jaja = Serial.parseInt();
  
  Serial.available() ==0;
  Serial.println("Centro en x: ");
  // char jaja = Serial.read();
  Serial.println(Serial.available());
  while(Serial.available()==0) { }
  // delay(1000);  
  circleX = Serial.parseInt();
  delay(100);  

  Serial.available() ==0;
  Serial.println("Centro en y: ");
  // jaja = Serial.read();
  jaja = Serial.parseInt();

  Serial.println(Serial.available());
  while(Serial.available()==0) { }
  circleY = Serial.parseInt();

  circleX = double(circleX);
  circleY = double(circleY);


  Serial.println("\n");
  Serial.print(circleX);
  Serial.print(", ");
  Serial.print(circleY);
  Serial.println("\n");

  Serial.available() ==0;
  Serial.println("Radio: ");
  // jaja = Serial.read();
  jaja = Serial.parseInt();

  Serial.println(Serial.available());
  while(Serial.available()==0) { }
  radius = Serial.parseInt();
  radius = double(radius);


  Serial.println("\n");
  Serial.print(radius);

  delta = sqrt(2)*radius*cos(angArco/2);
  int infX = circleX - delta - radius;
  int infY = circleY - radius;
  int supX = circleX + delta + radius; 
  int supY = circleY + delta + radius;


  if(infX<minX || supX>maxX || infY<minY || supY>maxY){
    Serial.println("xd");
  
    redFlag = true;
  }else{
    redFlag = false;
  }

}



void PercMax(){
  int angul = 0;
  double prevX, prevY;
  double equis = circleX + (radius * cos(0));
  double ye = circleY + (radius * sin(0));
  double tetita = 0;

  double pasos = 0;
  double pasitos = 0;

  MaxPerc = 100;
  Serial.available() ==0;
  Serial.println("Porcentaje deseado: ");
  //Serial.available() ==0;

  float equide = Serial.parseFloat();
  while(Serial.available()==0){}
  Porcentaje = Serial.parseFloat();
  if(Porcentaje>MaxPerc){
    Serial.println("mamaste");
    return;
  }

  cantPasos = 3*Porcentaje*360/(calidad*100); // el 3 es la cantidad de círculos

  // angul = akiempieza;

  if(cantPasos<=36){
    circulo(circleX+delta, circleY+delta, radius, cantPasos);
  }else if(cantPasos<=36*2){
    int suavecito = cantPasos-36;
    circulo(circleX+delta, circleY+delta, radius, 36);
    circulo(circleX, circleY, radius, suavecito);
  }else{
    int suavecito = cantPasos-36*2;
    circulo(circleX+delta, circleY+delta, radius, 36);
    circulo(circleX, circleY, radius, 36);
    circulo(circleX-delta, circleY+delta, radius, suavecito);

  }

}




