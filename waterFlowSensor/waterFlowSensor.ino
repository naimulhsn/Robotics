/*
  AnalogReadSerial

  Reads an analog input on pin 0, prints the result to the Serial Monitor.
  Graphical representation is available using Serial Plotter (Tools > Serial Plotter menu).
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

  This example code is in the public domain.

  https://www.arduino.cc/en/Tutorial/BuiltInExamples/AnalogReadSerial
*/
volatile int flow_frequency; // Measures flow sensor pulses

unsigned int litre_per_hour; // Calculated litres/hour
float litre_per_min; // Calculated litres/hour
double total_volumn; // total volumn
float volumn; // calculated volumn
unsigned char flowsensor = 2; // Flow Meter Pin number
unsigned long currentTime;
unsigned long cloopTime;

void flow () // Interrupt function
{
   flow_frequency++;
}

// the setup routine runs once when you press reset:
void setup() {
   pinMode(flowsensor, INPUT);
   //digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
   Serial.begin(9600);
   
   attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;
   total_volumn = 0.0;
   volumn = 0.0;
}

void loop() {
  currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
      cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      //litre_per_hour = (flow_frequency * 60 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      litre_per_min = (flow_frequency * 1.0 / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter]

      volumn = litre_per_min/60.0;
      total_volumn += volumn;
      
      //Serial.print(litre_per_min, DEC); // Print litres/hour
      Serial.print(litre_per_min, 3); // Print litres/hour
      //Serial.println(" L/hour");
      Serial.print(" L/min   ..........   ");

      Serial.print(total_volumn,3);
      Serial.println(" Litre Total");
   }
   
  //delay(500);        // delay in between reads for stability
}
