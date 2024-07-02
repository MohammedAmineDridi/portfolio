#include <Servo.h>

// Pin configuration
const int flexSensorPin = A0;  // Analog pin for flex sensor
const int forceSensor1Pin = A1;  // Analog pin for force sensor 1
const int forceSensor2Pin = A2;  // Analog pin for force sensor 2
const int pwmPin = 9;  // PWM pin for servo motor

// Servo and PID controller configuration
Servo myServo;
double Kp = 1.0;  // Proportional gain
double Ki = 0.1;  // Integral gain
double Kd = 0.01;  // Derivative gain
double setpoint = 0.0;  // Desired servo angle

double prevError = 0.0;
double integral = 0.0;

void setup() {
  // Initialize servo and PID controller
  myServo.attach(pwmPin);
  myServo.write(90);  // Initial position
  Serial.begin(9600);
}

void loop() {
  // Read sensor values
  int flexSensorValue = analogRead(flexSensorPin);
  int forceSensor1Value = analogRead(forceSensor1Pin);
  int forceSensor2Value = analogRead(forceSensor2Pin);

  // Calculate actual servo angle (assuming a linear relationship between flex sensor value and angle)
  double actualAngle = map(flexSensorValue, 0, 1023, 0, 180);

  // Calculate error
  double error = setpoint - actualAngle;

  // Calculate PID terms
  double proportional = Kp * error;
  integral += Ki * error;
  double derivative = Kd * (error - prevError);

  // Calculate PID output
  double output = proportional + integral + derivative;

  // Calculate current based on force sensors and torque constant
  double torqueConstant = 1.0;  // Replace with your actual torque constant
  double current = (forceSensor1Value + forceSensor2Value) / torqueConstant;

  // Apply PID output to servo motor
  int pwmValue = map(output, -180, 180, 100, 0);
  myServo.write(pwmValue);

  // Print debug information
  Serial.print("Flex Sensor: ");
  Serial.print(flexSensorValue);
  Serial.print(" | Actual Angle: ");
  Serial.print(actualAngle);
  Serial.print(" | Error: ");
  Serial.print(error);
  Serial.print(" | PWM Output: ");
  Serial.print(pwmValue);
  Serial.print(" | Current: ");
  Serial.println(current);

  // Update previous error
  prevError = error;

  delay(100);
}
