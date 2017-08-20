//motor = 12/D6
//dht11 = 13/D7
//moisture=A0
//rain sensor = 5/D1


#include<dht.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
const char* ssid = "********";//your ssid name
const char* password = "*******";//ssid password
#define mqtt_server  "mqtt.dioty.co"
int dht11 = 13;
//int value;
dht DHT;
float value;
int dht11_temperature,dht11_humidity,check,value1,rain,flame,motion;
char moisture[50],temperature_value[50],humidity_value[50],setmoisture[2];
WiFiClient espClient;
PubSubClient client(espClient);

void setup_wifi()
{

  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
int soil_moisture()
{
  value=analogRead(A0);
  value1=(value/10);
  
 return value1;
}
int temperature()
{
  DHT.read11(dht11);
  dht11_temperature=DHT.temperature;
  return dht11_temperature;
}

int humidity()
{
  DHT.read11(dht11);
  dht11_humidity=DHT.humidity;
  return dht11_humidity;
  
}
int rainsensor()
{
  rain=digitalRead(5);
  return rain;
}



void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
//  char* a=topic;
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  } 
  Serial.println();
  if(strcmp(topic,"/taditarun123@gmail.com/led1")==0)
  {
    if((char)payload[0]=='t')
    {
  
    digitalWrite(12, HIGH); 
    
  } else
  {
    digitalWrite(12, LOW);
  } 
  }
   if(strcmp(topic,"/taditarun123@gmail.com/publish")==0)
    {

    for (int i = 0; i < length; i++)
  {
    Serial.print((char)payload[i]);
    setmoisture[i]=(char)payload[i];
  }
  
  Serial.println("");
    }
    check = atoi(setmoisture);
    Serial.println(check);

  
}


void reconnect()
{
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
     client.connect("clientId","username","password");// user name and password is provided by mqtt broker and repalce the topics with your one.
    // Attempt to connect
    if (client.connect(clientId.c_str())) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("/taditarun123@gmail.com/temperature",temperature_value);
  client.publish("/taditarun123@gmail.com/humidity",humidity_value);
  client.publish("/taditarun123@gmail.com/soil",moisture);
      client.subscribe("/taditarun123@gmail.com/led1");
      client.subscribe("/taditarun123@gmail.com/publish");

      // ... and resubscribe
      //client.subscribe("inTopic");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() {
  // pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  Serial.begin(115200);
  pinMode(12,OUTPUT);

  setup_wifi();
  client.setServer(mqtt_server, 1883);
  //  client.setCallback(callback);
}

void loop() {

  if (!client.connected())
  {
    reconnect();
    
  }
  
 temperature();
 humidity();
 soil_moisture();
 rainsensor();


 
 Serial.print("moisture=");
 Serial.println(value1);
 Serial.print("temperature=");
 Serial.println(dht11_temperature);
 Serial.print("humidity=");
 Serial.println(dht11_humidity);
 Serial.print("rain value=");
 Serial.println(rain);
  client.loop();
snprintf(moisture,50,"Moisture=%d %",value1);
 snprintf(temperature_value,50,"Temperature=%d C",dht11_temperature);
  snprintf(humidity_value,50,"Humidity=%d %",dht11_humidity);
client.setCallback(callback);
Serial.print("check value=");
Serial.print(check);
client.publish("/taditarun123@gmail.com/temperature",temperature_value);
  client.publish("/taditarun123@gmail.com/humidity",humidity_value);
  client.publish("/taditarun123@gmail.com/soil",moisture);
  delay(1000);

}
