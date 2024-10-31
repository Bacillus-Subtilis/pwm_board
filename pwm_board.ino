#include <Wire.h>
#include <timers.h>
#include "lcdAQ0802.h"

#define pwm1 1
#define pwm2 4
#define pwm3 5
#define ref1 0
#define ref2 2
#define ref3 3
#define display_rst 8
#define green 9
#define blue 10

bool wise = true;
short i=0;
unsigned short duty1,duty2,duty3;

void mozi(unsigned short duty,short num){
  char disp1[] = "        ";
  char disp2[] = "        ";
  disp1[0]= num + '0';
  disp1[4]= '0' + round(duty/1000);
  disp1[5]= '0' + round((duty%1000)/100);
  disp1[6]= '0' + round((duty%100)/10);
  disp1[7]= '0' + round(duty%10);
  disp2[0]= '0';
  disp2[1]= '.';
  disp2[2]= '0' + round(duty/250000);
  disp2[3]= '0' + round(duty/25000%10);
  disp2[4]= '0' + round(duty/2500%10);
  disp2[5]= '0' + round(duty/250%10);
  disp2[6]= '0' + round(duty/25%10);
  lcdClear();
  lcdInit();
  lcdDisp1(disp1);
  lcdDisp2(disp2);
  //delay(100);
}

void setup() {
  pinMode(ref1,INPUT);
  pinMode(ref2,INPUT);
  pinMode(ref3,INPUT);
  pinMode(pwm1,OUTPUT);
  pinMode(pwm2,OUTPUT);
  pinMode(pwm3,OUTPUT);
  pinMode(display_rst,OUTPUT);
  pinMode(green,OUTPUT);
  pinMode(blue,OUTPUT);
  digitalWrite(display_rst,HIGH);
  //delayMicroseconds(1000000);
  Wire.begin();
  //delayMicroseconds(1000);
  lcdInit();
  //delayMicroseconds(1000);
  lcdDisp1("Start");
  //delay(200);
  
  //delay(2000);
  
}

void loop() {
  duty1 = analogRead(ref1)*25/1024*100;
  duty2 = analogRead(ref2)*25/1024*100;
  duty3 = analogRead(ref3)*25/1024*100;
  
  //wise = !wise;

  //digitalWrite(green,wise);
  //digitalWrite(blue,wise);

  digitalWrite(pwm1,HIGH);
  digitalWrite(pwm2,HIGH);
  digitalWrite(pwm3,HIGH);

  long ntime = micros();
  short sa = 0;
  while(sa<2500){
    if (sa>duty1) digitalWrite(pwm1,LOW);
    if (sa>duty2) digitalWrite(pwm2,LOW);
    if (sa>duty3) digitalWrite(pwm3,LOW);
    sa = micros()-ntime;
  }
  digitalWrite(pwm1,LOW);
  digitalWrite(pwm2,LOW);
  digitalWrite(pwm3,LOW);
  delayMicroseconds(17500);

  
  i++;
  if (i%50==0){
    if (i<=50){
      mozi(duty1,1);
    }else if (i<=100){
      mozi(duty2,2);
    }else if (i<=150){
      mozi(duty3,3);
    }else i=0;
  }
  
}