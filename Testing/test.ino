#include <Servo.h>
Servo sx,sy;
int pin=9;
void setup(){
  sx.attach(2);
  sy.attach(5);
  pinMode(pin, OUTPUT);
  digitalWrite(pin,HIGH);  
}
void loop(){
  sx.write(90);    //98 and 66 9610 9200 11540
  sy.write(104);    //104 and 73
  delay(5000);
  
  //delay(4000);
}
