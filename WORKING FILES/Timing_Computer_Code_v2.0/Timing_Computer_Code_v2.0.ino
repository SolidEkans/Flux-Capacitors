int64_t TPmS;             //Present period of Ignition events per microsecond of Engine
int64_t Dwell;            //The charge time for the coil in microseconds
double IGNDel;            //The ignition timing delay in degrees
int64_t DelayT;           //Delay of ignition timing in microseconds
int64_t Delay;            //Delay for Dwell Start
unsigned long systemCLK;  //the previous value of the system clock
unsigned long temp;       //temprary variable for getting system clock
bool beginDel;

void delayMicSeconds(int64_t);

void toggle(){
  beginDel = 1;
}

void setup() {
  // put your setup code here, to run once:
  TPmS = 0;
  Dwell = 3600;
  IGNDel = 5;
  beginDel = 0;
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), toggle, FALLING);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(beginDel){
    temp = micros();
    TPmS = temp - systemCLK;
    systemCLK = temp;
    beginDel = 0;
    
    DelayT = (IGNDel / 90) * TPmS;
    if(DelayT <= Dwell){
      delayMicSeconds(DelayT-200);
      digitalWrite(13, LOW);  
      Delay = TPmS - Dwell;
      delayMicSeconds(Delay-50);
      digitalWrite(13, HIGH);      
    }else{
      Delay = DelayT - Dwell;
      
      //Delays until proper dwell start time
      delayMicSeconds(Delay-100);
      digitalWrite(13, HIGH);
      delayMicSeconds(Dwell-100);
      digitalWrite(13, LOW);
    }
  }
}

void delayMicSeconds(int64_t del){
  while(del > 16383){
    delayMicroseconds(16383);
    del -= 16383;    
  }
  delayMicroseconds(del);
}



