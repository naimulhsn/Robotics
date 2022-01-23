#define MoistureSensorVCCpin 7
#define MoistureSensor A0

void readSoilMoisture(){
  digitalWrite(MoistureSensorVCCpin, HIGH);
  delay(100);
  int sensorValue = analogRead(MoistureSensor);
  Serial.println(sensorValue);
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
  delay(900);
}
