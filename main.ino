// C++ code
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                       ////
////    RoboSInC 2021                                                                      ////
////                                                                                       ////
////    Smart vacuum Cleaner -                                                             ////
////    https://www.tinkercad.com/things/9JCeAIEu0vn                                       ////
////                                                                                       ////
////    ROBOS046 - SMK SULTAN ALAUDDIN - TINGKATAN 3 - WAN AMIRUL AIMAN BIN WAN HUSYAIDI   ////
////                                                                                       ////
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
//
//
// import library
#include <Servo.h>

// define pin number

//define sensor pin
#define trig 13
#define echo 12

//define buzzer pin
#define buzz 11

//define servo pin
#define serv 10

//define motor pin
#define Lmotor1 9
#define Lmotor2 8
#define Rmotor1 7
#define Rmotor2 6

// initialize servo object
Servo servo;

// define global variable
// define sensor trigger distance in cm
int trigDist = 10;

// define servo step time
int sstep = 5;

// run once
void setup(){

  // start the serial monitor for debugging
  Serial.begin(9600);

  // initialize pin
  pinInit();

  // startup sound
  tone(buzz,1000);
  delay(100);
  tone(buzz,500);
  delay(100);    
  noTone(buzz); 
  
}

// run forever
void loop(){
  
  // get distacne from front
  int distancec = findDistance(trig,echo);

  // sets condition for bot when there are wall blocking
  /*          ■
  / - bot     ■
  / ▢▢▢▢    ■ 
  / ▢▢▢▢    ■ - walls
  / ▢▢▢▢    ■
  /           ■
  /           ■          */

  if(distancec <= trigDist){

    // get distance from left and right
    // rotate servo to right
    rotate(0,90,1);
    delay(sstep*90);
    int distancer = findDistance(trig,echo);

    // rotate servo to left
    rotate(90,-90,-1);
    delay(sstep*180);
    int distancel = findDistance(trig,echo);

    // print distance value from all sensor for debugging purpose
    Serial.print("Lsensor : ");
    Serial.print(distancel);
    Serial.print("  Csensor : ");
    Serial.print(distancec);
    Serial.print("  Rsensor : ");
    Serial.println(distancer);


    // condition 1 : wall on front and right
    /*          ■
    / - bot     ■
    / ▢▢▢▢    ■ 
    / ▢▢▢▢    ■ - walls
    / ▢▢▢▢    ■
    /           ■
    / ■ ■ ■ ■ ■ ■          */

    if(distancer <= trigDist and distancel >= trigDist){

      // robot turn left
      move('l');

    }

    // condition 2 : wall on front and left
    /*■ ■ ■ ■ ■ ■
    / - bot     ■
    / ▢▢▢▢    ■ 
    / ▢▢▢▢    ■ - walls
    / ▢▢▢▢    ■
    /           ■
    /           ■           */

    else if(distancel <= trigDist and distancel >= trigDist){

      // robot turn right
      move('r');

    }
    
    // condition 3 : walls on unforseen location
    /*▦ ▦ ▦ ▦ ▦
    / - bot     ▦
    / ▢▢▢▢    ▦ 
    / ▢▢▢▢    ▦ - walls
    / ▢▢▢▢    ▦
    /           ▦
    / ▦ ▦ ▦ ▦ ▦          */

    else{

      // robot turn right
      move('r');

    }

  }
  else{

    // condition 4 : no walls
    /*          
    / - bot     
    / ▢▢▢▢     
    / ▢▢▢▢      - walls
    / ▢▢▢▢    
    /           
    /                       */
    // robot just moves forward
    move('f');

  }

  // robot continues moving/turning for 5 milisecond
  delay(5);

}


void move(char direction){

  // what robot gonna do when move(); was called
  // move('character');

  /* character :-
  / f - move forward
  / b - move backward
  / r - turn right
  / l - turn left
  / s - stop moving/turning
  */

  // switches cases
  switch (direction){

    // incase of 'f' it will move forward
    case 'f':
      digitalWrite(Rmotor1,HIGH);
      digitalWrite(Rmotor2,LOW);
      digitalWrite(Lmotor1,HIGH);
      digitalWrite(Lmotor2,LOW);
      break;

    // incase of 'b' it will move backward
    case 'b':
      digitalWrite(Rmotor1,LOW);
      digitalWrite(Rmotor2,HIGH);
      digitalWrite(Lmotor1,LOW);
      digitalWrite(Lmotor2,HIGH);
      break;

    // incase of 'r' it will turn right
    case 'r':
      digitalWrite(Rmotor1,LOW);
      digitalWrite(Rmotor2,HIGH);
      digitalWrite(Lmotor1,HIGH);
      digitalWrite(Lmotor2,LOW);
      break;

    // incase of 'l' it will turn left
    case 'l':
      digitalWrite(Rmotor1,HIGH);
      digitalWrite(Rmotor2,LOW);
      digitalWrite(Lmotor1,LOW);
      digitalWrite(Lmotor2,HIGH);
      break;

    // incase of 's' it will stop moving/turning
    case 's':
      digitalWrite(Rmotor1,LOW);
      digitalWrite(Rmotor2,LOW);
      digitalWrite(Rmotor1,LOW);
      digitalWrite(Rmotor2,LOW);
      break;
  }
}

// find distance for ultrasonic sensor
int findDistance(int trigpin,int echopin){

  // clear the trigger pin condition
  digitalWrite(trigpin, LOW);
  delayMicroseconds(2);

  // set the trigger HIGH for 10 microseconds
  digitalWrite(trigpin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigpin, LOW);

  // read the echopin, returns the sound wave travel time in microseconds
  long duration = pulseIn(echopin, HIGH);

  // calculate the distance in cm
  int distance = duration * 0.034 / 2;

  // return the distance
  return distance;

}

void rotate(int startrot,int rotation,int direction){

  // store current rotation
  int rot = 0;

  // servo rotate from current rotation to `rotation`
  for (rot = startrot; rot <= rotation; rot += direction) {

    // servo rotate to `rot`
    servo.write(rot);

    // wait `sstep` milisecond before looping
    delay(sstep);

  }
}

// initialize pin
void pinInit(){

  // initialize motor pin
  pinMode(Lmotor1, OUTPUT);
  pinMode(Lmotor2, OUTPUT);
  pinMode(Rmotor1, OUTPUT);
  pinMode(Rmotor2, OUTPUT);

  // initialize sensor pin
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  // initialize buzzer pin
  pinMode(buzz, OUTPUT);

  // initialize servo pin
  servo.attach(serv);
}
