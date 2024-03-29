// Project Smart Garden


#include<Servo.h>
#include <dht.h>


 dht DHT ;

#define DHT11_PIN 9

int sensor_pin = A0; // Soil Sensor input at Analog PIN A0
int output_value ;
int Temp;
int Humidity;
int relayPin = A5;





Servo motor;
int pir;
int pirstate= LOW;
int ldr;
int trigPin =8;
int echoPin = 7;


//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h>

// RemoteXY connection settings
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "Project Smart Garden"
#define REMOTEXY_WIFI_PASSWORD "12345678@"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,4,0,33,0,12,1,14,24,1,
  2,1,3,21,22,11,1,26,31,31,
  79,78,0,79,70,70,0,2,1,3,
  45,22,11,1,26,31,31,79,78,0,
  79,70,70,0,2,1,36,21,22,11,
  1,26,31,31,79,78,0,79,70,70,
  0,2,1,36,45,22,11,1,26,31,
  31,79,78,0,79,70,70,0,129,0,
  7,3,49,6,31,83,77,65,82,84,
  32,71,65,82,68,69,78,0,129,0,
  1,15,25,3,31,87,97,116,101,114,
  105,110,103,32,83,121,115,116,101,109,
  0,129,0,31,15,31,3,31,83,117,
  114,118,101,105,108,108,97,110,99,101,
  32,83,121,115,116,101,109,32,0,129,
  0,2,39,24,4,31,83,109,97,114,
  116,32,76,69,68,39,115,0,129,0,
  32,39,28,4,31,71,97,116,105,110,
  103,32,83,121,115,116,101,109,0,67,
  5,38,68,20,5,120,26,11,67,5,
  38,78,20,5,120,26,11,67,5,38,
  88,20,5,120,26,11,129,0,2,68,
  28,4,31,84,101,109,112,114,97,116,
  117,114,101,40,67,41,0,129,0,2,
  78,24,4,31,72,117,109,105,100,105,
  116,121,40,37,41,0,129,0,2,88,
  23,4,31,77,111,105,115,116,117,114,
  101,40,37,41,0 };

// this structure defines all the variables and events of your control interface
struct {

    // input variables
  uint8_t switch_1; // =1 if switch ON and =0 if OFF
  uint8_t switch_2; // =1 if switch ON and =0 if OFF
  uint8_t switch_3; // =1 if switch ON and =0 if OFF
  uint8_t switch_4; // =1 if switch ON and =0 if OFF

    // output variables
  char text_T[11];  // string UTF8 end zero
  char text_H[11];  // string UTF8 end zero
  char text_M[11];  // string UTF8 end zero

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

#define PIN_SWITCH_1 A0
#define PIN_SWITCH_2 A1
#define PIN_SWITCH_3 10
#define PIN_SWITCH_4 6


void setup()
{
  RemoteXY_Init ();

  pinMode (PIN_SWITCH_1, OUTPUT);
  pinMode (PIN_SWITCH_2, OUTPUT);
  pinMode (PIN_SWITCH_3, OUTPUT);
  pinMode (PIN_SWITCH_4, OUTPUT);

  // TODO you setup code

  pinMode(A5,OUTPUT);
  pinMode(9,INPUT);
  pinMode(A4,OUTPUT);
  pinMode(10,INPUT);
  pinMode(A1,INPUT);
  pinMode(5,OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  motor.attach(6);
  motor.write(180);
  pinMode(sensor_pin, INPUT);
  pinMode(relayPin, OUTPUT);
 // Serial.begin(9600);


}

void loop()
{
  RemoteXY_Handler ();

  digitalWrite(PIN_SWITCH_1, (RemoteXY.switch_1==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_2, (RemoteXY.switch_2==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_3, (RemoteXY.switch_3==0)?LOW:HIGH);
  digitalWrite(PIN_SWITCH_4, (RemoteXY.switch_4==0)?LOW:HIGH);




   //DHT AND SOIL SENSOR WITH PUMP
  int chk = DHT.read11(DHT11_PIN);
  Temp= DHT.temperature;
  Humidity=DHT.humidity;
  output_value= analogRead(sensor_pin);

  dtostrf(Temp, 0, 1, RemoteXY.text_T);
  dtostrf(Humidity, 0, 1, RemoteXY.text_H);
  dtostrf(output_value, 0, 1, RemoteXY.text_M);
  delay(2000);

   if(output_value<30){
  digitalWrite(relayPin, LOW);

 }
 else
 {
  digitalWrite(relayPin, HIGH);

 }

  delay(2500);



  pir=digitalRead(10);
  ldr=analogRead(A1);


   //SURVILLENCE SYSTEM
 // Serial.println(pir);

  if(pir==1)
    {
  digitalWrite(A4,HIGH);

    if(pirstate==LOW){
    pirstate== HIGH;
    }

   }
  else
  {
     digitalWrite(A4,LOW);

    if(pirstate==HIGH){
      pirstate== LOW;
    }


  }
  delay(400);


   //AUTOMATIC LIGHTS
  //Serial.print("LDR");
  //Serial.println(ldr);

   if(ldr<50)
    {
     digitalWrite(5,LOW);

   }
else
  {
    digitalWrite(5,HIGH);
  }
  delay(400);



  //AUTOMATIC GATE
  long duration, distance;
  digitalWrite(trigPin,LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin,LOW);
  duration=pulseIn(echoPin, HIGH);
  distance =duration*0.034/2;
  //Serial.print(distance);
 // Serial.println("CM");
  delay(10);

  if(distance<=100)
  {
    motor.write(180);
   }

   else
 {
  motor.write(90);
   }
 delay(400);








}