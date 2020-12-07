#include <ECE3.h>

uint16_t sen[8];
const int lNP=31, lDP=29, lPP=40, rNP=11, rDP=30, rPP=39;
const int of[8]={598, 691, 644, 529, 574, 667, 667, 667};
const int wt[8]={8, 4, 2, 1, -1, -2, -4, -8};
const int sp=80, lE=-100, rE=-300;
bool t=0;

///////////////////////////////////

void wpin(int l, int r, bool rel, int d = 0) {
    analogWrite(lPP,l+(rel?sp:0));
    analogWrite(rPP,r+(rel?sp:0));
    delay(d);
}

void setup() {
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
    Serial.begin(9600);
    
    delay(2000);
    wpin(0,0,1,100);
}

void spin() {
    digitalWrite(lDP,HIGH);
    wpin(100,100,0,655);
    digitalWrite(lDP,LOW);
    t=1;
}

void loop() {
    ECE3_read_IR(sen);
    int er=0;
    for(int i=0;i<8;i++)
        er+=wt[i]*map(sen[i]-of[i],0,2500-of[i],0,1000);
    if(sen[0]+sen[7]>4000)
        (!t)?spin():wpin(0,0,0,10000000);
    if(er<rE)
        wpin(-(rE-er>>4+t),rE-er>>6+t,1);
    else if(er>lE)
        wpin(er-lE>>6+t,-(er-lE>>4+t),1);
    else
        wpin(0,0,1);
}
