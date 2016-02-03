int TPmS;                 //Present period of Ignition events per microsecond of Engine
double Dwell;             //The charge time for the coil in ms
int IGNTim;               //The ignition timing in degrees
int Delay;                //Delay for Dwell Start
unsigned long systemCLK;  //the previous value of the system clock
unsigned long temp;       //temprary variable for getting system clock
bool beginDel;

void toggle(){
  temp = micros();
  TPmS = temp-systemCLK;
  systemCLK = temp;
  beginDel = 1;
}

void setup() {
  // put your setup code here, to run once:
  TPmS = 0;
  Dwell = 3.6;
  IGNTim = -20;
  beginDel = 0;
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), toggle, FALLING);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  while(!beginDel){}
  beginDel = 0;

  Delay = TPmS + (((IGNTim + 10) * TPmS) / 90) - (Dwell / 1000);
  Delay = Delay - (micros() - systemCLK) - 2;

  //Delays until proper dwell start time
  delayMicroseconds(Delay);
  digitalWrite(13, HIGH);
  delayMicroseconds(Dwell * 1000);
  digitalWrite(13, LOW);
}
