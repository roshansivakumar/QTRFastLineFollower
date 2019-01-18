#include <QTRSensors.h>

#define Kp 0 
#define Kd 0 
#define RMaxSpeed 300 
#define LMaxSpeed 300 
#define RBaseSpeed 250 
#define LBaseSpeed 250  
#define NUM_SENSORS  8  
#define TIMEOUT    2000  
#define EMITTER_PIN   2     

#define RM1 3
#define RM2 4
#define RMPWM 5
#define LM1 12
#define LM2 13
#define LMPWM 11
#define MP 8

QTRSensorsRC qtrrc((unsigned char[]) 
{
  A0, A1, A2, A3, A4, A5, A6, A7
} , NUM_SENSORS, TIMEOUT, EMITTER_PIN); 
unsigned int sensorValues[NUM_SENSORS];

void setup()
{
  pinMode(RM1, OUTPUT);
  pinMode(RM2, OUTPUT);
  pinMode(RMPWM, OUTPUT);
  pinMode(LM1, OUTPUT);
  pinMode(LM2, OUTPUT);
  pinMode(LMPWM, OUTPUT);
  pinMode(MP, OUTPUT);

  int i;
  for (int i = 0; i < 100; i++) // calibration
    qtrrc.calibrate();
  delay(20);
  wait();
  delay(5000); 

int prevErr = 0;

void loop()
{
  unsigned int sensors[8];
  int position = qtrrc.readLine(sensors,QTR_EMITTERS_ON,1); 
  int error = position - 3500;

  int motorSpeed = Kp * error + Kd * (error - prevErr);
  prevErr = error;

  int RMSpeed = RBaseSpeed + motorSpeed;
  int LMSpeed = LBaseSpeed - motorSpeed;

  if (RMSpeed > RMaxSpeed ) RMSpeed = RMaxSpeed; 
  if (LMSpeed > LMaxSpeed ) LMSpeed = LMaxSpeed;
  if (RMSpeed < 0) RMSpeed = 0; 
  if (LMSpeed < 0) LMSpeed = 0;

  {
    digitalWrite(MP, HIGH); 
    digitalWrite(RM1, HIGH);
    digitalWrite(RM2, LOW);
    analogWrite(RMPWM, RMSpeed);
    digitalWrite(MP, HIGH);
    digitalWrite(LM1, HIGH);
    digitalWrite(LM2, LOW);
    analogWrite(LMPWM, LMSpeed);
  }
}

void wait() {
  digitalWrite(MP, LOW);
}
