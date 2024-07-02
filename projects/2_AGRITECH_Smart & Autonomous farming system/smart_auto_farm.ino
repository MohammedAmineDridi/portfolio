/* ----------------------------------------------------------- LCD LIB ---------------------------------------------------------- */

#include <LiquidCrystal.h>
#define rs 8
#define en 2
#define d4 3
#define d5 11
#define d6 12
#define d7 13

LiquidCrystal lcd(rs,en,d4,d5,d6,d7); // rs(0)-en(1)-d4(2)-d5(3)-d6(4)-d7(5)

void LCD_Config(char lines_number,char cols_number){
  lcd.begin(cols_number,lines_number);
}

void LCD_Write(char col_number,char line_number,String text){
  lcd.setCursor(col_number,line_number);
  lcd.print(text);  
}

/* --------------------------------------------------- MOTORS CONTROL LIB -------------------------------------------------------- */

// --------------------- COVER MOTORS 3 + 4 --------------------- 

#define R_Motor3_4_PIN 9
#define CV_Motor3_4_PIN 10

void COVER_MOTOR_Init() {
  digitalWrite(CV_Motor3_4_PIN,HIGH); // CV = 1
  digitalWrite(R_Motor3_4_PIN,HIGH); // R = 1
}

void COVER_MOTORS_left(){
  COVER_MOTOR_Init(); // CV = R = 1
  delay(1); // 0.1s
  if ( digitalRead(CV_Motor3_4_PIN) == HIGH ) {
    digitalWrite(CV_Motor3_4_PIN,LOW); // CV = 0
  } else if ( digitalRead(R_Motor3_4_PIN) == LOW ) {
    digitalWrite(R_Motor3_4_PIN,HIGH); // R = 0->1
    // ====> CV = 0 / R = 0->1 : 0->-90deg (left)
  }
}

void COVER_MOTORS_right(){
  COVER_MOTOR_Init(); // CV = R = 1
  delay(1); // 0.1s
  if ( digitalRead(CV_Motor3_4_PIN) == LOW ) {
    digitalWrite(CV_Motor3_4_PIN,HIGH); // CV = 1  
  }
  if ( digitalRead(R_Motor3_4_PIN) == HIGH ) {
    digitalWrite(R_Motor3_4_PIN,LOW); // R = 1->0
    // ====> CV = 1 / R = 1->0 : 0->90deg (right) 
  }
}

// --------------------- MOTORS 1 + 2 --------------------- 

#define R_Motor1_PIN 4
#define CV_Motor1_PIN 5
#define R_Motor2_PIN 6
#define CV_Motor2_PIN 7

void MOTORS_Config() {
  pinMode(R_Motor1_PIN,OUTPUT);
  pinMode(CV_Motor1_PIN,OUTPUT);
  pinMode(R_Motor2_PIN,OUTPUT);
  pinMode(CV_Motor2_PIN,OUTPUT);
  pinMode(R_Motor3_4_PIN,OUTPUT);
  pinMode(CV_Motor3_4_PIN,OUTPUT);
}

void MOTOR_Init(char motor_number) {
  if ( motor_number == 1 ) {
    digitalWrite(R_Motor1_PIN,HIGH); // CV_motor1 = 1
    digitalWrite(CV_Motor1_PIN,HIGH); // R_motor1 = 1  
  } else if ( motor_number == 2 ) {
    digitalWrite(R_Motor2_PIN,HIGH); // CV_motor2 = 1
    digitalWrite(CV_Motor2_PIN,HIGH); // R_motor2 = 1 
  }
}

void MOTOR_left(char motor_number){
  if ( motor_number == 1 ) {
    MOTOR_Init(1); // CV = R = 1
    delay(1); // 0.1s
    if ( digitalRead(CV_Motor1_PIN) == HIGH ) {
      digitalWrite(CV_Motor1_PIN,LOW); // CV = 0
    }
    if ( digitalRead(R_Motor1_PIN) == LOW ) {
      digitalWrite(R_Motor1_PIN,HIGH); // R = 0->1
      // ====> CV = 0 / R = 0->1 : 0->-90deg (left)
    }  
  } else if ( motor_number == 2 ) {
    MOTOR_Init(2); // CV = R = 1
    delay(1); // 0.1s
    if ( digitalRead(CV_Motor2_PIN) == HIGH ) {
      digitalWrite(CV_Motor2_PIN,LOW); // CV = 0
    } 
    if ( digitalRead(R_Motor2_PIN) == LOW ) {
      digitalWrite(R_Motor2_PIN,HIGH); // R = 0->1
      // ====> CV = 0 / R = 0->1 : 0->-90deg (left)
    }  
  }
}

void MOTOR_right(char motor_number){
  if ( motor_number == 1 ) {
    MOTOR_Init(1); // CV = R = 1
    delay(1); // 0.1s
    if ( digitalRead(CV_Motor1_PIN) == LOW ) {
      digitalWrite(CV_Motor1_PIN,HIGH); // CV = 1  
    }
    if ( digitalRead(R_Motor1_PIN) == HIGH ) {
      digitalWrite(R_Motor1_PIN,LOW); // R = 1->0
      // ====> CV = 1 / R = 1->0 : 0->90deg (right) 
    }  
  } else if ( motor_number == 2 ) {
    MOTOR_Init(2); // CV = R = 1
    delay(1); // 0.1s
    if ( digitalRead(CV_Motor2_PIN) == LOW ) {
      digitalWrite(CV_Motor2_PIN,HIGH); // CV = 1  
    }
    if ( digitalRead(R_Motor2_PIN) == HIGH ) {
      digitalWrite(R_Motor2_PIN,LOW); // R = 1->0
      // ====> CV = 1 / R = 1->0 : 0->90deg (right) 
    }  
  }
}

/* ------------------------------------------------------- SENSORS LIB ----------------------------------------------------------- */

#define Soil_Moisture_Sensor_PIN A1
#define Water_Sensor_PIN A2
#define Rain_Sensor_PIN A3
#define Temperature_Sensor_PIN A0

void SENSORS_Config(){
   pinMode(Soil_Moisture_Sensor_PIN,INPUT);
   pinMode(Water_Sensor_PIN,INPUT);
   pinMode(Rain_Sensor_PIN,INPUT);
   pinMode(Temperature_Sensor_PIN,INPUT);
}

// ----------- 1 - Water Level Sensor -----------
int Water_Level_Value(){
  int Value = analogRead(Water_Sensor_PIN);
  int Water_Level_Value = map(Value,0,1023,0,100);
  return Water_Level_Value;
}

// ----------- 2 - Soil Moisture Sensor -----------
int Soil_Moisture_Level_Value(){
  int Value = analogRead(Soil_Moisture_Sensor_PIN);
  int Soil_Moisture_Value = map(Value,0,1023,0,100);
  return Soil_Moisture_Value;
}

// ----------- 3 - Rain Sensor -----------
bool Rain_Status(){
  bool Rain_Status = digitalRead(Rain_Sensor_PIN);
  return Rain_Status;
}

// ----------- 4 - Temperature Sensor -----------
int Temperature_Level_Value(){
  int Value = analogRead(Temperature_Sensor_PIN);
  float mv = (Value/1024.0)*5000;
  float celcius = mv/10;  
  return (int)celcius;
}

/* --------------------------------------------------------- Main Code ----------------------------------------------------------- */

#define LED_PIN A4

bool Motor1_State = false; // motor 1 = OFF init
bool Motor2_State = false; // motor 2 = OFF init
bool Motor3_State = false; // motor 3 = OFF init
bool Motor4_State = false; // motor 4 = OFF init

void setup() {
  pinMode(LED_PIN,OUTPUT);
  MOTORS_Config();
  MOTOR_left(1); // init : motor1 = -90 ( motor 1 = OFF)
  MOTOR_left(2); // init : motor2 = -90 ( motor 2 = OFF)
  COVER_MOTORS_left(); // init : motor 3+4 = -90 ( motor 3+4 = OFF)
  LCD_Config(4,16);
  SENSORS_Config();
}

void Display(){
  String Soil = "SOIL="+String(Soil_Moisture_Level_Value())+"%";
  String Temperature = "Temp="+String(Temperature_Level_Value());
  String Rain = "Rain="+String(Rain_Status());
  String WaterLevel = "Water="+String(Water_Level_Value())+"%";
  String Lcd_row1 = Soil +"/"+ Temperature;
  String Lcd_row2 = Rain +"/"+ WaterLevel;
  String Lcd_row3 = "MTR 1="+String(int(Motor1_State))+"/MTR 2="+String(int(Motor2_State));
  String Lcd_row4 = "MTR 3="+String(int(Motor3_State))+"/MTR 4="+String(int(Motor4_State));
  lcd.clear();
  LCD_Write(0,0,Lcd_row1);
  LCD_Write(0,1,Lcd_row2);
  LCD_Write(0,2,Lcd_row3);
  LCD_Write(0,3,Lcd_row4);  
}

void loop() {
  // RAIN TEST
  if ( Rain_Status() == 1 ){
    if ( Temperature_Level_Value() < 2 ) { // probability of snow '2 = snow avg tempertaure'
      COVER_MOTORS_right(); // motor 3+4 'cover motors' = ON to 'right' (0->90deg)
      Motor3_State = true;
      Motor4_State = true;
    } else {
      COVER_MOTORS_left(); // motor 3+4 'cover motors' = OFF to 'left' (0->-90deg)
      Motor3_State = false;
      Motor4_State = false;
      MOTOR_left(1); // 0->-90deg (motor1'tank'=OFF)  because it's already raining
      MOTOR_left(2); // 0->-90deg (motor2'irrigation'=OFF) because it's already raining
      Motor1_State = false;
      Motor2_State = false;
    }     
  } else if ( Rain_Status() == 0 ) {// not raining
    COVER_MOTORS_left(); // motor 3+4 'cover motors' = OFF to 'left' (0->-90deg)
    Motor3_State = false;
    Motor4_State = false;
    // WATER LEVEL TEST
    if ( Water_Level_Value() > 80 ) {
      MOTOR_left(1); // 0->-90deg (motor1'tank'=OFF)  because water level in tank is very high
      Motor1_State = false;
    } else if ( Water_Level_Value() < 20 ) {
      MOTOR_right(1); // 0->90deg (motor1'tank'=ON) to fill the water tank
      Motor1_State = true;
    } 
    // SOIL MOISTURE TEST
    if ( Soil_Moisture_Level_Value() > 80 ) { // the plants have a good quantity of water
      MOTOR_left(2); // 0->-90deg (motor2'irrigation'=OFF) because the plants have a good quantity of water
      Motor2_State = false;
    } else if ( Soil_Moisture_Level_Value() < 20 ) {
      MOTOR_right(2); // 0->90deg (motor2'irrigation'=ON) because the plants don't have a good quantity of water
      Motor2_State = true;
    }
  }
  Display();
}
