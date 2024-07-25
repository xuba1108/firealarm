#include <WiFi.h>
#include <WiFiAP.h>
#include <WiFiClient.h>
#include <WiFiGeneric.h>
#include <WiFiMulti.h>
#include <WiFiSTA.h>
#include <WiFiScan.h>
#include <WiFiServer.h>
#include <WiFiType.h>
#include <WiFiUdp.h>
// Sensor node 
#include <ESP32Firebase.h>
#include "MQ7.h"
WiFiClient  client;
const char* ssid = "Khanh";   // your network SSID (name)
const char* password ="22222222";   // your network password
#define REFERENCE_URL "https://cap2-firebaseusa-default-rtdb.firebaseio.com/"  // Your Firebase project reference url
#include "DHT.h"

//khai bao kieu sensor
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321 
// khai bao sensor
#define MQ7 33 // khai bao MQ7
#define MQ2 35 // Khai bao MQ2
#define DHTPIN 32     // Digital pin connected to the DHT sensor
//buzzer pin
#define buzzerpin 27 // buzzer pin
Firebase firebase(REFERENCE_URL);
// as the current DHT reading algorithm adjusts itself to work on faster procs.
DHT dht(DHTPIN, DHTTYPE);
String previousStateRoom1 = "Normal_State";
String stateroom1;
String statebuzzer;

// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 100; //10s


void setup() {
Serial.begin(115200); //Initialize serial
    // Connect to Wifi
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  WiFi.mode(WIFI_STA);
  //sensor MQ7
      pinMode(MQ7, INPUT);
      pinMode(MQ2, INPUT);
      pinMode(buzzerpin,OUTPUT);
      dht.begin();
  

 


}

void loop()
{

  if ((millis() - lastTime) > timerDelay)
  {

    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED) {
      Serial.print("Attempting to connect");
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        delay(500);
      }
      Serial.println("\nConnected.");
    }

    int MQ7_value=0;
    MQ7_value = analogRead(MQ7);
    int MQ2_value=0;
    MQ2_value = analogRead(MQ2);
    //
     float h = dht.readHumidity();
    // Read temperature as Celsius (the default)
     float t = dht.readTemperature();
       Serial.print("Mq2 value:");
      Serial.println(MQ2_value);
      Serial.print("Mq7 value:");
      Serial.println(MQ7_value);
        Serial.print("Temperature:");
      Serial.println(t);
        Serial.print("Humidity:");
      Serial.println(h);
      firebase.setInt("Node 1/Smoke Sensor", MQ2_value);
      firebase.setInt("Node 1/CO Sensor", MQ7_value);
      firebase.setFloat("Node 1/Temp",t);
      firebase.setFloat("Node 1/Humidity",h);
      
        stateroom1 = firebase.getString("Node 1/State");
        statebuzzer = firebase.getString("Node 1/State Number");
       if (stateroom1 != previousStateRoom1 || statebuzzer == "1") 
      {
          Serial.print("Statefire:");
          Serial.println(stateroom1);
          Serial.print("Statebuzzer:");
          Serial.println(statebuzzer);
          digitalWrite(buzzerpin, HIGH);
      }  
  else
        {
          digitalWrite(buzzerpin, LOW); // Turn off the buzzer if conditions are not met
        }
      }  
  }  