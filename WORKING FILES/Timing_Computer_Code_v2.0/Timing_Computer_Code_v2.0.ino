double TPmS;              //Present period of Ignition events per millisecond of Engine
double Dwell;             //The charge time for the coil in milliseconds
double IGNDel;            //The ignition timing delay in degrees
double DelayT;            //Delay of ignition timing in milliseconds
double Delay;             //Delay for Dwell Start
unsigned long systemCLK;  //the previous value of the system clock
unsigned long temp;       //temprary variable for getting system clock
bool beginDel;

void toggle(){
  beginDel = 1;
}

void setup() {
  // put your setup code here, to run once:
  TPmS = 0;
  Dwell = 3.6;
  IGNDel = 15;
  beginDel = 0;
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), toggle, FALLING);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(beginDel){
    temp = micros();
    TPmS = ((double)temp - (double)systemCLK) / 1000;
    systemCLK = temp;
    beginDel = 0;
    
    DelayT = 1.05 * (IGNDel / 90.0) * TPmS;
    if(DelayT <= Dwell){
      delay(DelayT);
      digitalWrite(13, LOW);  
      Delay = TPmS - Dwell;
      delay(Delay);
      digitalWrite(13, HIGH);      
    }else{
      Delay = DelayT - Dwell;
      
      //Delays until proper dwell start time
      delay(Delay);
      digitalWrite(13, HIGH);
      delay(Dwell);
      digitalWrite(13, LOW);
    }
  }
}
