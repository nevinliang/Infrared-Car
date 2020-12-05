#include <ECE3.h>

uint16_t senVals[8]; // right -> left, 0 -> 7
const int leftNslpPin=31, leftDirPin=29, leftPWMPin=40; // nslp ==> awake & ready for PWM
const int rightNslpPin=11, rightDirPin=30, rightPWMPin=39; // nslp ==> awake & ready for PWM
const int LED_RF = 41;
// const int offset[8]={640, 710, 664, 528, 595, 710, 710, 757};
const int offset = 1900;
// const int weight[8]={8, 4, 2, 1, -1, -2, -4, -8}; // for sensor fusion
const int weight[8] = {-334, -238, -142, -48, 48, 142, 238, 334};
const int speed=20; // base motor speed (doesn't work for 50)
bool turned=0;
const int turnDelay = 1130;
const int turnSpeed = 60;
int error = 0, delta = 0, steer = 0, lEps=300, rEps=-300, senSum = 0;

///////////////////////////////////
void setup() {
// put your setup code here, to run once:
  pinMode(leftNslpPin,OUTPUT);
  pinMode(leftDirPin,OUTPUT);
  pinMode(leftPWMPin,OUTPUT);

  digitalWrite(leftDirPin,LOW);
  digitalWrite(leftNslpPin,HIGH);

  pinMode(rightNslpPin,OUTPUT);
  pinMode(rightDirPin,OUTPUT);
  pinMode(rightPWMPin,OUTPUT);

  digitalWrite(rightDirPin,LOW);
  digitalWrite(rightNslpPin,HIGH);

  pinMode(LED_RF, OUTPUT);
  digitalWrite(LED_RF, LOW);

  ECE3_Init();

  Serial.begin(9600); // set the data rate in bits per second for serial data transmission

  //Wait 2 seconds before starting
  for(int k=0;k<3;k++){  
  digitalWrite(LED_RF, HIGH);
  delay(400);
  digitalWrite(LED_RF, LOW);
  delay(100);
  }
}

void wpin(int l, int r, int d=0) {
  analogWrite(leftPWMPin,l);
  analogWrite(rightPWMPin,r);
  delay(d);
}

void loop() {
  ECE3_read_IR(senVals);
  delta = error;

  senSum = 0;
  for (int i=0; i<8; i++) {
//    Serial.print(senVals[i]+offset);
//    Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
    senVals[i] = map(senVals[i]+offset,0,2500,0,1);
    //Serial.print(senVals[i]);
    //Serial.print('\t'); // tab to format the raw data into columns in the Serial monitor
    error += weight[i]*senVals[i]; // distance off line in mm (right) positive
    senSum += senVals[i];
  }
  //Serial.println();
  
  error = error/senSum;
  delta = error - delta;
  
  if (senSum>3)
    if (!turned) {
      wpin(0,0,100); 
      digitalWrite(leftDirPin,HIGH); // reverse left motor
      wpin(turnSpeed,turnSpeed,turnDelay); 
      digitalWrite(leftDirPin,LOW); // left motor back to forward
      wpin(40,40,300);
      turned = 1;
    } else {
      digitalWrite(LED_RF, HIGH); 
      wpin(0,0,100000000);
    }

    steer = speed*error/334;
    wpin(speed-steer,speed+steer,10);
    
//  if (err<rEps) {
//    amt=rEps-err;
//    wpin(speed-amt/10,speed+amt/30);
//  } else if (err>lEps) {
//    amt=err-lEps;
//    wpin(speed+amt/30,speed-amt/10);
//  } else wpin(speed,speed);
}
