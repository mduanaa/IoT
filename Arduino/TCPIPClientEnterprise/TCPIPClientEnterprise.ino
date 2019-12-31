/*|----------------------------------------------------------|*/
/*|WORKING EXAMPLE FOR HTTP/HTTPS CONNECTION                 |*/
/*|TESTED BOARDS: Devkit v1 DOIT, Devkitc v4                 |*/
/*|CORE: June 2018                                           |*/
/*|----------------------------------------------------------|*/
#include <WiFi.h>
//#include <HTTPClient.h>
#include "esp_wpa2.h"
#include <Wire.h>

// wifi name and password
#define EAP_IDENTITY "mduanaa@connect.ust.hk" //if connecting from another corporation, use identity@organisation.domain in Eduroam 
#define EAP_PASSWORD "" //your Eduroam password
const char* ssid = "eduroam"; // Eduroam SSID
// server ip and port
const char* server_ip = "143.89.50.18";
const uint16_t port = 49999;

int counter = 0;
int hall_sensor = 0;
int touch_pad = 0;

void setup() {
  Serial.begin(115200);
  delay(10);
  Serial.println();
  // ******** enterprise wifi connection ********//
  Serial.print("Connecting to network: ");
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
  Serial.println("WiFi connected");
  Serial.println("IP address set: "); 
  Serial.println(WiFi.localIP()); //print LAN IP
  
}
void loop() {
  // ******** wifi connection maintain ********//
  if (WiFi.status() == WL_CONNECTED) { //if we are connected to Eduroam network
    counter = 0; //reset counter
    Serial.print("wifi connection is stable: "); 
    Serial.println(WiFi.localIP());   //inform user about his IP address
  }else if (WiFi.status() != WL_CONNECTED) { //if we lost connection, retry
    WiFi.begin(ssid);      
  }
  while (WiFi.status() != WL_CONNECTED) { //during lost connection, print dots
    delay(500);
    Serial.print(".");
    counter++;
    if(counter>=60){ //30 seconds timeout - reset board
    ESP.restart();
    }
  }

  // ******** Sensor data read ********//
  hall_sensor = hallRead();
  touch_pad = touchRead(T0);

  // ******** TCP/IP client program ********//
  Serial.print("connecting to server: ");
  Serial.println(server_ip);
  WiFiClient client;
  // connect exception handle
  if(!client.connect(server_ip,port))
  {
      Serial.println("connection failed ");
      Serial.println("wait for 5 s ..... ");
      delay(5000);
      return;
    }
  // send data
//  client.print(hall_sensor);
  client.print(touch_pad);
  // receive data
  String line = client.readStringUntil('\r');
//  client.println(line);
  Serial.print("data received from server: ");
  Serial.println(line);
  //close connection
  Serial.println("closing connection... ");
  client.stop();
  Serial.println("wait 10s ..... ");
  delay(10000);

//    http.begin("https://arduino.php5.sk/rele/rele1.txt"); //HTTP example connection
//    int httpCode = http.GET();
//    if(httpCode > 0) {
//      Serial.printf("[HTTP] GET... code: %d\n", httpCode);
//      //file found at server --> on unsucessful connection code will be -1
//      if(httpCode == HTTP_CODE_OK) {
//        String payload = http.getString();
//        Serial.println(payload);
//      }
//     }else{
//      Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
//      }
 
//  delay(2000);
}
