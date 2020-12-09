#include <ECE3.h>

uint16_t sen[8];
const int lNP=31, lDP=29, lPP=40, rNP=11, rDP=30, rPP=39, LED=41;
const int of[8]={598, 691, 644, 529, 574, 667, 667, 667};
const int wt[8]={8, 4, 2, 1, -1, -2, -4, -8};
int sp=110, lE, rE;
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

    pinMode(LED, OUTPUT);
    
    ECE3_Init();
    Serial.begin(9600);
    
    for(int k=0;k<3;k++){  
        digitalWrite(LED,HIGH);
        delay(400);
        digitalWrite(LED,LOW);
        delay(100);
    }
    wpin(0,0,1,100);
}

int c=0,tot=0,avg;

void spin() {
    c=tot=0;
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
    
    if(sen[0]+sen[7]>3600)
        (!t)?spin():wpin(0,0,0,10000000);
        
    else {
        if (er < 1000 && er > -1000) {
            c++;
            tot+=er;
            avg=tot/c;
            lE=avg+150;
            rE=avg-150;
        }
        else {
            lE=300;
            rE=-500;
        }
    }
    
    if(er<rE)       wpin(-(rE-er>>4),rE-er>>5,1);
    else if(er>lE)  wpin(er-lE>>5,-(er-lE>>4),1);
    else            wpin(0,0,1);
}
