#include <AFMotor.h>          //library header motor dc
#include <NewPing.h>          //library header deklarasi new ping untuk komunikasi serial
#include <Servo.h>            //library header untuk motor servo
#define echoPin A1            //definisi pin echo pada pin analog A1
#define trigPin A0            //defenisi pin trig pada pin analog A0
#define MAX_DISTANCE 200      //definisi jarak maksimal dari sensor ultrasonic 
#define MAX_SPEED 200         //definisi kecepatan maksimal dari motor dc
#define MAX_SPEED_OFFSET 20   //definisi batas kecepatan motor dc

NewPing sonar(trigPin, echoPin, MAX_DISTANCE);  //deklarasi fungsi newping untuk penggunaan trigpin dan echopin terhadap max jarak
AF_DCMotor motor1(1, MOTOR12_1KHZ);   //penggunaan modulpin 1 untuk motor 1
AF_DCMotor motor2(2, MOTOR12_1KHZ);   //penggunaan modulpin 2 untuk motor 2
AF_DCMotor motor3(3, MOTOR34_1KHZ);   //penggunaan modulpin 3 untuk motor 3
AF_DCMotor motor4(4, MOTOR34_1KHZ);   //penggunaan modulpin 4 untuk motor 4
Servo myservo;    //variable nilai servo

unsigned long waktuSebelum=0; //deklarasi variable waktusebelum
boolean goesForward=false;    //deklarasi variable bolean goesForward
int distance = 100;           //deklarasi variable distace
int speedSet = 0;             //deklarasi variable speedSet
char command;                 //deklarasi karakter command

void setup()              //deklarsi fungsi operasi sekali
{       
  Serial.begin(9600);     //penggunaan serial.begin=9600
  pinMode(A2, OUTPUT);    //pin mode A2 sebagai output
  pinMode(A3, OUTPUT);    //pin mode A3 sebagai output
  pinMode(A4, OUTPUT);    //pin mode A4 sebagai output
  pinMode(A5, INPUT);     //pin mode A5 sebagai input
  myservo.attach(10);     //output motor servo pada pin 10
  myservo.write(90);      //penempatan sudut motor servo pada -+ 90 derajat
  delay(2000);            //waktu tunda hingga 2000ms
  distance = readPing();  //baca pin untuk variable distance
  delay(100);             //waktu tunda hingga 100ms
  distance = readPing();  //baca pin untuk variable distance
  delay(100);             //waktu tunda hingga 100ms
  distance = readPing();  //baca pin untuk variable distance
  delay(100);             //waktu tunda hingga 100ms
  distance = readPing();  //baca pin untuk variable distance
  delay(100);             //waktu tunda hingga 100ms
}

void loop()              //deklarsi fungsi operasi sekali
{
  if(Serial.available() > 0){ //baca nilai input dari modul bluetooth
    command = Serial.read();  //value dari command adalah nilai dari serial.read bluetooth
    Stop();                   //release motor dc dengan nilai 0
    switch(command){          //input kondisi variable command
    case 'F':                 //deklarasi char F untuk aktivitas robot maju
      Auto();
      break;
    case 'G':                 //deklarasi char G untuk aktivitas robot maju kekiri
      fleft();
      break;
    case 'I':                 //deklarasi char I untuk aktivitas robot maju kekanan
      fright();
      break;
    case 'H':                 //deklarasi char H untuk aktivitas robot mundur kekiri
      bleft();
      break;
    case 'J':                 //deklarasi char J untuk aktivitas robot mundur kekanan
      bright();
      break;
    case 'B':                 //deklarasi char B untuk aktivitas robot mundur 
      back();
      break;
    case 'L':                 //deklarasi char L untuk aktivitas robot kekiri  
      left();
      break;
    case 'R':                 //deklarasi char R untuk aktivitas robot kekanan
      right();
      break;
    case 'W':                 //deklarasi char W untuk aktivitas robot led putih aktif
      lighton();
      break;
    case 'w':                 //deklarasi char w untuk aktivitas robot led putih nonaktif
      lightoff();
      break;
    case 'U':                 //deklarasi char U untuk aktivitas robot led kuning aktif
      backon();
      break;
    case 'u':                 //deklarasi char u untuk aktivitas robot led kuning nonaktif
      backoff();
      break;
    case 'v':                 //deklarasi char v untuk aktivitas robot buzzer aktif
      bunyi();
      break;
    case 'V':                 //deklarasi char V untuk aktivitas robot buzzer nonaktif
      mati();
      break;
    case 'X':                 //deklarasi char X untuk aktivitas robot sistem alert aktif
      Alert();
      break;
    case 'q':                 //deklarasi char q untuk aktivitas robot arah sensor ultasonic 180
      A180();
      break;
    case '9':                 //deklarasi char 9 untuk aktivitas robot arah sensor ultasonic 160
      A160();
      break;
    case '8':                 //deklarasi char 8 untuk aktivitas robot arah sensor ultasonic 140
      A140();
      break;
    case '7':                 //deklarasi char 7 untuk aktivitas robot arah sensor ultasonic 120
      A120();
      break;
    case '6':                 //deklarasi char 6 untuk aktivitas robot arah sensor ultasonic 100
      A100();
      break;
    case '5':                 //deklarasi char 5 untuk aktivitas robot arah sensor ultasonic 80
      A80();
      break;
    case '4':                 //deklarasi char 4 untuk aktivitas robot arah sensor ultasonic 60
      A60();
      break;
    case '3':                 //deklarasi char 3 untuk aktivitas robot arah sensor ultasonic 40
      A40();
      break;
    case '2':                 //deklarasi char 2 untuk aktivitas robot arah sensor ultasonic 20
      A20();
      break;
    case '1':                 //deklarasi char 1 untuk aktivitas robot arah sensor ultasonic 0
      A00();
      break;
    case '0':                 //deklarasi char 0 untuk aktivitas robot arah sensor ultasonic 90 (release)
      myservo.write(90);
      break;
    }
    }
}

void beb(){                   //deklarasi fungsi beb
    unsigned long waktuSekarang=millis(); //deklarasi variable waktusekarang
    if(waktuSekarang-waktuSebelum>=2700){ //jika waktusekarang dikurang waktusebelum melebihi 2700ms maka
      pinMode(A5, OUTPUT);
      delay(45);
      pinMode(A5, INPUT);
      delay(45);
      pinMode(A5, OUTPUT);
      delay(45);
      pinMode(A5, INPUT);
      waktuSebelum=millis();
  }
}

void Auto(){                  //deklarasi fungsi Auto
       int distanceR = 0;     //variable distaceR = 0
       int distanceL = 0;     //variable distaceL = 0
       if(distance<=37)       //deklarasi kondisi jarak kurang dari  37cm
       {
        moveStop();
        Alert();
        delay(200);
        moveBackward();
        delay(200);
        moveStop();
        delay(200);
        distanceR = lookRight();
        distanceL = lookLeft();
       
        if(distanceR>=distanceL)
        {
          turnRight();
          Stop();
        }else
        {
          turnLeft();
          Stop();
        }
       }else
       {
        forward();
       }
       distance = readPing();
}

int lookRight()
{
      digitalWrite(A2, HIGH);
      digitalWrite(A4, LOW);
      myservo.write(0); 
      delay(500);
      int distance = readPing();
      myservo.write(90); 
      return distance;
}

int lookLeft()
{
      digitalWrite(A2, HIGH);
      digitalWrite(A4, LOW);
      myservo.write(180); 
      delay(500);
      int distance = readPing();
      myservo.write(90); 
      return distance;
}

int readPing() { 
      int cm = sonar.ping_cm();
      if(cm==0)
      {
        cm = 250;
      }
      return cm;
}

void Alert()
{
      for(int i=1;i<4;i++){
      digitalWrite(A2, HIGH);
      digitalWrite(A3, HIGH);
      digitalWrite(A4, HIGH);
      pinMode(A5, OUTPUT);
      myservo.write(180);
      delay(400);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
      digitalWrite(A4, LOW);
      pinMode(A5, INPUT);
      myservo.write(0);
      delay(400);
      }
      myservo.write(90);
      digitalWrite(A3, HIGH);
}

void forward()
{
      digitalWrite(A2, LOW);
      digitalWrite(A4, LOW);
      motor1.setSpeed(255); //Define maximum velocity
      motor1.run(FORWARD); //rotate the motor clockwise
      motor2.setSpeed(255); //Define maximum velocity
      motor2.run(FORWARD); //rotate the motor clockwise
      motor3.setSpeed(255);//Define maximum velocity
      motor3.run(FORWARD); //rotate the motor clockwise
      motor4.setSpeed(255);//Define maximum velocity
      motor4.run(FORWARD); //rotate the motor clockwise  
}

void fleft()
{
      digitalWrite(A2, LOW);
      digitalWrite(A4, LOW);
      motor1.setSpeed(40); //Define maximum velocity
      motor1.run(FORWARD); //rotate the motor clockwise
      motor2.setSpeed(40); //Define maximum velocity
      motor2.run(FORWARD); //rotate the motor clockwise
      motor3.setSpeed(255);//Define maximum velocity
      motor3.run(FORWARD); //rotate the motor clockwise
      motor4.setSpeed(255);//Define maximum velocity
      motor4.run(FORWARD); //rotate the motor clockwise  
}

void fright()
{
      digitalWrite(A2, LOW);
      digitalWrite(A4, LOW);
      motor1.setSpeed(255); //Define maximum velocity
      motor1.run(FORWARD); //rotate the motor clockwise
      motor2.setSpeed(255); //Define maximum velocity
      motor2.run(FORWARD); //rotate the motor clockwise
      motor3.setSpeed(40);//Define maximum velocity
      motor3.run(FORWARD); //rotate the motor clockwise
      motor4.setSpeed(40);//Define maximum velocity
      motor4.run(FORWARD); //rotate the motor clockwise  
}

void back()
{
      digitalWrite(A4, HIGH);
      digitalWrite(A2, LOW);
      motor1.setSpeed(255); //Define maximum velocity
      motor1.run(BACKWARD); //rotate the motor anti-clockwise
      motor2.setSpeed(255); //Define maximum velocity
      motor2.run(BACKWARD); //rotate the motor anti-clockwise
      motor3.setSpeed(255); //Define maximum velocity
      motor3.run(BACKWARD); //rotate the motor anti-clockwise
      motor4.setSpeed(255); //Define maximum velocity
      motor4.run(BACKWARD); //rotate the motor anti-clockwise
}

void bright()
{
      digitalWrite(A4, HIGH);
      digitalWrite(A2, LOW);
      motor1.setSpeed(255); //Define maximum velocity
      motor1.run(BACKWARD); //rotate the motor anti-clockwise
      motor2.setSpeed(255); //Define maximum velocity
      motor2.run(BACKWARD); //rotate the motor anti-clockwise
      motor3.setSpeed(40); //Define maximum velocity
      motor3.run(BACKWARD); //rotate the motor anti-clockwise
      motor4.setSpeed(40); //Define maximum velocity
      motor4.run(BACKWARD); //rotate the motor anti-clockwise
}

void bleft()
{
      digitalWrite(A4, HIGH);
      digitalWrite(A2, LOW);
      motor1.setSpeed(40); //Define maximum velocity
      motor1.run(BACKWARD); //rotate the motor anti-clockwise
      motor2.setSpeed(40); //Define maximum velocity
      motor2.run(BACKWARD); //rotate the motor anti-clockwise
      motor3.setSpeed(255); //Define maximum velocity
      motor3.run(BACKWARD); //rotate the motor anti-clockwise
      motor4.setSpeed(255); //Define maximum velocity
      motor4.run(BACKWARD); //rotate the motor anti-clockwise
}

void left()
{
      digitalWrite(A4, LOW);
      digitalWrite(A2, LOW);
      motor1.setSpeed(255); //Define maximum velocity
      motor1.run(BACKWARD); //rotate the motor anti-clockwise
      motor2.setSpeed(255); //Define maximum velocity
      motor2.run(BACKWARD); //rotate the motor anti-clockwise
      motor3.setSpeed(255); //Define maximum velocity
      motor3.run(FORWARD);  //rotate the motor clockwise
      motor4.setSpeed(255); //Define maximum velocity
      motor4.run(FORWARD);  //rotate the motor clockwise
}

void right()
{
      digitalWrite(A4, LOW);
      digitalWrite(A2, LOW);
      motor1.setSpeed(255); //Define maximum velocity
      motor1.run(FORWARD); //rotate the motor clockwise
      motor2.setSpeed(255); //Define maximum velocity
      motor2.run(FORWARD); //rotate the motor clockwise
      motor3.setSpeed(255); //Define maximum velocity
      motor3.run(BACKWARD); //rotate the motor anti-clockwise
      motor4.setSpeed(255); //Define maximum velocity
      motor4.run(BACKWARD); //rotate the motor anti-clockwise
} 

void Stop()
{
      beb();
      digitalWrite(A2, HIGH);
      motor1.setSpeed(0); //Define minimum velocity
      motor1.run(RELEASE); //stop the motor when release the button
      motor2.setSpeed(0); //Define minimum velocity
      motor2.run(RELEASE); //rotate the motor clockwise
      motor3.setSpeed(0); //Define minimum velocity
      motor3.run(RELEASE); //stop the motor when release the button
      motor4.setSpeed(0); //Define minimum velocity
      motor4.run(RELEASE); //stop the motor when release the button
}

void moveStop() {
      digitalWrite(A2, HIGH);
      digitalWrite(A4, LOW);
      digitalWrite(A3, HIGH);
      motor1.run(RELEASE); 
      motor2.run(RELEASE);
      motor3.run(RELEASE);
      motor4.run(RELEASE);
} 
  
void turnRight() {
      motor1.setSpeed(255);
      motor1.run(FORWARD);
      motor2.setSpeed(255);
      motor2.run(FORWARD);
      motor3.setSpeed(255);
      motor3.run(BACKWARD);
      motor4.setSpeed(255);
      motor4.run(BACKWARD);     
      delay(350);    
} 
 
void turnLeft() {
      motor1.setSpeed(255);
      motor1.run(BACKWARD);
      motor2.setSpeed(255);
      motor2.run(BACKWARD);
      motor3.setSpeed(255);
      motor3.run(FORWARD);
      motor4.setSpeed(255);
      motor4.run(FORWARD);     
      delay(350);
}

void moveBackward() {
      digitalWrite(A4, HIGH);
      digitalWrite(A2, LOW);
      digitalWrite(A3, LOW);
      goesForward=false;
      motor1.setSpeed(200);
      motor1.run(BACKWARD);
      motor2.setSpeed(200);      
      motor2.run(BACKWARD);
      motor3.setSpeed(200);
      motor3.run(BACKWARD);
      motor4.setSpeed(200);
      motor4.run(BACKWARD);  
      delay(350);
}

//3 Lampu
void lighton(){
    digitalWrite(A3, HIGH);
}

void lightoff(){
    digitalWrite(A3, LOW);
}

void backon(){
    digitalWrite(A4, HIGH);
}

void backoff(){
    digitalWrite(A4, LOW);
}

//Buzzer
void bunyi(){
    pinMode(A5, INPUT);
}

void mati(){
    pinMode(A5, OUTPUT);
}

//Arah Kepala
void A00(){
    myservo.write(180);
}
void A20(){
    myservo.write(160);
}
void A40(){
    myservo.write(140);
}
void A60(){
    myservo.write(120);
}
void A80(){
    myservo.write(100);
}
void A100(){
    myservo.write(80);
}
void A120(){
    myservo.write(60);
}
void A140(){
    myservo.write(40);
}
void A160(){
    myservo.write(20);
}
void A180(){
    myservo.write(0);
}
