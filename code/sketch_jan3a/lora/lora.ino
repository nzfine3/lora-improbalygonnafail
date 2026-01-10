#include <Servo.h>

Servo myservo;
Servo myservo2;

int posx = 0;
int posy = 0;

void setup() {
  myservo.attach(9); 
  myservo2.attach(10);
}
// vrx -> A5
// vry -> A4
void loop() {
  posx = map(analogRead(5), 0, 1023, 0, 180);
  posy = map(analogRead(4), 0, 1023, 0, 180);
  myservo.write(posx);
  myservo2.write(posy);
  delay(10);
}
