#include <Servo.h>
#include<SoftwareSerial.h>
#define echoPin 5 
#define trigPin 6

int bttx=9;    //tx of bluetooth module is connected to pin 9 of arduino
int btrx=10;    //rx of bluetooth module is connected to pin 10 of arduino
SoftwareSerial bluetooth(bttx,btrx);


 
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement

Servo myservo,x;  // create servo object to control a servo


int pos = 0; 

void setup() {
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
  pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT
  Serial.begin(9600);
  myservo.attach(7);
   x.attach(11);        // servo is connected to pin 11 of arduino
  bluetooth.begin(9600); 

}

void loop() {
 digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  duration = pulseIn(echoPin, HIGH);
 
  distance = duration * 0.034 / 2; 
  
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if(distance<10)
  {
    for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
    // in steps of 1 degree
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  for (pos = 180; pos >= 0; pos -= 1) { // goes from 180 degrees to 0 degrees
    myservo.write(pos);              // tell servo to go to position in variable 'pos'
    delay(15);                       // waits 15ms for the servo to reach the position
  }
  }
  delay(300);

   if(bluetooth.available()>0)    //if bluetooth module is transmitting data
  {
    int pos=bluetooth.read(); // store the data in pos variable
    Serial.println(pos);
    x.write(pos);             //move servo head to the given position
  }

}
