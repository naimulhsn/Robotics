#include "RTClib.h"
RTC_DS3231 rtc;

#define MoistureSensorVCCpin 7
#define MoistureSensor A0
#define relayPin1 6
#define flowsensor 2

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

volatile int flow_frequency;
unsigned long one_sec = 1000L;
unsigned long min_10 = one_sec * 600;
unsigned long min_30 = one_sec * 1800;

const int pot_size = 5; // Volume(Litre)of soil in each pot
const int pot_number = 1;    // Number of Pots/ Tobs / Container / Drams 


int lastWatered;
double howMuchWater(int avg){
  int total_soil = pot_size*pot_number; 
  if(avg>900) return 0.200*total_soil;
  if(avg>700) return 0.175*total_soil;
  if(avg>600) return 0.150*total_soil;
  if(avg>500) return 0.125*total_soil;
  if(avg>400) return 0.075*total_soil;
  return 0.0;
}
double SoilMoisture(){
  unsigned long avgMoisture = 0;
  int sample = 30;
  digitalWrite(MoistureSensorVCCpin, HIGH);
  delay(one_sec);
  for(int i=0; i<sample; i++){
    int sensorValue = analogRead(MoistureSensor);
    avgMoisture += sensorValue;
    //Serial.println(sensorValue);
    delay(300);
  }
  digitalWrite(MoistureSensorVCCpin, LOW);
  avgMoisture /= sample;
  Serial.print("Avg Moisture = ");
  Serial.println(avgMoisture,DEC);
  double needed_water = howMuchWater(avgMoisture);
  return needed_water;
}
void flow () // Interrupt function
{
   flow_frequency++;
}
double StartWatering(double needed_water){
  
  digitalWrite(relayPin1, LOW); // turn on Relay to start pump
  double total_volumn = 0.0; 
  float volumn;
  float litre_per_min; 
  unsigned long cloopTime = millis();
  unsigned long currentTime = cloopTime;
  
  unsigned long maxTime = currentTime + (int)(needed_water+1) * 120 * one_sec;
  
  while(total_volumn < needed_water){
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
      Serial.println(total_volumn,3);
    }
    
  }
  needed_water = max( (needed_water - total_volumn) , 0.0);
  Serial.print("More water needed :");
  Serial.println(needed_water,3);
  DateTime now = rtc.now();
  if(needed_water == 0.0)lastWatered = now.hour();
  Serial.print("lastWatered (hr) :");
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
    double needed_water = SoilMoisture();
    Serial.print("Needed Water(L) = ");
    Serial.println(needed_water,3);
    Serial.println(" ");
    while(needed_water>0.0){
      needed_water = StartWatering(needed_water);
      Serial.println(" ");
      delay(one_sec*60);
    }
    
  }
  
  delay(1000); // 5 min = 5*60*1000
}
