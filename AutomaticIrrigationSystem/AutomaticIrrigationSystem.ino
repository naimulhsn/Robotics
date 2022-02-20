#include "RTClib.h"
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


#define MoistureSensorVCCpin 7
#define MoistureSensor A0
#define relayPin1 6
#define flowsensor 2


volatile int flow_frequency;
unsigned long one_sec = 1000L;
unsigned long min_10 = one_sec * 600;
unsigned long min_30 = one_sec * 1800;

const int Container = 1;    // Number of Pots/ Tobs / Container / Drams 


int lastWatered;
int howMuchWater(int avg){
  if(avg>900) return 1*Container;
  if(avg>800) return 5*Container;
  if(avg>700) return 4*Container;
  if(avg>600) return 3*Container;
  if(avg>500) return 2*Container;
  if(avg>400) return 1*Container;
  return 0;
}
int SoilMoisture(){
  unsigned long avgMoisture = 0;
  int sample = 5;
  digitalWrite(MoistureSensorVCCpin, HIGH);
  delay(one_sec);
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
int StartWatering(int needed_water){
  digitalWrite(relayPin1, LOW); // turn on Relay to start pump
  double total_volumn = 0.0; 
  float volumn;
  float litre_per_min; 
  unsigned long cloopTime = millis();
  unsigned long currentTime = cloopTime;
  
  unsigned long maxTime = currentTime + (needed_water+1)/2 * 60 * one_sec;
  
  while( (int)total_volumn < needed_water){
    currentTime = millis();
    
    if(currentTime >= (cloopTime + 1000)){
      if(currentTime > maxTime)break;
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      litre_per_min = (flow_frequency * 1.0 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter]

      volumn = litre_per_min/60.0;
      total_volumn += volumn;
      Serial.print("Given Water:");
      Serial.println(total_volumn,4);
    }
    
  }
  needed_water = max(needed_water - (int)total_volumn, 0);
  Serial.print("More water needed :");
  Serial.println(needed_water,4);
  DateTime now = rtc.now();
  if(needed_water == 0)lastWatered = now.hour();
  Serial.print("lastWatered :");
  Serial.println(lastWatered,DEC);
  digitalWrite(relayPin1, HIGH);
  return needed_water;
}

void setup() {
  
  pinMode(MoistureSensorVCCpin, OUTPUT);
  digitalWrite(MoistureSensorVCCpin, LOW);
  pinMode(MoistureSensor,INPUT);
  
  pinMode(relayPin1, OUTPUT);
  digitalWrite(relayPin1,HIGH);

  pinMode(flowsensor, INPUT);
  
  Serial.begin(57600);

  attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   
  delay(1000); // wait Serial port to connect
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    Serial.flush();
    while (1) delay(20);
  }
  if ( rtc.lostPower()) {
    Serial.println("RTC lost power, let's set the time!");
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  lastWatered = -100;
  
}

void loop() {
  DateTime now = rtc.now();
  int hr = now.hour();
  if( hr>(lastWatered+5) && (  (hr>=7 && hr <=9) || ( hr>=17 && hr<=20) ) ){
    int needed_water = SoilMoisture();
    Serial.print("Needed Water = ");
    Serial.println(needed_water,DEC);
    while(needed_water>0){
      needed_water = StartWatering(needed_water);
      delay(min_30);
    }
    
  }
  
  delay(1000); // 5 min = 5*60*1000
}
