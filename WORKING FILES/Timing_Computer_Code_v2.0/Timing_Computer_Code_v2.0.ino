int64_t TPmS;             //Present period of Ignition events per microsecond of Engine
int64_t Dwell;            //The charge time for the coil in microseconds
float IGNDel;            //The ignition timing delay in degrees
int64_t DelayT;           //Delay of ignition timing in microseconds
int64_t Delay;            //Delay for Dwell Start
unsigned long systemCLK;  //the previous value of the system clock
unsigned long temp;       //temprary variable for getting system clock
bool beginDel;            //this boolean variable is triggered when the ignition 
                          //enable signal has reached a negative edge

const int TimingOffset = 36;   //this is the base timing offset on the distributor
      
                             /*0    400  800  1200 1600 2000 2400 2800 3200 3600 4000 4400 4800 5200 5600 6000  RPM*/
int SparkMAP[19][16] = /*15*/{{20,  23,  23,  25,  26,  27,  28,  29,  29,  29,  29,  29,  29,  29,  29,  29},
                       /*20*/ {20,  23,  23,  25,  27,  28,  29,  30,  30,  30,  30,  30,  30,  30,  30,  30},
                       /*25*/ {20,  23,  23,  26,  27,  29,  30,  32,  32,  32,  32,  32,  32,  32,  32,  32},
                       /*30*/ {20,  23,  23,  26,  28,  30,  32,  34,  34,  34,  34,  34,  34,  34,  34,  34},
                       /*35*/ {20,  23,  23,  27,  29,  32,  34,  36,  35,  35,  35,  34,  34,  34,  34,  34},
                       /*40*/ {20,  23,  23,  28,  32,  34,  36,  36,  36,  36,  35,  34,  34,  34,  34,  34},
                       /*45*/ {20,  21,  23,  28,  32,  34,  35,  36,  36,  36,  35,  34,  34,  34,  34,  34},
                       /*50*/ {18,  18,  21,  28,  32,  33,  34,  35,  36,  36,  35,  34,  34,  34,  34,  34},
                       /*55*/ {14,  14,  19,  26,  30,  32,  33,  34,  35,  35,  34,  33,  33,  33,  33,  33},
                       /*60*/ {12,  12,  16,  24,  28,  31,  33,  34,  33,  33,  32,  31,  29,  29,  29,  29},
                       /*65*/ {11,  11,  14,  21,  26,  29,  31,  32,  32,  32,  31,  29,  27,  27,  27,  27},
                       /*70*/ { 9,   9,  11,  18,  23,  27,  29,  31,  30,  30,  28,  26,  25,  25,  25,  25},
                       /*75*/ { 7,   7,   9,  15,  20,  25,  28,  30,  29,  28,  26,  24,  23,  23,  23,  23},
                       /*80*/ { 5,   5,   7,  13,  17,  23,  26,  28,  27,  25,  23,  21,  21,  21,  21,  21},
                       /*85*/ { 2,   2,   5,  11,  16,  22,  23,  25,  25,  24,  21,  20,  20,  20,  20,  20},
                       /*90*/ { 0,   0,   2,   5,  14,  19,  22,  22,  23,  22,  21,  20,  20,  20,  20,  20},
                       /*95*/ {-1,  -1,   0,   7,  12,  18,  21,  22,  21,  21,  21,  20,  20,  20,  20,  20},
                       /*100*/{-2,  -2,  -1,   5,  11,  16,  20,  21,  20,  20,  19,  19,  19,  19,  19,  19},
                       /*105*/{-2,  -2,  -1,   4,   9,  15,  18,  19,  19,  18,  18,  18,  18,  18,  18,  18}};
                       /*kPa*/

void delayMicSeconds(int64_t);

float TimingLookup();

void toggle(){
  beginDel = 1;
}

void setup() {
  // put your setup code here, to run once:
  TPmS = 0;
  Dwell = 3500;
  IGNDel = 0;
  beginDel = 0;
  pinMode(2, INPUT);
  attachInterrupt(digitalPinToInterrupt(2), toggle, FALLING);
  pinMode(1, OUTPUT);
  pinMode(10, OUTPUT);
  pinMode(8, INPUT);
  digitalWrite(10, HIGH);
}

void loop() {
  IGNDel = TimingLookup();

  if(IGNDel < 0){
    IGNDel = 0;
  }
  
  //if(digitalRead(8) == HIGH){
  //  IGNDel = 10;
  //}else{
  //  IGNDel = 0;
  //}
  
  if(beginDel){
    temp = micros();
    TPmS = temp - systemCLK;
    systemCLK = temp;
    
    DelayT = ((IGNDel * TPmS) / 90);
    
    //Make sure that we do not input a delay that is negative
    if(DelayT<110)
      DelayT = 110;
    
    //This handles the delay. There are two cases that take 
    //into account whether or not the dwell straddles the 
    //ignition triggering signal.
    if(DelayT <= Dwell){
      Delay = (TPmS - Dwell);
      
      delayMicSeconds(DelayT - 110);
      digitalWrite(1, LOW);  
      delayMicSeconds((Delay - 182));
      digitalWrite(1, HIGH);
    }else{
      Delay = (DelayT - Dwell);
      
      //Delays until proper dwell start time
      delayMicSeconds(Delay - 86);
      digitalWrite(1, HIGH);
      delayMicSeconds(Dwell - 36);
      digitalWrite(1, LOW);
    }
    beginDel = 0;
  }
}

//This function takes care of the issue with the delay microseconds 
//function. By repeating delay if the requested delay exceeds the 
//maximum accurate delay.
void delayMicSeconds(int64_t del){
  while(del > 16000){
    delayMicroseconds(16000);
    del = del - 16000;    
  }
  delayMicroseconds(del);
}

//this function looks up the desired timing based on RPM and MAP 
float TimingLookup(){
int RPM = (1000000 / (4 * TPmS));
int RPMindex = RPM / 400;
int ModRPM = RPM % 400;

int MAPVoltage = analogRead(A1);
int MAP;
int ModMAP;
int MAPindex;

float TempTiming1;
float TempTiming2;
float Timing;

//Transfer function for voltage to MAP
MAP = (MAPVoltage * (5.0 / 1023.0) * 26.25) - 3.125;
ModMAP = MAP % 5;
MAPindex = MAP / 5;

TempTiming1 = (float)SparkMAP[MAPindex][RPMindex] + (((float)SparkMAP[MAPindex][RPMindex + 1]-(float)SparkMAP[MAPindex][RPMindex]) * ((float)ModRPM / 400.0));
TempTiming2 = (float)SparkMAP[MAPindex + 1][RPMindex] + (((float)SparkMAP[MAPindex + 1][RPMindex + 1]-(float)SparkMAP[MAPindex + 1][RPMindex]) * ((float)ModRPM / 400.0));
Timing = (float)TimingOffset - ((float)TempTiming1 + (((float)TempTiming2 - (float)TempTiming1) * ((float)ModMAP / 5.0)));

return Timing;
}


