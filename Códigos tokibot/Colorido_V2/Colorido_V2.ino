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

char color;
char receivedString[10];  // array to store received string
int charCount = 0; // counter for the target character


void setup() {
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
  
  // Begins serial communication
  Serial.begin(9600);
}
void loop() {
  receivedString[0]='a';
  receivedString[1]='a';
  receivedString[2]='s';
  receivedString[3]='s';
  receivedString[4]='d';
  receivedString[5]='d';
  receivedString[6]='f';
  receivedString[7]='f';
  receivedString[8]='g';
  receivedString[9]='g';
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
  
    // // Definiendo la lectura de los fotodiodos con filtro azul
    // digitalWrite(S2,HIGH);
    // digitalWrite(S3,LOW);
    
    // // Leyendo la frecuencia de salida del sensor
    // frecuenciaAzul = pulseIn(sensorOut, LOW);
    
    // // Mostrando por serie el valor para el azul (B = Blue)
    // Serial.print(" C = ");
    // Serial.println(frecuenciaNatural);
    // delay(100);

    if(frecuenciaRojo < 25 && frecuenciaVerde < 25 && frecuenciaAzul < 25){
      color = 'w';
    }else if(frecuenciaRojo > 75 && frecuenciaVerde > 75 && frecuenciaAzul > 75){
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
