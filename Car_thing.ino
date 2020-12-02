#include <ECE3.h>

uint16_t senVals[8]; // right -> left, 0 -> 7
const int lNP=31, lDP=29, lPP=40; // nslp ==> awake & ready for PWM
const int rNP=11, rDP=30, rPP=39; // nslp ==> awake & ready for PWM
const int off[8]={640, 710, 664, 528, 595, 710, 710, 757};
const int wts[8]={8, 4, 2, 1, -1, -2, -4, -8};
const int spd=60;
bool turned=0;

///////////////////////////////////
void setup() {
// put your setup code here, to run once:
  pinMode(lNP,OUTPUT);
  pinMode(lDP,OUTPUT);
  pinMode(lPP,OUTPUT);

  digitalWrite(lDP,LOW);
  digitalWrite(lNP,HIGH);

  pinMode(rNP,OUTPUT);
  pinMode(rDP,OUTPUT);
  pinMode(rPP,OUTPUT);

  digitalWrite(rDP,LOW);
  digitalWrite(rNP,HIGH);

  ECE3_Init();

  delay(2000); //Wait 2 seconds before starting 
}

void wpin(int l, int r, int d=0) {
  analogWrite(lPP,l);
  analogWrite(rPP,r);
  delay(d);
}

void loop() {
  ECE3_read_IR(senVals);
  int err=0, amt, lEps=300, rEps=-300, turn=0;
  for (int i=0; i<8; i++) {
    err+=wts[i]*map(senVals[i]-off[i],0,2500-off[i],0,1000);
    turn+=senVals[i];
  }
  if (turn>16000)
    if (!turned) {
      wpin(0,0,100); digitalWrite(lDP,HIGH);
      wpin(60,60,1130); digitalWrite(lDP,LOW);
      wpin(40,40,300); turned=1;
    } else wpin(0,0,100000000);
  if (err<rEps) {
    amt=rEps-err;
    wpin(spd-amt/10,spd+amt/30);
  } else if (err>lEps) {
    amt=err-lEps;
    wpin(spd+amt/30,spd-amt/10);
  } else wpin(spd,spd);
}
