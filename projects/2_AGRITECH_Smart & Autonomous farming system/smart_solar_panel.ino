#define GREEN_LED 4
#define RED_LED 3
#define input_aop 2
#define output_dir1 5
#define output_dir2 6
bool permission = true;
void setup() {
  pinMode(RED_LED,OUTPUT);
  pinMode(GREEN_LED,OUTPUT);
  pinMode(output_dir1,OUTPUT);
  pinMode(output_dir2,OUTPUT);
  pinMode(input_aop,INPUT);
}

void loop() {
  if (digitalRead(input_aop) == HIGH){ // LDR2 > LDR1
        if (permission == true){
          digitalWrite(RED_LED,HIGH);
          digitalWrite(GREEN_LED,LOW);
          // motor direction 1 
          digitalWrite(output_dir1,HIGH);
          digitalWrite(output_dir2,LOW);
          delay(60); // 60 ms 
          digitalWrite(RED_LED,LOW);
          digitalWrite(GREEN_LED,LOW);
          digitalWrite(output_dir1,LOW);
          digitalWrite(output_dir2,LOW);
          permission = false;  
        }
  } else if (digitalRead(input_aop) == LOW) { // LDR1 > LDR2
        if (permission == false){
          digitalWrite(RED_LED,LOW);
          digitalWrite(GREEN_LED,HIGH);
          // motor direction 2 
          digitalWrite(output_dir1,LOW);
          digitalWrite(output_dir2,HIGH);
          delay(60); // 60 ms 
          digitalWrite(output_dir1,LOW);
          digitalWrite(output_dir2,LOW);
          digitalWrite(RED_LED,LOW);
          digitalWrite(GREEN_LED,LOW);
          permission = true;
        }
  }
 }
