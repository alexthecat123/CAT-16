#define srData 11
#define srClock 10
#define srPower 5
#define RAMWE 2
#define compClock 8
#define notCompReset 6
#define compReset 7
#define PCOE 69696969
#define MARWE 69696969
#define PCEN 3
#define MCRES 4
#define HLT 9

long int stepCounter = 0;
long int currentAddress = 0;
bool runClock = false;
float period = 0;

void setup() {
  Serial.begin(2000000);
  Serial.setTimeout(5);
  initComputer();

  /*for(int i = 0; i < 16384; i++){
    writeWord(0);
    //delay(200);
    incPC(1); 
  }*/

  

}

void loop() {
  String data = "";
  String halt = "stop:";
  if(Serial.available()){
    data = Serial.readStringUntil(":");
    long int intData = strtol( data.c_str(), NULL, 2 );
    //Serial.println(intData);
    if(intData == 70000){
      //Serial.println("Clock Time!");
      resetComputer();
      clockHandler();
    }
    else if(stepCounter == 0){
      incPC(intData - currentAddress);
      //Serial.println("Step 1: Increment PC by " + String(intData - currentAddress));
      currentAddress = intData;
      stepCounter += 1;
      pulseClock();
    }
    else if(stepCounter == 1){
      writeWord(intData);
      //Serial.println("Step 2: Write word " + String(intData));
      incPC(1);
      currentAddress += 1;
      stepCounter += 1;
      pulseClock();
    }
    else if(stepCounter == 2){
      //Serial.println("Step 3: Write word " + String(intData));
      writeWord(intData);
      stepCounter = 0;
      pulseClock();
    }
  }
}



void clockHandler(){
  deInitComputer();
  digitalWrite(srPower, LOW);
  while(true){
    if(Serial.available()){
      String clockRaw = Serial.readStringUntil(":");
      long int clockData = strtol( clockRaw.c_str(), NULL, 2 );
      if(clockData == 70001){
        //Serial.println("Free-Running!");
        freeRunning();
      }
      else if(clockData == 70002){
        //Serial.println("Single Step!");
        singleStep();
      }
      else if(clockData == 70004){
        resetComputer();
      }
    }
  }
}

void freeRunning(){
  while(true){
    if(Serial.available()){
      String FRRaw = Serial.readStringUntil(":");
      long int FRData = strtol( FRRaw.c_str(), NULL, 2 );
      if(FRData == 70003){
        //Serial.println("Back to Clock Handler!");
        clockHandler();
      }
      else if(FRData == 70004){
        resetComputer();
      }
      else if(FRData == 70005){
        //Serial.println("Clock Running");
        runClock = true;
      }
      else if(FRData == 70006 ){
        //Serial.println("Clock Stopped");
        runClock = false;
      }
      else{
        period = 1.00/FRData;
        period *= 1000;
        period *= 1000;
        Serial.println(period);
      }
    }
    if(!Serial.available() and runClock == true and digitalRead(9) == HIGH){
      //Serial.println("Pulse");
      pulseClock();
      delayMicroseconds(350);
    }
  }
}

void singleStep(){
  while(true){
    if(Serial.available()){
      String SSRaw = Serial.readStringUntil(":");
      long int SSData = strtol( SSRaw.c_str(), NULL, 2 );
      if(SSData == 70003){
        //Serial.println("Back to Clock Handler!");
        clockHandler();
      }
      else if(SSData == 70004){
        resetComputer();
      }
      else if(digitalRead(9) == HIGH){
        //Serial.println("Stepping...");
        pulseClock();
      }
    }
  }  
}

void writeWord(long int meow){
  //byte ByteHigh = (meow & 65280)/256;
  //byte ByteLow = meow & 255;
  //digitalWrite(srPower, HIGH);
  //shiftOut(srData, srClock, LSBFIRST, ByteLow); //Commented out lines are for shift register.
  //shiftOut(srData, srClock, MSBFIRST, ByteHigh);
  long int j = 1;
  for(int i = 22; i <= 36; i += 2){
    bool num = (meow & j);
    digitalWrite(i, num);
    j *= 2;
  }
  for(int i = 52; i > 36; i -= 2){
    bool num = (meow & j);
    digitalWrite(i, num);
    j *= 2;
  }
  digitalWrite(RAMWE, HIGH);
  delay(1);                         
  digitalWrite(RAMWE, LOW);
  //digitalWrite(srPower, LOW);
}

void incPC(long int steps){
  for(long int i = 0; i < steps; i++){
    digitalWrite(PCEN, HIGH);
    pulseClock();
    digitalWrite(PCEN, LOW);
  }
}


void pulseClock(){
  digitalWrite(compClock, HIGH);
  delayMicroseconds(1);
  digitalWrite(compClock, LOW);
}

void resetComputer(){
  digitalWrite(notCompReset, LOW);
  digitalWrite(compReset, HIGH);
  delay(10);
  digitalWrite(notCompReset, HIGH);
  digitalWrite(compReset, LOW);
}

void initComputer(){
  for(int i = 20; i <= 54; i += 1){
    pinMode(i, OUTPUT);
  }
  pinMode(srData, OUTPUT);
  pinMode(srClock, OUTPUT);
  pinMode(srPower, OUTPUT);
  pinMode(RAMWE, OUTPUT);
  pinMode(compClock, OUTPUT);
  pinMode(notCompReset, OUTPUT);
  pinMode(compReset, OUTPUT);
  pinMode(PCEN, OUTPUT);
  pinMode(MCRES, OUTPUT);
  pinMode(HLT, INPUT);
  digitalWrite(srPower, LOW);
  digitalWrite(compClock, LOW);
  digitalWrite(notCompReset, HIGH);
  digitalWrite(compReset, LOW);
  digitalWrite(PCEN, LOW);
  digitalWrite(MCRES, LOW);
  resetComputer();
}


void deInitComputer(){
  for(int i = 20; i <= 54; i += 1){
    pinMode(i, INPUT);
  }
  pinMode(srData, INPUT);
  pinMode(srClock, INPUT);
  //pinMode(srPower, INPUT);
  pinMode(RAMWE, INPUT);
  //pinMode(compClock, INPUT);
  //pinMode(notCompReset, INPUT);
  //pinMode(compReset, INPUT);
  pinMode(PCEN, INPUT);
  pinMode(MCRES, INPUT);
}
