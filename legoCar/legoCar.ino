#include <SoftwareSerial.h>
#include <Adafruit_MotorShield.h>
#include <Wire.h>
#include <Servo.h>

SoftwareSerial HM10(0, 1); // RX = 0, TX = 1

char inChar;
String inData = "";

// Create the motor shield object with the default I2C address
Adafruit_MotorShield AFMS = Adafruit_MotorShield(); 
// Or, create it with a different I2C address (say for stacking)
// Adafruit_MotorShield AFMS = Adafruit_MotorShield(0x61); 

// Select which 'port' M1, M2, M3 or M4. In this case, M1
Adafruit_DCMotor *myMotor = AFMS.getMotor(1);
// You can also make another motor on port M2
//Adafruit_DCMotor *myOtherMotor = AFMS.getMotor(2);

Servo myServo;
const int SERVO_MIDDLE = 90;
const int SERVO_RIGHT = 10;
const int SERVO_LEFT = 170;

const int MOTOR_FORWARD_SPEED = 70;

const int SERVO_STEPS_COUNT = 3;

const int SERVO_STEP = (SERVO_LEFT - SERVO_MIDDLE) / SERVO_STEPS_COUNT;

void setup()
{
  myServo.attach(9);
  myServo.write(90);  // set servo to mid-point
  
  Serial.begin(9600);
  Serial.println("HM10 serial started at 9600");
  
  HM10.begin(9600); // set HM10 serial at 9600 baud rate

  AFMS.begin();  // create with the default frequency 1.6KHz
  myMotor->setSpeed(150);
  myMotor->run(FORWARD);
  // turn on motor
  myMotor->run(RELEASE);
  
  inData = "";
}

void loop()
{
  // listen the HM10 port
  HM10.listen();  
  
  // if HM10 sends something then read
  while (HM10.available() > 0) 
  {   
    inChar = HM10.read();
    
    if (inChar == 's')
    {
      executeServoCommand(inData);
      inData = "";
    }
    else if (inChar == 'm')
    {
      executeMotorCommand(inData);
      inData = "";
    }
    else 
    {
      inData += (char)inChar;
    }
    
    Serial.write(inChar);
  }
 
  if (Serial.available()) 
  {           
    // Send serial data to HM10
    delay(10);
    HM10.write(Serial.read());
  }
  
  delay(2000);
}

void executeServoCommand(String command)
{
  int value = command.toInt();
  
  Serial.print("Command: ");
  Serial.println(value);
  
  myServo.write(SERVO_MIDDLE + SERVO_STEP * value);
}

void executeMotorCommand(String command)
{
  int value = command.toInt();

  Serial.print("Command: ");
  Serial.println(value);

  myMotor->run(FORWARD);
  myMotor->setSpeed(MOTOR_FORWARD_SPEED * value);
  
}
