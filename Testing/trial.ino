#include <Servo.h>
#include <SoftwareSerial.h>

Servo mx,my;
int tx(int,int);
int ty(int,int);
void movex(float,float);
void movey(float,float);
void moveon(float,float,float,float);
void moveoff(float,float,float,float);
char key,xich;
int pin=9,vxp=98,vxn=66,vyp=70,vyn=104;//txp=4850,txn=2700,typ=4050,tyn=4900;
int inix=1,iniy=1,xi=1,xf=1,yi=1,yf=1;
void setup(){
  mx.attach(2);
  my.attach(5);
  mx.write(90);
  my.write(90);
  pinMode(pin, OUTPUT);
Serial.begin(9600);  
}
void loop(){
  if(Serial.available()>4){
    key=Serial.read();Serial.println(key);
    xich = Serial.read();Serial.println(xich);
    xi=int(xich) - 96;
    yi=int(Serial.read()) - 48;
    xf=int(Serial.read()) - 96;
    yf=int(Serial.read()) - 48;
    if(key=='0'){
      moveoff(inix,iniy,xi,yi);
      digitalWrite(pin, HIGH);
      moveon(xi,yi,xf,yf);
      digitalWrite(pin, LOW);
      inix=xf;iniy=yf;
    }
    else{
      moveoff(inix,iniy,xf,yf);
      digitalWrite(pin, HIGH);
      if(xf>yf) moveon(xf,yf,9,(9-xf+yf));
      else moveon(xf,yf,9-yf+xf,9);
      digitalWrite(pin, LOW);
      if(xf>yf) moveon(9,(9-xf+yf),xi,yi);
      else moveon(9-yf+xf,9,xi,yi);
      digitalWrite(pin, HIGH);
      moveon(xi,yi,xf,yf);
      digitalWrite(pin, LOW);
      inix=xf;iniy=yf;
    }
  }
  mx.write(90);
  my.write(90);
  digitalWrite(pin, LOW);
  char chh=iniy+48;
  Serial.println(chh);
  delay(5000);
}
void moveon(float xi,float yi,float xf,float yf){
  int x=2*(xf-xi), y=2*(yf-yi);
  if(x==0 || y==0) {moveoff(xi,yi,xf,yf);return;}
  int sx,sy,sidex,sidey;
  if(x>0) {sx=vxp;sidex=1;}
  else {sx=vxn;sidex=-1;}
  if(y>0) {sy=vyp;sidey=1;}
  else {sy=vyn;sidey=-1;}
  x=abs(x);
  y=abs(y);
  if(x==y){
    if(x%2==0){
      movex(xi,xi+sidex*0.5);x-=1;xi=xi+sidex*0.5;
      movey(yi,yi+sidey*0.5);y-=1;yi=yi+sidey*0.5;
    }
    while(x!=1 && x!=0){
      movex(xi,xi+sidex*1);x-=2;xi=xi+sidex*1;
      movey(yi,yi+sidey*1);y-=2;yi=yi+sidey*1;
    }
    if(x==1){
      movex(xi,xi+sidex*0.5);x-=1;xi=xi+sidex*0.5;
      movey(yi,yi+sidey*0.5);y-=1;yi=yi+sidey*0.5;
    }
  }
  else{
    if(y>x){
      movex(xi,xi+sidex*0.5);x-=1;xi=xi+sidex*0.5;
      movey(yi,yf);y=0;
      movex(xi,xf);x=0;
    }
    else{
      movey(yi,yi+sidey*0.5);y-=1;yi=yi+sidey*0.5;
      movex(xi,xf);x=0;
      movey(yi,yf);y=0;
    }
  }
}
void moveoff(float xi,float yi,float xf,float yf){
  if(yi!=yf) movey(yi,yf);
  if(xi!=xf) movex(xi,xf);
}
void movex(float xi,float xf){
  float x=xf-xi;
  if(x>0){
    mx.write(vxp);
    while(x!=0){
      delay(tx(2*(xf-x),1));x-=0.5;
    }
    mx.write(90);
  }
  else{
    mx.write(vxn);
    while(x!=0){
      delay(tx(2*(xf-x),-1));x+=0.5;
    }
    mx.write(90);
  }
}
void movey(float xi,float xf){
  float x=xf-xi;
  if(x>0){
    my.write(vyp);
    while(x!=0){
      delay(ty(2*(xf-x),1));x-=0.5;
    }
    my.write(90);
  }
  else{
    my.write(vyn);
    while(x!=0){
      delay(ty(2*(xf-x),-1));x+=0.5;
    }
    my.write(90);
  }
}
int tx(int x,int side){
  switch(side){
    case 1 :
      switch(x){
        case 2  : return 9610;break;
        case 3  : return 9200;break;
        case 4  : return 11540;break;
        case 5  : return 14550;break;
        case 6  : return 12820;break;
        case 7  : return 11260;break;
        case 8  : return 8980;break;
        case 9  : return 12550;break;
        case 10 : return 9930;break;
        case 11 : return 12270;break;
        case 12 : return 9790;break;
        case 13 : return 11160;break;
        case 14 : return 11700;break;
        case 15 : return 17900;break;
        case 16 : return 26160;break;
        case 17 : return 13870;break;
      }break;
    case -1:
      switch(x){
        case 3  : return 2140;break;
        case 4  : return 2340;break;
        case 5  : return 2660;break;
        case 6  : return 1930;break;
        case 7  : return 2590;break;
        case 8  : return 2380;break;
        case 9  : return 2510;break;
        case 10 : return 2250;break;
        case 11 : return 2360;break;
        case 12 : return 2420;break;
        case 13 : return 2560;break;
        case 14 : return 2090;break;
        case 15 : return 2960;break;
        case 16 : return 3290;break;
        case 17 : return 3600;break;
        case 18 : return 4720;break;
      }break;
  }
}
int ty(int y,int side){
  switch(side){
    case 1 :
      switch(y){
        case 2  : return 3040;break;
        case 3  : return 3300;break;
        case 4  : return 1740;break;
        case 5  : return 2580;break;
        case 6  : return 2310;break;
        case 7  : return 2310;break;
        case 8  : return 2270;break;
        case 9  : return 2400;break;
        case 10 : return 2310;break;
        case 11 : return 1990;break;
        case 12 : return 2310;break;
        case 13 : return 2590;break;
        case 14 : return 1790;break;
        case 15 : return 1030;break;
        case 16 : return 2530;break;
        case 17 : return 2280;break;
      }break;
    case -1:
      switch(y){
        case 3  : return 12430;break;
        case 4  : return 10790;break;
        case 5  : return 11310;break;
        case 6  : return 10230;break;
        case 7  : return 11160;break;
        case 8  : return 8800;break;
        case 9  : return 11050;break;
        case 10 : return 11760;break;
        case 11 : return 13240;break;
        case 12 : return 8080;break;
        case 13 : return 13360;break;
        case 14 : return 9840;break;
        case 15 : return 10520;break;
        case 16 : return 7100;break;
        case 17 : return 8910;break;
        case 18 : return 14030;break;
      }break;
  }
}
