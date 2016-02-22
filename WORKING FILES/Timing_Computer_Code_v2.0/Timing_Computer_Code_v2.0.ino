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
  Dwell = 3500;
  IGNDel = 9;
  beginDel = 0;
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), toggle, FALLING);
  pinMode(1, OUTPUT);
}

void loop() {
  if(beginDel){
    temp = micros();
    TPmS = temp - systemCLK;
    systemCLK = temp;
    
    DelayT = ((IGNDel * TPmS) / 90);
    if(DelayT <= Dwell){
      Delay = (TPmS - Dwell);
      
      delayMicSeconds(DelayT);
      digitalWrite(1, LOW);  
      delayMicSeconds((Delay - 350));
      digitalWrite(1, HIGH);
    }else{
      Delay = (DelayT - Dwell);
      
      //Delays until proper dwell start time
      delayMicSeconds(Delay);
      digitalWrite(1, HIGH);
      delayMicSeconds(Dwell);
      digitalWrite(1, LOW);
    }
    beginDel = 0;
  }
}

void delayMicSeconds(int64_t del){
  while(del > 16000){
    delayMicroseconds(16000);
    del = del - 16000;    
  }
  delayMicroseconds(del);
}



