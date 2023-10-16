#define in1 8
#define in2 9
#define in3 10
#define in4 11

void Stepping(int pasito);
bool CW = false;

void setup() {
  // put your setup code here, to run once:
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);

  Serial.begin(9600);
}

void loop() {
  if(Serial.available()){
    float vueltas = Serial.parseFloat();
    if (vueltas>0)CW=false;
    else CW=true;
    vueltas = abs(vueltas);
    int pasos = int(vueltas*64*32);
    Stepping(pasos);
    Serial.println(String(pasos));
  }
}

///////////////////////////// ROTACIÓN /////////////////////////////
void Stepping(int pasito){
    int i=0, j=1;
    for(i=0; i<pasito; i++){
        if (CW==true){
            if(j==1) j=2; 
            else if(j==2) j=3; 
            else if(j==3) j=4; 
            else if(j==4) j=1; 
        } 
        if (CW==false){
            if(j==1) j=4; 
            else if(j==2) j=1; 
            else if(j==3) j=2; 
            else if(j==4) j=3; 
        } 
        
        if (j==1){
            digitalWrite(in1, HIGH);
            digitalWrite(in2, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in4, LOW);
        } else if (j==2){
            digitalWrite(in1, LOW);
            digitalWrite(in2, HIGH);
            digitalWrite(in3, LOW);
            digitalWrite(in4, LOW);
        } else if (j==3){
            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
            digitalWrite(in3, HIGH);
            digitalWrite(in4, LOW);
        } else if (j==4){
            digitalWrite(in1, LOW);
            digitalWrite(in2, LOW);
            digitalWrite(in3, LOW);
            digitalWrite(in4, HIGH);
        }
        delay(5);
    }
    
}
