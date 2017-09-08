#include <HX711.h>


//#include <MQTTClient.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
HX711 scale;
unsigned long prevTime;


  //  your network SSID (name)
char ssid[] = <T2O>;
// your network password
char pass[] = <taptoorder>;
// Initialize the Wifi client library
WiFiClient client;

// Initialize the PuBSubClient library
PubSubClient mqttClient(client);
// Define the ThingSpeak MQTT broker
const char* server = "mqtt.thingspeak.com";

// track the last connection time
unsigned long lastConnectionTime = 0;
// post data every 20 seconds
const unsigned long postingInterval = 20L * 1000L;



void setup() {
  Serial.begin(115200);
  Serial.println("HX711 Demo");
  Serial.println("Initializing the scale");
  // parameter "gain" is ommited; the default value 128 is used by the library
  // HX711.DOUT  - pin #D1
  
  scale.begin(D1, D2);
  //13,15 D1,D2

  Serial.println("Before setting up the scale:");
  Serial.print("read: \t\t");
  Serial.println(scale.read());
  
  Serial.print("Connecting to ");
      Serial.println(ssid);
      WiFi.begin(ssid, pass);
      while (WiFi.status() != WL_CONNECTED)
      {
        delay(500);
        Serial.print(".");
      }
      Serial.println("");
      Serial.println("WiFi connected");
      Serial.println("IP address: ");
      Serial.println(WiFi.localIP());
  // Set the MQTT broker details
  mqttClient.setServer(server, 1883);
}  // print a raw reading from the ADC
/*
  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));   // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight (not set yet)

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);  // print the average of 5 readings from the ADC minus tare weight (not set) divided
            // by the SCALE parameter (not set yet)

  scale.set_scale(2280.f);                      // this value is obtained by calibrating the scale with known weights; see the README for details
  scale.tare();               // reset the scale to 0

  Serial.println("After setting up the scale:");

  Serial.print("read: \t\t");
  Serial.println(scale.read());                 // print a raw reading from the ADC

  Serial.print("read average: \t\t");
  Serial.println(scale.read_average(20));       // print the average of 20 readings from the ADC

  Serial.print("get value: \t\t");
  Serial.println(scale.get_value(5));   // print the average of 5 readings from the ADC minus the tare weight, set with tare()

  Serial.print("get units: \t\t");
  Serial.println(scale.get_units(5), 1);        // print the average of 5 readings from the ADC minus tare weight, divided
            // by the SCALE parameter set with set_scale

  Serial.println("Readings:");
}
*/
void loop() {
 // Check if MQTT client has connected else reconnect
  if (!mqttClient.connected())
  {
    reconnect();
  }
  // Call the loop continuously to establish connection to the server
  mqttClient.loop();
 
  if (millis() - lastConnectionTime > postingInterval)
  {
    mqttpublish();
   
  }
}

void reconnect()
{
  // Loop until we're reconnected
  while (!mqttClient.connected())
  {
    Serial.print("Attempting MQTT connection...");
    // Connect to the MQTT broker
    if (mqttClient.connect("ArduinoWiFi101Client"))
    {
      Serial.println("connected");
    } else
    {
      Serial.print("failed, rc=");
      // Print to know why the connection failed
// See http://pubsubclient.knolleary.net/api.html#state for the failure code and its reason
      Serial.print(mqttClient.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying to connect again
      delay(5000);
    }
  }
}


void mqttpublish() {
  // Read temperature from DHT sensor
  //float t = dht.readTemperature(true);
  float wt = scale.get_units();
  Serial.println(wt);
 
  //String data = String(t, DEC);
  String data=String(wt);
 
  // Get the data string length
  int length = data.length();
  char msgBuffer[length];
  // Convert data string to character buffer
  data.toCharArray(msgBuffer,length+1);
  Serial.println(msgBuffer);
 
  mqttClient.publish("channels/294404/publish/fields/field1/<api key>",msgBuffer);
  Serial.println("Published");
  // note the last connection time
  lastConnectionTime = millis();
  scale.power_down();             // put the ADC in sleep mode
  delay(1000);
  scale.power_up();

}
