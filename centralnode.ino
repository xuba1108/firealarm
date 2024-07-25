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
#include <ESP32Firebase.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4); // Set the LCD address to 0x27 for a 16 chars and 2 line display
WiFiClient client;
const char* ssid = "esp32";   // your network SSID (name)
const char* password = "12345678";   // your network password
#define REFERENCE_URL "https://cap2-firebaseusa-default-rtdb.firebaseio.com/"  // My Firebase project reference url
// introduce button1
int Key1 = 4;
int b=0;
bool prevState1 = LOW;
bool stateb1 = false;
// Introduce button2
int Key2 = 2;
bool prevState2 = LOW;
bool stateb2 = false;
// Introduce Button3
int Key3 = 15;
bool prevState3 = LOW;
bool stateb3 = false;
unsigned long lastTime = 0;
unsigned long timerDelay = 10; // Example delay of 100 mili second
String SDT1="0965225481";  //SDT CHINH
String stateroom1;
String stateroom2;
String stateroom3;
#define LED_STT 13  // LED BA0
Firebase firebase(REFERENCE_URL);

void setup() 
{
   Serial.begin(115200);  
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  lcd.init();                       // Initialize the LCD
  lcd.backlight();                  // Turn on the backlight
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("Prepare...");                      // Clear the LCD screen
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  WiFi.mode(WIFI_STA);
  //set up sim
  // initialize the button pins as input with pull-up:
  pinMode(Key1, INPUT);
  pinMode(Key2, INPUT);
  pinMode(Key3, INPUT);
  pinMode(LED_STT,OUTPUT);
  digitalWrite(LED_STT,0);
  delay(200);
  
     for(int a=0;a<40;a++)   //Doi 40s cho co song' ( 20-60s tuy noi )
 { 
  digitalWrite(LED_STT,0);
  delay(500);
  digitalWrite(LED_STT,1);
  delay(500);
 }


       // m? serial v?i baudrate 9600

  Serial.print("AT\r\n");  //gui lenh AT test SIM  
  digitalWrite(LED_STT,0);
  delay(50);
  digitalWrite(LED_STT,1);
  delay(250); 
  Serial.print("ATE0\r\n");// OFF rep
  digitalWrite(LED_STT,0);
  delay(50);
  digitalWrite(LED_STT,1);
  delay(250);
  Serial.print("AT+CSCS=\"GSM\"\r\n");              
  digitalWrite(LED_STT,0);
  delay(50);
  digitalWrite(LED_STT,1);
  delay(250);
  Serial.print("AT+CMGF=1\r\n");             
  digitalWrite(LED_STT,0);
  delay(50);
  digitalWrite(LED_STT,1);
  delay(250);
  Serial.print("AT+CNMI=2,2,0,0,0\r\n");  
  digitalWrite(LED_STT,0);
  delay(50);
  digitalWrite(LED_STT,1);
  delay(250);
  Serial.print("AT+CMGDA=\"DEL ALL\"\r\n");    
  digitalWrite(LED_STT,0);delay(50); digitalWrite(LED_STT,1);delay(250);
  Serial.print("AT+CLIP=1\r\n");    
  digitalWrite(LED_STT,0);delay(50); digitalWrite(LED_STT,1);delay(250);
  Serial.print("AT&W\r\n");     
  digitalWrite(LED_STT,0);delay(50); digitalWrite(LED_STT,1);delay(250);
  lcd.setCursor(0,0);
  lcd.print("Ready...");

}

void loop() 
{
  lcd.setCursor(0,0);
  lcd.print("    State of Fire");
  lcd.setCursor(0,1);
  lcd.print("LA2.201:");
  lcd.setCursor(0,2);
  lcd.print("LA2.202:");
  lcd.setCursor(0,3);
  lcd.print("LA2.208:");
  if ((millis() - lastTime) > timerDelay)
  {
    lastTime = millis(); // Update last time

    // Connect or reconnect to WiFi
    if (WiFi.status() != WL_CONNECTED) 
    {
      Serial.print("Attempting to connect");
      while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, password);
        delay(500);
        Serial.print(".");
      }
      Serial.println("\nConnected.");
    }

    button1();
    button2();
    button3();
    getroom1();
    getroom2();
    getroom3();
  }  
}

void button1()
{
  int currentState1 = digitalRead(Key1);
  if (currentState1 == HIGH && prevState1 == LOW) 
  {
    stateb1 = !stateb1; // Toggle the state
    if (stateb1)
    {
      firebase.setString("Node 1/State Number", "1");
      lcd.setCursor(0, 0);               // Set the cursor to the first column and first row
      lcd.print(" State of each room   ");     // Print some text
      lcd.setCursor(0,1);
      lcd.print("Room1: Fire");
      guitinnhan(SDT1," Lab 201 is fire!!!");
      delay(2000);
    }
    else
    {
      firebase.setString("Node 1/State Number", "0");
    }
  }

  prevState1 = currentState1;
  delay(50);
}

void button2()
{
  int currentState2 = digitalRead(Key2);
  if (currentState2 == HIGH && prevState2 == LOW) 
  {
    stateb2 = !stateb2; // Toggle the state
    if (stateb2)
    {
      firebase.setString("Node 2/State Number", "1");
      //lcd.setCursor(0, 0);               // Set the cursor to the first column and first row
      //lcd.print(" State of each room   ");     // Print some text
      lcd.setCursor(0,1);
      lcd.print("Room 2: Fire");
      guitinnhan(SDT1," Lab 202 is fire!!!");
      delay(5000);
    }
    else
    {
      firebase.setString("Node 2/State Number", "0");
    }
  }

  prevState2 = currentState2;
  delay(50);
}
void button3()
{
  int currentState3 = digitalRead(Key3);
  if (currentState3 == HIGH && prevState3 == LOW) 
  {
    stateb3 = !stateb3; // Toggle the state
    if (stateb3)
    {
      //firebase.setString("Node 3/State Number", "1");
            lcd.setCursor(0, 0);               // Set the cursor to the first column and first row
      lcd.print(" State of each room   ");     // Print some text
      lcd.setCursor(0,1);
      lcd.print("Room 3: Fire");
      guitinnhan(SDT1," Lab 203 is fire!!!");
      delay(2000);
    }
    else
    {
      firebase.setString("Node 3/State Number", "0");
    }
  }
  prevState3 = currentState3;
  delay(50);
}
 void goidien(String sdtnhan)
{  
 
  Serial.print("ATD");
  delay(200);
  Serial.print(SDT1);
  delay(200);
  Serial.println(";"); 
    
}
void getroom1()
{if(b%3==0){
  b++;
    stateroom1 = firebase.getString("Node 1/State");
                     // Turn on the backlight
                          // Clear the LCD screen
    if (stateroom1 != "Normal_State")
    {
      
      //firebase.setString("Node 1/State Number", "1");
      //lcd.setCursor(0,0);
      lcd.setCursor(0,1);
      lcd.print("LA2.201: Fire!!!");
      guitinnhan(SDT1," Lab 201 is fire!!!");
      delay(2000);
      goidien(SDT1);
      delay(5000);
  Serial.print("AT+CVHU=0\r\n");delay(100); 
  Serial.print("ATH\r\n");delay(100);
    }
    else 
    {
      //firebase.setString("Node 1/State Number", "0");
      lcd.setCursor(0,1);
      lcd.print("LA2.201: No Fire");
    }
}}

void getroom2()
{if(b%3==1){
    b++;
    stateroom2 = firebase.getString("Node 2/State");
                     // Turn on the backlight
                           // Clear the LCD screen
    if (stateroom2 != "Normal_State")
    {
      
      //firebase.setString("Node 2/State Number", "1");
      //lcd.print("State of each room");     // Print some text
      lcd.setCursor(0,2);
      lcd.print("LA2.202: Fire!!!");
      guitinnhan(SDT1," Lab 202 is fire!!!");
      delay(2000);
      goidien(SDT1);
    delay(5000);
  Serial.print("AT+CVHU=0\r\n");delay(100); 
  Serial.print("ATH\r\n");delay(100);
    }
    else 
    {
      //firebase.setString("Node 2/State Number", "0");
      lcd.setCursor(0,2);
      lcd.print("LA2.202: No Fire");
    }
}}

void getroom3()
{if(b%3==2){
  b++;
    stateroom3 = firebase.getString("Node 3/State");
                     // Turn on the backlight
                          // Clear the LCD screen
    if (stateroom3 != "Normal_State")
    {
      
      //firebase.setString("Node 3/State Number", "1");
      //lcd.print("State of each room");     // Print some text
      lcd.setCursor(0,3);
      lcd.print("LA2.208: Fire!!!");
      guitinnhan(SDT1," Lab 208 is fire!!!");
      delay(2000);
      goidien(SDT1);
      delay(5000);
  Serial.print("AT+CVHU=0\r\n");delay(100); 
  Serial.print("ATH\r\n");delay(100);
    }
    else 
    {
      //firebase.setString("Node 3/State Number", "0");
      lcd.setCursor(0,3);
      lcd.print("LA2.208: No Fire");
    }
}}


void guitinnhan(String sdtnhan,String noidung)
{ 

  Serial.print("AT+CMGS=\"");
  delay(200);
  Serial.print(sdtnhan) ;
  delay(200);
  Serial.println("\"")  ;
  delay(200);
  Serial.print(noidung) ;
  delay(200);
  Serial.write(26)      ;
  delay(200);     // Ending sendmessage
 
}