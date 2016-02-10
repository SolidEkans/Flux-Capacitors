double TPmS;              //Present period of Ignition events per microsecond of Engine
double Dwell;             //The charge time for the coil in micro seconds
double IGNTim;            //The ignition timing in degrees
double Delay;             //Delay for Dwell Start
unsigned long systemCLK;  //the previous value of the system clock
unsigned long temp;       //temprary variable for getting system clock
bool beginDel;

void toggle(){
  beginDel = 1;
  //digitalWrite(13, HIGH);
  //delayMicroseconds(1000);
  //digitalWrite(13, LOW);
}

void setup() {
  // put your setup code here, to run once:
  TPmS = 0;
  Dwell = 3600;
  IGNTim = -20;
  beginDel = 0;
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), toggle, RISING);
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(beginDel){
  temp = micros();
  TPmS = ((double)temp - (double)systemCLK);
  systemCLK = temp;
  beginDel = 0;

  Delay = (TPmS) + (0.79*(((IGNTim + 10.0) / 90.0) * TPmS)) - (Dwell);
  //Delay = Delay - (double)(micros() - systemCLK);

  //Delays until proper dwell start time
  delayMicroseconds(Delay);
  digitalWrite(13, HIGH);
  delayMicroseconds(Dwell);
  digitalWrite(13, LOW);
  }
}
