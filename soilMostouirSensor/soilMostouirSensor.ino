#define MoistureSensorVCCpin 7
#define MoistureSensor A0

void readSoilMoisture(){
  digitalWrite(MoistureSensorVCCpin, HIGH);
  delay(50);
  int sensorValue = analogRead(MoistureSensor);
  Serial.println(sensorValue);
  delay(50);
  digitalWrite(MoistureSensorVCCpin, LOW);
}
void setup() {
  pinMode(MoistureSensorVCCpin, OUTPUT);
  digitalWrite(MoistureSensorVCCpin, HIGH);
  pinMode(MoistureSensor,INPUT);
  
  Serial.begin(9600);
}
void loop() {
  readSoilMoisture();
  delay(100);
}
