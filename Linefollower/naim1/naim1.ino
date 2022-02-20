// IR Sensor right
int IR1 = 12;
// IR Sensor left
int IR2 = 2;
// Motor A connections
int enA = 9;
int in1 = 8;
int in2 = 7;
// Motor B connections
int enB = 3;
int in3 = 5;
int in4 = 4;

void setup() {
  // Set all the motor control pins to outputs
  Serial.begin(9600);
  pinMode(IR1, INPUT);
  pinMode(IR2, INPUT);
  
  pinMode(enA, OUTPUT);
  pinMode(enB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(13,OUTPUT);
  
  // Turn off motors - Initial state
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);

  // Set Motor speed
  analogWrite(enA, 255);
  analogWrite(enB, 255);
}

void loop() {

  int ir1_input = digitalRead(IR1);
  int ir2_input = digitalRead(IR2);
  if( ir1_input == LOW && ir2_input == LOW ){
    Serial.println("go forward");
    motorRight(1); // forward
    motorLeft(1); //forward
  }
  else if( ir1_input == HIGH && ir2_input == LOW ){
    Serial.println("go right");
    motorRight(0); // stop
    motorLeft(1); //forward
  }
  else if( ir1_input == LOW && ir2_input == HIGH ){
    Serial.println("go left");
    motorRight(1); // forward
    motorLeft(0); //stop
  }
  else if( ir1_input == HIGH && ir2_input == HIGH ){
    Serial.println("Stop");
    motorRight(0); // stop
    motorLeft(0); //stop
  }

  //speedControl();
  delay(200);
}
void motorRight(int dir){
  if(dir == 1){ // go forward
    digitalWrite(in1, HIGH);
    digitalWrite(in2, LOW);
   }
   else if(dir == -1){ // go backward
    digitalWrite(in1, LOW);
    digitalWrite(in2, HIGH);
   }
   else{ // stop spinning
    digitalWrite(in1, LOW);
    digitalWrite(in2, LOW);
   }
}
void motorLeft(int dir){
  if(dir == 1){ // go forward
    digitalWrite(in3, HIGH);
    digitalWrite(in4, LOW);
   }
   else if(dir == -1){ // go backward
    digitalWrite(in3, LOW);
    digitalWrite(in4, HIGH);
   }
   else{ // stop spinning
    digitalWrite(in3, LOW);
    digitalWrite(in4, LOW);
   }
}

// This function lets you control spinning direction of motors
void directionControl() {
  // Set motors to maximum speed
  // For PWM maximum possible values are 0 to 255
  analogWrite(enA, 255);
  analogWrite(enB, 255);
}

// This function lets you control speed of the motors
void speedControl() {
  // Turn on motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);
  digitalWrite(in3, LOW);
  digitalWrite(in4, HIGH);
  
  // Accelerate from zero to maximum speed
  for (int i = 0; i < 256; i++) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  }
  
  
  // Decelerate from maximum speed to zero
  for (int i = 255; i >= 0; --i) {
    analogWrite(enA, i);
    analogWrite(enB, i);
    delay(20);
  }
  
  // Now turn off motors
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  digitalWrite(in3, LOW);
  digitalWrite(in4, LOW);
}
