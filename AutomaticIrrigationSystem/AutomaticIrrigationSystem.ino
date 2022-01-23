#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

#define MoistureSensorVCCpin 7
#define MoistureSensor A0

#define relayPin1 8

#define flowsensor 2
volatile int flow_frequency;



int lastWatered;
int howMuchWater(int avg){
  if(avg>800) return 3;
  if(avg>600) return 2;
  if(avg>400) return 1;
}
int SoilMoisture(){
  unsigned long avgMoisture = 0;
  int sample = 20;
  digitalWrite(MoistureSensorVCCpin, HIGH);
  for(int i=0; i<sample; i++){
    int sensorValue = analogRead(MoistureSensor);
    avgMoisture += sensorValue;
    //Serial.println(sensorValue);
    delay(500);
  }
  digitalWrite(MoistureSensorVCCpin, LOW);
  avgMoisture /= sample;
  Serial.print("Avg Moisture = ");
  Serial.println(avgMoisture,DEC);
  int needed_water = howMuchWater(avgMoisture);
  return needed_water;
}
void flow () // Interrupt function
{
   flow_frequency++;
}
void StartWatering(int needed_water){
  digitalWrite(relayPin1, HIGH); // turn on Relay to start pump
  double total_volumn; 
  float volumn;
  float litre_per_min; 
  unsigned long cloopTime = millis();
  unsigned long currentTime;
  while( (int)total_volumn < needed_water){
    currentTime = millis();
    if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      litre_per_min = (flow_frequency * 1.0 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter]

      volumn = litre_per_min/60.0;
      total_volumn += volumn;
   }
  }
  DateTime now = rtc.now();
  lastWatered = now.hour();
  
  digitalWrite(relayPin1, LOW);
}
void setup() {
  
  pinMode(MoistureSensorVCCpin, OUTPUT);
  digitalWrite(MoistureSensorVCCpin, LOW);
  pinMode(MoistureSensor,INPUT);
  
  pinMode(relayPin1, OUTPUT);

  pinMode(flowsensor, INPUT);
  
  Serial.begin(57600);

  attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   
  delay(1000); // wait Serial port to connect
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(10);
  }
  if (rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  lastWatered = -100;
  
}

void loop() {
  DateTime now = rtc.now();
  int hr = now.hour();
  if( hr>(lastWatered+5) && (  (hr>=7 && hr <9) || ( hr>=17 && hr<19) ) ){
    int needed_water = SoilMoisture();
    Serial.print("Needed Water = ");
    Serial.println(needed_water,DEC);
    StartWatering(needed_water);
  }
  delay(60000);
}
