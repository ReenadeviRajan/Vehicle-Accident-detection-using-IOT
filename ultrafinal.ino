#include <ESP8266WiFi.h>
#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"
#include "Servo.h" 

Servo servo_test;        //initialize a servo object for the connected servo  
 int obj = 0;
int angle = 0; 
long duration,distance;
int echopin = D2; 
int trigpin = D1; 
int led=D4;
int my_array1[20];
int my_array2[20];
int my_array3[20];
#define WLAN_SSID       "Rajkutty"             // Your SSID
#define WLAN_PASS       "rajkutty123"      //mention Password

/************************* Adafruit.io Setup *********************************/

#define IO_SERVER      "io.adafruit.com"
#define IO_SERVERPORT  1883                   // use 8883 for SSL

#define IO_USERNAME  "adventures2701"
#define IO_KEY       "d80c6678f66c4b2a835ea439b9f3aeaf"

/************ Global State (you don't need to change this!) ******************/

// Create an ESP8266 WiFiClient class to connect to the MQTT server.
WiFiClient client;
// or... use WiFiFlientSecure for SSL
//WiFiClientSecure client;

// Setup the MQTT client class by passing in the WiFi client and MQTT server and login details.
Adafruit_MQTT_Client mqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_KEY);

/****************************** Feeds ***************************************/


// Setup a feed called 'onoff' for subscribing to changes.
Adafruit_MQTT_Publish object = Adafruit_MQTT_Publish(&mqtt, IO_USERNAME "/feeds/obj");


void MQTT_connect();
void setup() {
  // put your setup code here, to run once:
  pinMode(trigpin, OUTPUT);
  pinMode(echopin, INPUT);
  pinMode(led,OUTPUT);
  servo_test.attach(D7);   
  delay(2000);  
  Serial.begin (9600);
  // Connect to WiFi access point.
  Serial.println(); Serial.println();
  Serial.print("Connecting to ");
 WiFi.begin(WLAN_SSID, WLAN_PASS);
//    WiFi.begin(WLAN_SSID);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();

  Serial.println("WiFi connected");
  Serial.println("IP address: "); 
  Serial.println(WiFi.localIP());

}

void loop() {
    MQTT_connect();
   for(angle = 0; angle <=90; angle += 1)
   {
    if(angle==45)
    {
    servo_test.write(angle);
    angle1();
    //delay(1000);
   }
   if(angle==90)
   {
    servo_test.write(angle);
    angle2();
    //delay(1000);
   }
   }
    for(angle = 90; angle>=0; angle-=1)     // command to move from 180 degrees to 0 degrees                               
    {
   if(angle==0)
   {
    servo_test.write(angle);
    angle3();
    //delay(1000);
   }
}
}
void angle1()
{
for (int i=0; i<= 5; i++) {
  digitalWrite(trigpin, LOW);
  delay(2000);
  digitalWrite(trigpin, HIGH);
  delay(2000);
  digitalWrite(trigpin, LOW);
  duration = pulseIn(echopin, HIGH);
  distance = duration * 0.034/2;
  Serial.print("1distance ");
  Serial.print(distance);
  Serial.println("cm");
   if(distance>=35)
   {
     my_array1[i] = 0; 
   break;
   }
  if(distance<35)
  {
     digitalWrite(led,HIGH);
     delay(60000);
    my_array1[i]=distance;
  }
   }
   int pro = 1;
    for (int i = 0; i <= 5; i++)
    {  
    pro = pro * my_array1[i];
    } 
    if(pro>0)
    {
      digitalWrite(led,HIGH);
       obj = 1;
      Serial.println("1Motion Detected");
      delay(6000);
    }
      if(pro<=0)
    {
      digitalWrite(led,LOW);
      obj = 0;
      Serial.println("1Motion Stopped");
      delay(3600);
    } 
}
void angle2()
{
for (int i=0; i<= 5; i++) {
  digitalWrite(trigpin, LOW);
  delay(2000);
  digitalWrite(trigpin, HIGH);
  delay(2000);
  digitalWrite(trigpin, LOW);
  duration = pulseIn(echopin, HIGH);
  distance = duration * 0.034/2;
  Serial.print("2distance ");
  Serial.print(distance);
  Serial.println("cm");
   if(distance>=47)
   {
   my_array2[i] = 0; 
   break;
   }
  if(distance<47)
  {
     digitalWrite(led,HIGH);
     delay(60000);
    my_array2[i]=distance;
  }
   }
   int pro1 = 1;
    for (int i = 0; i <= 5; i++)
    {  
    pro1 = pro1 * my_array2[i];
    } 
    if(pro1>0)
    {
      digitalWrite(led,HIGH);
       obj = 1;
      Serial.println("2Motion Detected");
      delay(6000);
    }
      if(pro1<=0)
    {
      digitalWrite(led,LOW);
       obj = 0;
      Serial.println("2Motion Stopped");
      delay(3600);
    } 
}
  void angle3()
{

for (int i=0; i<= 5; i++) {
  digitalWrite(trigpin, LOW);
  delay(2000);
  digitalWrite(trigpin, HIGH);
  delay(2000);
  digitalWrite(trigpin, LOW);
  duration = pulseIn(echopin, HIGH);
  distance = duration * 0.034/2;
  Serial.print("3distance ");
  Serial.print(distance);
  Serial.println("cm");
   if(distance>=67)
   {
   my_array3[i] = 0; 
   break;
   }
  if(distance<67)
  {
     digitalWrite(led,HIGH);
     delay(60000);
    my_array3[i]=distance;
  }
   }
   int pro2 = 1;
    for (int i = 0; i <= 5; i++)
    {  
    pro2 = pro2 * my_array3[i];
    } 
    if(pro2>0)
    {
      digitalWrite(led,HIGH);
      obj = 1;
      Serial.println("3Motion Detected");
      delay(6000);
    }
      if(pro2<=0)
    {
      digitalWrite(led,LOW);
       obj = 0;
      Serial.println("3Motion Stopped");
      delay(3600);
    } 
object.publish(obj);

}

void MQTT_connect() {
  int8_t ret;

  // Stop if already connected.
  if (mqtt.connected()) {
    return;
  }

  Serial.print("Connecting to MQTT... ");

  uint8_t retries = 3;
  while ((ret = mqtt.connect()) != 0) { // connect will return 0 for connected
    Serial.println(mqtt.connectErrorString(ret));
    Serial.println("Retrying MQTT connection in 5 seconds...");
    mqtt.disconnect();
    delay(5000);  // wait 5 seconds
    retries--;
    if (retries == 0) {
      // basically die and wait for WDT to reset me
      while (1);
    }
  }
  Serial.println("MQTT Connected!");
}
