/*|----------------------------------------------------------|*/
/*|WORKING EXAMPLE FOR HTTP/HTTPS CONNECTION                 |*/
/*|TESTED BOARDS: Devkit v1 DOIT, Devkitc v4                 |*/
/*|CORE: June 2018                                           |*/
/*|----------------------------------------------------------|*/
#include <WiFi.h>
//#include <HTTPClient.h>
#include <PubSubClient.h>
#include "esp_wpa2.h"
#include <Wire.h>

// wifi name and password
#define EAP_IDENTITY "mduanaa@connect.ust.hk" //if connecting from another corporation, use identity@organisation.domain in Eduroam 
#define EAP_PASSWORD "" //your Eduroam password
const char* ssid = "eduroam"; // Eduroam SSID
// server ip and port
const char* server_ip = "143.89.50.18";
const uint16_t port = 49999;

// mqtt client 
WiFiClient espClient;
PubSubClient client(espClient);

//variable define
#define ledPin 2
int counter = 0;
//const int ledPin = 2;
long lastMsg = 0;
int hall_sensor = 0;
int touch_pad = 0;

//******** global set up ********//
void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  //wpa2 wifi
  wifi_setup();  
  //mqtt initiate
  client.setServer(server_ip, port);
  client.setCallback(callback);
  pinMode(ledPin, OUTPUT);
}

void loop() {
  //**** wifi connection ****//
//  wifi_reconnect();
  //**** mqtt connection ****//
  while (!client.connected()) {
    wifi_reconnect();
    mqtt_reconnect();
  }
  client.loop();

  //**** sensor read & publish ****//
//  long now = millis();
//  if(now - lastMsg > 10000){
//      lastMsg = now;

      // **** Sensor data read ****//
      //touch pad
      touch_pad = touchRead(T0);
      // Convert the value to a char array
      char touchString[8];
      dtostrf(touch_pad, 1, 2, touchString);
      Serial.print("touch sensor: ");
      Serial.println(touchString);
      client.publish("CYT3014/touch", touchString); 
      
      //hall sensor
      hall_sensor = hallRead();
      // Convert the value to a char array
      char hallString[8];
      dtostrf(hall_sensor, 1, 2, hallString);
      Serial.print("hall sensor: ");
      Serial.println(hallString);
      client.publish("CYT3014/hall", hallString); 

      // delay 10s
      Serial.println("delaying...");
      delay(10000);
//    }
}

//******** wifi setup ********//
void wifi_setup(){
  // ******** enterprise wifi connection ********//
  Serial.print("connecting to network: ");
  Serial.println(ssid);
  WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
  WiFi.mode(WIFI_STA); //init wifi mode
  esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide identity
  esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide username --> identity and username is same
  esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); //provide password
  esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
  esp_wifi_sta_wpa2_ent_enable(&config); //set config settings to enable function
  WiFi.begin(ssid); //connect to wifi
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    counter++;
    if(counter>=60){ //after 30 seconds timeout - reset board
      ESP.restart();
    }
  }
  Serial.println("");
  Serial.print("WiFi connected");
  Serial.println("IP address set: "); 
  Serial.println(WiFi.localIP()); //print LAN IP
  }

//******** wifi status check and reconnect ********//
void wifi_reconnect(){
    // **** wifi connection check ****//
  if (WiFi.status() != WL_CONNECTED) { //if we lost connection, retry
    Serial.print("wifi reconnecting");
    WiFi.disconnect(true);  //disconnect form wifi to set new wifi connection
    WiFi.mode(WIFI_STA); //init wifi mode
    esp_wifi_sta_wpa2_ent_set_identity((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide identity
    esp_wifi_sta_wpa2_ent_set_username((uint8_t *)EAP_IDENTITY, strlen(EAP_IDENTITY)); //provide username --> identity and username is same
    esp_wifi_sta_wpa2_ent_set_password((uint8_t *)EAP_PASSWORD, strlen(EAP_PASSWORD)); //provide password
    esp_wpa2_config_t config = WPA2_CONFIG_INIT_DEFAULT(); //set config settings to default
    esp_wifi_sta_wpa2_ent_enable(&config); //set config settings to enable function
    WiFi.begin(ssid);      
   }
   // **** wifi reconnect if disconnected ****//
  while (WiFi.status() != WL_CONNECTED) { //during lost connection, print dots
    delay(500);
    Serial.print(".");
    counter++;
    if(counter>=60){ //30 seconds timeout - reset board
    ESP.restart();
      break;
    }
  }
  }
  
//******** mqtt reconnect ********//
void mqtt_reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("CYT3014")) {
      Serial.println("connected");
      // Subscribe
      client.subscribe("CYT3014/led");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

//******** mqtt callback function ********//
void callback(char* topic, byte* message, unsigned int length) {
  Serial.print("message arrived on topic: ");
  Serial.print(topic);
  Serial.print(". message: ");
  String messageTemp;
  
  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  // Feel free to add more if statements to control more GPIOs with MQTT

  // If a message is received on the topic esp32/output, you check if the message is either "on" or "off". 
  // Changes the output state according to the message
  if (String(topic) == "CYT3014/led") {
    Serial.print("changing output to ");
    if(messageTemp == "on"){
      Serial.println("on");
      digitalWrite(ledPin, LOW);
    }
    else if(messageTemp == "off"){
      Serial.println("off");
      digitalWrite(ledPin, HIGH);
    }
  }
}
