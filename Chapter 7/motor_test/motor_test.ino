// Define motor pins
int motorOnePlus = 6;
int motorOneMinus = 7;
int motorOneEnable = 5;

int motorTwoPlus = 8;
int motorTwoMinus = 9;
int motorTwoEnable = 4;

void setup()
{

   Serial.begin(1152000);

   // Set pins
   pinMode(motorOnePlus, OUTPUT);
   pinMode(motorOneMinus, OUTPUT);
   pinMode(motorOneEnable, OUTPUT);

   pinMode(motorTwoPlus, OUTPUT);
   pinMode(motorTwoMinus, OUTPUT);
   pinMode(motorTwoEnable, OUTPUT);

}

void loop()
{

   // Accelerate forward
   setMotorOne(true, 500);
   setMotorTwo(true, 500);

   // Delay
   delay(5000);

   // Stop
   setMotorOne(true, 0);
   setMotorTwo(true, 0);

   // Delay
   delay(5000);

}

// Function to control the motor
void setMotorOne(boolean forward, int motor_speed){
   digitalWrite(motorOnePlus, forward);
   digitalWrite(motorOneMinus, !forward);
   analogWrite(motorOneEnable, motor_speed);
}

// Function to control the motor
void setMotorTwo(boolean forward, int motor_speed){
   digitalWrite(motorTwoPlus, forward);
   digitalWrite(motorTwoMinus, !forward);
   analogWrite(motorTwoEnable, motor_speed);
}
