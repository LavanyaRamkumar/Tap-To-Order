//#include <DHT.h>
#include "HX711.h"
HX711 scale;

#include <ESP8266WiFi.h>

// replace with your channel’s thingspeak API key,
String apiKey = "SKXKCNDF1Z0GJK7K";
const char* ssid = "T2O";
const char* password = "taptoorder";

const char* server = "api.thingspeak.com";
//#define DHTPIN 2 // what pin we’re connected to

//DHT dht(DHTPIN, DHT22,15);
WiFiClient client;

void setup() {
  Serial.begin(115200);   
  Serial.println("HX711 Demo");
  Serial.println("Initializing the scale");
  // parameter "gain" is ommited; the default value 128 is used by the library
  // HX711.DOUT  - pin #D1
  // HX711.PD_SCK - pin #D2
  scale.begin(13, 15);
 

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  //float t=scale.read();
  //float t1=scale1.read();
  float t=((((scale.read()/10000.0)-31)*10)-87+50-8);
  Serial.println(String(t)+"g");
  //Serial.println(String(t1)+"g");

//Serial.begin(115200);
delay(10);
//dht.begin();

WiFi.begin(ssid, password);
Serial.println();//Serial.println();
Serial.print("Connecting to ");
Serial.println(ssid);

WiFi.begin(ssid, password) ;

while (WiFi.status() != WL_CONNECTED) {
delay(500);
Serial.print(".");
}
Serial.println("");
 Serial.println("WiFi connected");

}

void loop() {
  //float t=scale.read();
  //float t1=scale1.read();
  float t=(((scale.read()/10000.0)-31)*10)-87+50-8;
  Serial.print("Weight = ");
  //float wt = scale.get_units();
  Serial.print(t);
 // Serial.println(t1);
  Serial.println("g");
  



if (client.connect(server,80)) { // "184.106.153.149" oapi.thingspeak.com
String postStr = apiKey;
postStr +="&field2=";
postStr += String(t);
postStr += "\r\n\r\n";

client.print("POST /update HTTP/1.1\n");
client.print("Host: api.thingspeak.com\n");
client.print("Connection: close\n");
client.print("X-THINGSPEAKAPIKEY: "+apiKey+"\n");
client.print("Content-Type: application/x-www-form-urlencoded\n");
client.print("Content-Length: ");
client.print(postStr.length());
client.print("\n\n");
client.print(postStr);

Serial.print("Weight: ");
Serial.print(t);

Serial.println("g sent to Server");
scale.power_down();             // put the ADC in sleep mode
  delay(1000);  
  scale.power_up();

}
client.stop();

Serial.println("Waiting...");
// thingspeak needs minimum 15 sec delay between updates
delay(500);
}
