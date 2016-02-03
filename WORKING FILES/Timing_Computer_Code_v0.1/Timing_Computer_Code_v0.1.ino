int TPmS;                 //Present period of Ignition events per microsecond of Engine
int Dwell;                //The charge time for the coil
int IGN_Tim;              //The ignition timing
unsigned long systemCLK;  //the previous value of the system clock
unsigned long temp;       //temprary variable for getting system clock

void toggle(){
  temp = micros();
  TPmS = temp-systemCLK;
  systemCLK = temp;
}

void setup() {
  // put your setup code here, to run once:
  TPmS = 0;
  Dwell = 2500;
  IGN_Tim = -20;
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), toggle, FALLING);
}

void loop() {
  // put your main code here, to run repeatedly:
  
}



