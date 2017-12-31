long startTime, stopTime;
long duration, smallturn;

int encoderApinA=2;
int encoderApinB=12;
int encoderBpinA=3;
int encoderBpinB=13;
int Apin=4;
int Apin2=5;
int Bpin=7;
int Bpin2=8;
int kiirusA= 9;
int kiirusB= 10;

volatile int counterA=0;
volatile int counterB=0;
unsigned long int praegu;
int eelmineaeg=0;
int i=0;

const int paremIn = A5;
const int paremOut = A4;
const int vasakIn = A3;
const int vasakOut = A2;
const int keskPin = 11;

const int vkiirus = 33; //40
const int pkiirus = 30; //36
const int kaugel = 18;
const int otse_delay = 5;
int sonarK1,sonarK2, sonarV, sonarK,sonarP, turncounter=0, counterA1, counterB1;

void setup() {  
  // put your setup code here, to run once:
  pinMode(encoderApinA, INPUT);
  pinMode(encoderApinB, INPUT);
  pinMode(encoderBpinA, INPUT);
  pinMode(encoderBpinB, INPUT);
  pinMode(Apin, OUTPUT);
  pinMode(Apin2, OUTPUT);
  pinMode(Bpin, OUTPUT);
  pinMode(Bpin2, OUTPUT);
  pinMode(kiirusA, OUTPUT);
  pinMode(kiirusB, OUTPUT);
  pinMode(6, OUTPUT); 
  digitalWrite(6, HIGH);

  pinMode(paremIn, INPUT);
  pinMode(paremOut, OUTPUT);
  pinMode(vasakIn, INPUT);
  pinMode(vasakOut, OUTPUT);
  pinMode(keskPin, OUTPUT);

  attachInterrupt(0, countA, CHANGE); //loeb vaid muutust encoderpinA-s
  attachInterrupt(1, countB, CHANGE);
  Serial.begin(9600);
  Serial.print("setup");

  duration=780;
  smallturn = 2;
  
  delay(5000);
  startTime=millis();
  stopTime=startTime+duration;
}

void loop() 
{
  
  digitalWrite(Apin, HIGH);
  digitalWrite(Apin2, LOW);    
  digitalWrite(Bpin, LOW);
  digitalWrite(Bpin2, HIGH);
  sonarV = distVasak();
  sonarP = distParem();
  //sonarK1=sonarK;
  sonarK = distKesk();
  /*
  if (sonarK1==sonarK/* || sonarK1==sonarK+1) {
    turncounter++;
  } else {
    turncounter=0;
  } */

  counterA1=counterA;
  counterB1=counterB;
  Serial.println(counterA);
  Serial.println(counterB);

  if(counterA1==counterA && counterB1==counterB){
    turncounter++;
  }
  else{
    turncounter=0;
  }
  Serial.println(String(sonarV)+ " V " + String(sonarK) + " K " + String(sonarP) );

  if (sonarP < 15 && sonarV < 15){ //näeme mõlemaid seinu mõistlikult
     if (sonarK < 4) {     // oleme tupikus
          Serial.println("going to Right tupik");
          turnRight(duration);
         
    }
    if (sonarP > sonarV){ //vasak on lähemal
      analogWrite(kiirusA, 0);
      analogWrite(kiirusB, vkiirus);
      Serial.println("Parem");
    }
    else if (sonarP < sonarV){ //parem on lähemal
      analogWrite(kiirusA, pkiirus);
      analogWrite(kiirusB, 0);
      Serial.println("Vasak");
    }
    else if (sonarP == sonarV){ //samal kaugusel
      analogWrite(kiirusA, pkiirus);
      analogWrite(kiirusB, vkiirus);
      Serial.println("Otse");
    }    
  }

  else if (sonarP > kaugel || sonarV > kaugel){ //vähemalt ükssein on kaugel
      Serial.println("YKS KAUGEL" );
   
      
      if (sonarK < 4){  // otseseinale lähedale
        Serial.println("ots paistab");
        if (sonarP > kaugel && sonarV < kaugel){//parem on kas vastu seina või auk
          Serial.println("going to Right");
          turnRight(duration);
//        Serial.println("Right");
        }
        else if (sonarP < kaugel && sonarV > kaugel){ //vasak on kas vastu seina või auk
          Serial.println("going to Left");
          turnLeft(duration);
//        Serial.println("Left");
        }
        else if (sonarP > kaugel && sonarV > kaugel){ //T-kujuline ristmik
          if (flip() == 1){ //viskame münti
            Serial.println("going to Right2");
            turnRight(duration);
//          Serial.println("Right2");
          }
          else { //viskame münti
            Serial.println("going to Left2");
            turnLeft(duration);
//          Serial.println("Left2");
          }
        }
      } 
      else {
        korrigeerib(sonarP, sonarV);
      }
    
    /* else if((sonarP<kaugel && sonarV>kaugel) || (sonarP>kaugel && sonarV<kaugel)){ //ees rohkem ruumi kui 22 cm ehk ees seina pole ja ainult üks külg vaba
      if(sonarP<kaugel && sonarV>kaugel){ //vasak külg vaba
        if (flip() == 1){
            //põõre vasakule:1. panna otse sõitma, siis delay ja siis põõre 2. panna aeg jooksma, selle jooksul korrigeerib end ja sõidab otse ja siis pööre
            analogWrite(kiirusA, pkiirus);
            analogWrite(kiirusB, vkiirus);
            delay(300);
            turnLeft(duration);

            /* long drivetime;
            startTime=millis();
            drivetime=300;
            stopTime=startTime+drivetime;
            while((stopTime - startTime) <= drivetime){
              digitalWrite(Apin, HIGH);
              digitalWrite(Apin2, LOW);
              analogWrite(kiirusA, 40);
              digitalWrite(Bpin, HIGH);
              digitalWrite(Bpin2, LOW);
              analogWrite(kiirusB, 36); // peaks pöörama umbes sama kiirusega
      
              stopTime=millis();
             }
             
            
          }
        else{ //otsustas otse sõita
          korrigeerib(sonarP, sonarV);
          }
        }
      else if(sonarP>kaugel && sonarV<kaugel){ //parem külg vaba
        if (flip() == 1){
            //põõre paremale
            
            analogWrite(kiirusA, pkiirus);
            analogWrite(kiirusB, vkiirus);
            delay(300);
            turnRight(duration);
          }
        else{ //otsustas otse sõita
          korrigeerib(sonarP, sonarV);
          } 
        }
      } */
  }
   /* else{ //igast küljest vaba, sõidab otse
      analogWrite(kiirusA, pkiirus);
      analogWrite(kiirusB, vkiirus);
      Serial.println("ristmik");
    } */


  else{ // ei teinud otsust, liigume otse
      analogWrite(kiirusA, pkiirus);
      analogWrite(kiirusB, vkiirus);
      Serial.println("Otse 2");
  }
  delay (1);
//    analogWrite(kiirusA, pkiirus);
//    analogWrite(kiirusB, vkiirus);
  Serial.println("Otse 3");
  if (turncounter>50){
    //tagasi
    Serial.println("tagasi");
    reverse ();
    turncounter=0;
    
  } 
}


bool flip()
{
  static uint32_t buf = 0;
  static uint8_t idx = 0;
  if (idx)
  {
    buf >>= 1;
    idx--;
  }
  else
  {
    buf = random();  // refill
    idx = 30;
  }
  return buf & 0x01;
}

long distParem(){
  long dur1;
  long dis;
  dur1=distParem1();
  dis=dur1;
  return dis;
}
long distKesk(){
  long dur1;
  long dis;

  dur1=distKesk1();
  dis= dur1;
  return dis;
}

long distVasak(){
  long dur1;
  long dis;
  
  dur1=distVasak1();
  dis=dur1;
  return dis;
}
long distParem1(){
  long dur;
  long dis;

  digitalWrite(paremOut,LOW);
  delayMicroseconds(2);
  digitalWrite(paremOut,HIGH);
  delayMicroseconds(10);
  digitalWrite(paremOut,LOW);
  
  dur=pulseIn(paremIn,HIGH);

  dis=dur/29/2;

  return dis;
}

long distKesk1(){
  long dur;
  long dis=0;
  digitalWrite(keskPin,LOW);
  delayMicroseconds(2);
  digitalWrite(keskPin,HIGH);
  delayMicroseconds(10);
  digitalWrite(keskPin,LOW);
  
  pinMode(keskPin, INPUT);  
  dur=pulseIn(keskPin,HIGH);

  dis=dur/29/2;

  return dis;
}

long distVasak1(){
  long dur;
  long dis;

  digitalWrite(vasakOut,LOW);
  delayMicroseconds(2);
  digitalWrite(vasakOut,HIGH);
  delayMicroseconds(10);
  digitalWrite(vasakOut,LOW);
  
  dur=pulseIn(vasakIn,HIGH);

  dis=dur/29/2;

  return dis;
}


void turnRight(long amount){
  long turntime;
  startTime=millis();
  turntime=amount;
  stopTime=startTime+turntime;
  Serial.println(" Turnright: ");
  while((stopTime - startTime) <= turntime)
  {
      digitalWrite(Apin, LOW);
      digitalWrite(Apin2, HIGH);
      analogWrite(kiirusA, 40);
      digitalWrite(Bpin, LOW);
      digitalWrite(Bpin2, HIGH);
      analogWrite(kiirusB, 36); // peaks pöörama umbes sama kiirusega
      
      stopTime=millis();
  }
    analogWrite(kiirusA, 0);
    analogWrite(kiirusB, 0);
}

void turnLeft(long amount){
  long turntime;
  startTime=millis();
  turntime=amount;
  stopTime=startTime+turntime;
  Serial.println("Turnleft: ");
  while((stopTime - startTime) <= turntime)
  {
      digitalWrite(Apin, HIGH);
      digitalWrite(Apin2, LOW);
      analogWrite(kiirusA, 40);
      digitalWrite(Bpin, HIGH);
      digitalWrite(Bpin2, LOW);
      analogWrite(kiirusB, 36); // peaks pöörama umbes sama kiirusega
      
      stopTime=millis();
  }
    analogWrite(kiirusA, 0);
    analogWrite(kiirusB, 0);
}

void reverse() {
  long revtime= 500;
  startTime=millis();
  stopTime=startTime+revtime;
  if(flip()==1){
    while((stopTime - startTime) <= revtime)
    {
      digitalWrite(Apin, LOW);
      digitalWrite(Apin2, HIGH);
      analogWrite(kiirusA, 0);//40
      digitalWrite(Bpin, HIGH);
      digitalWrite(Bpin2, LOW);
      analogWrite(kiirusB, 40); // peaks pöörama umbes sama kiirusega
      stopTime=millis();
    }
  }
   else{
    while((stopTime - startTime) <= revtime)
    {
      digitalWrite(Apin, LOW);
      digitalWrite(Apin2, HIGH);
      analogWrite(kiirusA, 40);//40
      digitalWrite(Bpin, HIGH);
      digitalWrite(Bpin2, LOW);
      analogWrite(kiirusB, 0);
      stopTime=millis();
      
    }
  }
}

void korrigeerib(int paremsonar, int vasaksonar){
        if(paremsonar<4){
          analogWrite(kiirusB, 0);
          analogWrite(kiirusA, pkiirus);
          Serial.println("enne põõret vasakule");
        }
        else if(vasaksonar<4){
          analogWrite(kiirusA, 0);
          analogWrite(kiirusB, vkiirus);
          Serial.println("enne põõret paremale");
        }
        else if(paremsonar<12 && paremsonar>5){
          analogWrite(kiirusA, 0);
          analogWrite(kiirusB, vkiirus);
          Serial.println("enne põõret vasakule 2");
        }
        else if(vasaksonar<12 && vasaksonar>5){
          analogWrite(kiirusA, pkiirus);
          analogWrite(kiirusB, 0);
          Serial.println("enne põõret paremale 2");
        }
        else{
          analogWrite(kiirusA, pkiirus);
          analogWrite(kiirusB, vkiirus);
          Serial.println("enne põõret otse");
        }
}

void countA(){
  if (digitalRead(encoderApinA)== digitalRead(encoderApinB)){
    counterA++;
  }
  else{
    counterA--;
  }
}

void countB(){
  if (digitalRead(encoderBpinA) == digitalRead(encoderBpinB)){
    counterB++;
  }
  else{
    counterB--;
  }
  
}
  
  

