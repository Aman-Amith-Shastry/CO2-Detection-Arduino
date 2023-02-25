#include<SoftwareSerial.h>
#include <ArduinoQueue.h>

#define QUEUE_SIZE_ITEMS 10

ArduinoQueue<String> strQueue(QUEUE_SIZE_ITEMS);

int co2Zero1 = 59;
int co2Zero2 = 110;

int tx = 2; // actual RX on HC-05
int rx = 3; // actual TX on HC-05

int sensePin1 = A0;
int sensePin2 = A1;

float val1 = 0;
float val2 = 0;

float co2ppm1 = 0;
float co2ppm2 = 0;

String m1 = "";
String m2 = "";
String msg = "";

int count = 0;

SoftwareSerial BTserial(rx, tx);


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  BTserial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  val1 = analogRead(sensePin1) - co2Zero1;
  val2 = analogRead(sensePin2) - co2Zero2;
  
  m1 = String(map(val1, 0,1023, 400,5000)) + ".00";
  m2 = String(map(val2, 0,1023, 400,5000)) + ".00";


  if(m1.length() < 7){
    m1 = "0" + m1; 
    }

 if(m2.length() < 7){
    m2 = "0" + m2;
    }

  msg = m1 + "," + m2;
  
  int rec = BTserial.read();

  Serial.println(val1);

  if(rec == 1){

      for(int i = 0; i < QUEUE_SIZE_ITEMS; i++){
          if(!strQueue.isEmpty()){
              String queue_msg = strQueue.dequeue();
              BTserial.println(queue_msg);
              Serial.println("Removed from queue: " + queue_msg);
            }
        }

      BTserial.println(msg);
    } 
    
  else{
    strQueue.enqueue(msg);
    Serial.println("Added to queue: " + msg);
   }


    delay(2000);
}
