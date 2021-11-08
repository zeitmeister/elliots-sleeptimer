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
  // put your setup code here, to run once:
  pinMode(ledon, OUTPUT);
  pinMode(ledoff, OUTPUT);
  Serial.begin(9600);
  setup_wifi();
  timeClient.begin();
}

void loop() {
  // put your main code here, to run repeatedly:
  timeClient.update();
  if (timeClient.getHours() == 6 && timeClient.getMinutes() == 30){
    wakeNow = true;
    sleepMore = false;
    
  }
  if (timeClient.getHours() == 21 && timeClient.getMinutes() == 0) {
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
