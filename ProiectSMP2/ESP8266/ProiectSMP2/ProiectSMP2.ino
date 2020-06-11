#include <ESP8266WiFi.h>
#include <PubSubClient.h> 
#include <ArduinoJson.h> 
#include "Credential.h"
#include <NTPClient.h>
#include <WiFiUdp.h>

#define NTP_OFFSET   0      // In seconds
#define NTP_INTERVAL 60 * 1000    // In miliseconds
#define NTP_ADDRESS  "europe.pool.ntp.org"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, NTP_ADDRESS, NTP_OFFSET, NTP_INTERVAL);
String timestampString;
long timestamp; 

int eCO2;
int tVOC;
float lux;
float humidity;
float pressure;
float temperature;

char buff[60]={0x00};
int j=0;
int k=0;
int ctr=0;
  
  


//IOT functions
WiFiClient wifiClient;
void wifiConnect() ;
void mqttConnect() ;
void callback(char* topic, byte* payload, unsigned int payloadLength);
PubSubClient client(server, 1883, callback, wifiClient);
void publishJson();




//setup function

void setup() 
{
  ctr=0;
  Serial.begin(115200); Serial.println();
  wifiConnect();
  timeClient.begin();
  mqttConnect();
  
}


void loop() 
{
  
   readAndSendData();
  if(ctr==-1)
  {
    delay(500);
    Serial.print("Enter deep sleep");
    ESP.deepSleep(0); //esp_deep_sleep_start();
    Serial.print("You should not be here");
    Serial.print("You should not be here");

 }

}

void readAndSendData()
{
   if(Serial.available()){
    buff[ctr]=Serial.read();
    Serial.print(buff[ctr]);   
    if (buff[ctr] == 'z') {
      Serial.print(buff[ctr]);
      Serial.print("Enter sleep mode z");   
      ESP.deepSleep(0);
    }
    ctr++;
   }
   if(ctr==60)
   { 
    processRawSensorValue();
    sendProcessedData();
    ctr=-1;
  
   }

  }

void processRawSensorValue()
{
    j=0;
    for(j;j<60;j+=10)
    {
      
      char _buff[11]={0x00};

      for(int l=0; l<10;l++)      
      {
        _buff[l]=buff[j+l];
      }
      _buff[10]='\0';
     
     if(j<=0) temperature= atoi(_buff);
     else if(j<=10) humidity=atoi(_buff);
     else if(j<=20) pressure=atoi(_buff);
     else if(j<=30) lux=atoi(_buff);
     else if(j<=40) eCO2=atoi(_buff);
     else if(j<=50) tVOC=atoi(_buff);

   
      }
}


void sendProcessedData(){
  
        timeClient.update();
        timestamp=  timeClient.getEpochTime();
        publishJson();
    
  }


/*
 * 
 * Publish Data
 * 
 * 
 */

void publishJson()
{
  
StaticJsonBuffer<300> jsonBuffer;
JsonObject& root = jsonBuffer.createObject();

JsonObject& object1 = root.createNestedObject("Sensor1");
object1["t"] = temperature/100; //C .2zecimale
object1["h"] = humidity/100; //% .2zecimale
object1["p"] = pressure/1000; //hPA .3 zecimale
object1["l"] = lux/100; //1ux .2 zecimale
object1["e"] = eCO2; // ppm
object1["T"] = tVOC; //ppm
object1["ts"]= timestamp;


 char buff[300];
 root.printTo(buff, sizeof(buff));
 Serial.println("publishing device metadata:"); Serial.println(buff);
 if (client.publish(eventTopic, buff)) 
 {
   Serial.println("device Publish ok");
 } 
 else 
 {
   Serial.print("device Publish failed:");
 }
}




/*
 * 
 * Connecting to wifi and mqtt
 * 
 * 
 */


void wifiConnect()
{
  Serial.print("Connecting to "); Serial.print(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.print("nWiFi connected, IP address: "); Serial.println(WiFi.localIP());

}

void mqttConnect() 
{
  if (!!!client.connected()) 
  {
    Serial.print("Reconnecting MQTT client to "); Serial.println(server);
    while (!!!client.connect(clientId, authMethod, token)) 
    {
      Serial.print(".");
      delay(500);
    }
    if (client.subscribe(cmdTopic)) 
    {
      Serial.println("subscribe to responses OK");
    } 
    else 
    {
      Serial.println("subscribe to responses FAILED");
    }
    Serial.println();
  }
}



void callback(char* topic, byte* payload, unsigned int payloadLength) 
{
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < payloadLength; i++) 
  {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  // Switch on the LED if an 1 was received as first character
  if (payload[0] == '1') 
  {
    digitalWrite(2, LOW);   // Turn the LED on (Note that LOW is the voltage level
    // but actually the LED is on; this is because
    // it is acive low on the ESP-01)
  } 
  else 
  {
    digitalWrite(2, HIGH);  // Turn the LED off by making the voltage HIGH
  }
  
}
