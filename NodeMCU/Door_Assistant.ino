#include <ESP8266WiFi.h>
#include <FirebaseArduino.h>
#include <ESP8266HTTPClient.h>
#define FIREBASE_HOST "<host_here>"
#define FIREBASE_AUTH "<auth_key>"
#define WIFI_SSID "<ssid>"  
#define WIFI_PASSWORD "<password>"
#define LED 16 //new add
#define SERVER_KEY "<server_key>"

// defines pins numbers
const int trigPin = 2;  //D4
const int echoPin = 0;  //D3

//defines door status 
bool  doorStatus=0;     //0 means door is closed



// defines variables
long duration;
int distance;
bool sensor_status;//new add


void setup() {
Serial.begin(9600);
// connect to wifi.
WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
Serial.print("connecting");
while (WiFi.status() != WL_CONNECTED) {
Serial.print(".");
delay(500);
}
Serial.println();
Serial.print("connected: ");
Serial.println(WiFi.localIP());
Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(LED,OUTPUT);     //new add
}



void loop() {
  sensor_status = Firebase.getBool("sensor_status");
  //Serial.print("Sensor status");
  //Serial.println(sensor_status);
  if(sensor_status==1)
     digitalWrite(LED,LOW);
  else digitalWrite(LED,HIGH);   

if (sensor_status == 1) {
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);

// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);

// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);

// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
//Serial.print("Distance: ");
//Serial.print(distance);

delay(20);
if(distance<5){
  if(doorStatus==1){
        //Serial.println(distance);
      delay(20);  
      Serial.println("Door closed");
      Firebase.setBool("door_open",false);
     

 doorStatus=0;}
    }
    else
    {
      
      if(doorStatus==0){
        //Serial.println(distance);
      delay(20);  
      Serial.println("Door open");
      Firebase.setBool("door_open",true);
      doorStatus=1;

      if (sensor_status==1) {
        //Serial.println("Notification");
        sendNotification();
      }}
    }
}
  delay(200);
}

void sendNotification() {
     HTTPClient http;    //Declare object of class HTTPClient
  
     http.begin("http://fcm.googleapis.com/fcm/send");      //Specify request destination
     http.addHeader("Content-Type","application/json");  //Specify content-type header
     http.addHeader("Authorization","key=<server_key>");  //Specify content-type header
  
     String data = "{\n";
     data += "\"notification\": {\n";
     data += "\"title\" : \"SECURITY ALERT!\",\n";
     data += "\"body\": \"WARNING: Your door has been broken into.\",\n";
     data += "\"sound\" : \"notification.mp3\"\n";
     data += "}\n";
     data += ",\n";
     data += "\"to\" : \"/topics/door_status\"\n";
     data += "}\"\n";
  
     //Serial.println(data);
     
     int httpCode = http.POST(data);   //Send the request
     String payload = http.getString();                  //Get the response payload
   
     Serial.println(httpCode);   //Print HTTP return code
     Serial.println(payload);    //Print request response payload
     http.end();
}
