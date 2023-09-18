#define samp_siz 4
#define rise_threshold 5
#define USE_ARDUINO_INTERRUPTS true    
#include <PulseSensorPlayground.h>     
#include <SoftwareSerial.h>
                               
PulseSensorPlayground pulseSensor;
SoftwareSerial BTserial(10, 11); 

int sensorPin = 0;
int buzzer = 13 ;
const int PulseWire = 0;       
int PulseSensorPurplePin = 0;        
int LED13 = 13;   
int Signal;                
int Threshold = 550; 
int plot;

void setup() {
   Serial.begin(9600);
   BTserial.begin(9600);

  pinMode(LED13,OUTPUT); 

   pulseSensor.analogInput(PulseWire);   
	pulseSensor.blinkOnPulse(LED13);       
	pulseSensor.setThreshold(Threshold);

  if (pulseSensor.begin()) {
		Serial.println("PulseSensor object created!");
	}
}

void loop ()
{
   float reads[samp_siz], sum;
   long int now, ptr;
   float last, reader, start;
   float first, second, third, before;
   int print_value;
   bool rising;
   int rise_count;
   int n;
   long int last_beat;

   int myBPM = pulseSensor.getBeatsPerMinute();      

    delay(20);
    pinMode (buzzer, OUTPUT) ;

  pulseSensor.outputSample();

   Signal = analogRead(PulseSensorPurplePin);  
   Serial.println(Signal);                    


   if(Signal > Threshold){                         
     digitalWrite(LED13,HIGH);
   } else {
     digitalWrite(LED13,LOW);                
   }
   delay(10);
   for (int i = 0; i < samp_siz; i++)
     reads[i] = 0;
   sum = 0;
   ptr = 0;
   while(1)
   {
     n = 0;
     start = millis();
     reader = 0.;
     do
     {
       reader += analogRead (sensorPin);
       n++;
       now = millis();
     }
     while (now < start + 20);  
     reader /= n; 

     sum -= reads[ptr];
     sum += reader;
     reads[ptr] = reader;
     last = sum / samp_siz;

     if (last > before)
     {
       rise_count++;
       if (!rising && rise_count > rise_threshold)
       {
         
         rising = true;
         first = millis() - last_beat;
         last_beat = millis();
              
         print_value = 60000. / (0.4 * first + 0.3 * second + 0.3 * third);
          if(print_value>60){
               noTone(buzzer);
        }else{
              tone(buzzer, 2250);
             
        }
        Serial.println("BPM :");
        Serial.println(print_value);
        Serial.println(" ");
        BTserial.println(print_value);         
         third = second;
         second = first;
       }
     }
     else
     {
       rising = false;
       rise_count = 0;
     }
     before = last;
     ptr++;
     ptr %= samp_siz;
   }
} 