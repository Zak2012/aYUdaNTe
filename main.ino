// C++ code
//
///////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////
////                                                                                       ////
////    RoboSInC 2021                                                                      ////
////                                                                                       ////
////    Smart vacuum Cleaner -                                                             ////
////    https://www.tinkercad.com/things/9JCeAIEu0vn-smart-vacuum-cleaner                  ////
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

//define motor pin
#define Lmotor1 13
#define Lmotor2 12
#define Rmotor1 9
#define Rmotor2 8

//define sensor pin
#define trigl 7
#define echol 6
#define trigc 5
#define echoc 4
#define trigr 3
#define echor 2

//define buzzer pin
#define buzz 11

//define servo pin
#define serv 10

// initialize servo object
Servo servo;

// define global variable
// define sensor trigger distance in cm
int trigDist = 5;

// store servo rotation
int rot = 0;

// store random number
long chance;

// define chances of spraying water in percentage
int sprayc = 20;

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
  
  // getnoises for random number generator
  randomSeed(analogRead(0));
}

// run forever
void loop(){
  
  // generate random number
  	chance = random(100);
  
  // check the chances of spraying every loop
  if(chance <= sprayc){

    // calls the spray method
    spray();
  }

  // wait until finish spray
  delay(sstep*360);

  // get distacne from each ultrasonic sensor
  int distancel = findDistance(trigl,echol);
  int distancec = findDistance(trigc,echoc);
  int distancer = findDistance(trigr,echor);

  // print distance value from all sensor for debugging purpose
  Serial.print("Lsensor : ");
  Serial.print(distancel);
  Serial.print("  Csensor : ");
  Serial.print(distancec);
  Serial.print("  Rsensor : ");
  Serial.println(distancer);

  // sets condition for bot when there are wall blocking
  /*          ■
  / - bot     ■
  / ▢▢▢▢    ■ 
  / ▢▢▢▢    ■ - walls
  / ▢▢▢▢    ■
  /           ■
  /           ■          */

  if(distancec <= trigDist){

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

void spray(){

  // servo rotate from 0 degree to 180 degree
  for (rot = 0; rot <= 180; rot += 1) {

    // servo rotate to `rot`
    servo.write(rot);

    // wait 15 milisecond before looping
    delay(5);

  }

  // servo rotate from 180 degree to 0 degree
  for (rot = 180; rot >= 0; rot -= 1) {

    // servo rotate to `rot`
    servo.write(rot);

    // wait 15 milisecond before looping
    delay(5);

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
  pinMode(trigl, OUTPUT);
  pinMode(echol, INPUT);
  pinMode(trigc, OUTPUT);
  pinMode(echoc, INPUT);
  pinMode(trigr, OUTPUT);
  pinMode(echor, INPUT);

  // initialize buzzer pin
  pinMode(buzz, OUTPUT);

  // initialize servo pin
  servo.attach(serv);
}
