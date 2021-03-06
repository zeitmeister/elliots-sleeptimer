#include <ESP8266WiFi.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <PubSubClient.h>

#define ledon 4
#define ledoff 12

bool sleepMore = false;
bool wakeNow = false;
const char* ssid = "";
const char* password = "";
const long utcOffsetInSeconds = 3600;
byte ledOnPins[] ={5, 4, 0, 2};
int ledDelay(65); // delay changes
int leddirection = 1;
int ledcurrentLED = 0;
unsigned long changeTime;
 
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

void setup_wifi(){
  delay(10);
  WiFi.begin(ssid, password);
  while(WiFi.status() != WL_CONNECTED){
    delay(500);
    Serial.print(".");
  }
  Serial.print("Wifi connected - ESP IP address: ");
  Serial.println(WiFi.localIP());
}




void setup() {

  for (int x=0; x<4; x++) {
    pinMode(ledOnPins[x], OUTPUT); 
  } // set all pins to output
   changeTime = millis();
  // put your setup code here, to run once:

  pinMode(ledoff, OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  timeClient.begin();
}

void loop() {
  if ((millis() - changeTime) > ledDelay) { // if it has been ledDelay ms since last change
    changeLED();
    changeTime = millis();
  }
  // put your main code here, to run repeatedly:
  timeClient.update();
  if (timeClient.getHours() == 6 && timeClient.getMinutes() == 30){
    wakeNow = true;
    sleepMore = false;
    
  }
  if (timeClient.getHours() == 19 && timeClient.getMinutes() == 0) {
    sleepMore = true;
    wakeNow = false;
  }
  if (sleepMore == true){
    Serial.print("You should be sleeping");
    digitalWrite(ledon, LOW);
    digitalWrite(ledoff, HIGH);
    Serial.println(timeClient.getFormattedTime());
  }
  if (wakeNow == true) {
    Serial.println("WAKE UP");
    Serial.println(timeClient.getFormattedTime());
    digitalWrite(ledon, HIGH);
    digitalWrite(ledoff, LOW);
  }
  
}

void changeLED() {
for (int x=0; x<4; x++) {    // Increment x++
digitalWrite(ledOnPins[x], LOW);
}
 
digitalWrite(ledOnPins[ledcurrentLED], HIGH); // Turning on the current LED          
ledcurrentLED += leddirection; // increment the direction by value
 
// changing the direction if we reach the last LED
if (ledcurrentLED == 3) {leddirection = -1;}
if (ledcurrentLED == 0) {leddirection = 1;}
 
}
