#include <DHT.h>
#include <ESP8266WiFi.h>
 
// replace with your channel’s thingspeak API key and your SSID and password
String apiKey = "<KEY>";
const char* ssid = "<SSID>";
const char* password = "<PWD>";
const char* server = "api.thingspeak.com";
 
#define DHTPIN D2
#define DHTTYPE DHT11 
 
DHT dht(DHTPIN, DHTTYPE);
WiFiClient client;

//#define DEBUG

 
void setup() 
{
#ifdef DEBUG
  Serial.begin(115200);
  delay(10);
#endif

pinMode(14, OUTPUT);     // Initialize the LED_BUILTIN pin as an output

dht.begin();
 
WiFi.begin(ssid, password);

#ifdef DEBUG
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
#endif

WiFi.begin(ssid, password);
 
while (WiFi.status() != WL_CONNECTED) 
{
delay(500);
#ifdef DEBUG
  Serial.print(".");
#endif
}

#ifdef DEBUG
  Serial.println("");
  Serial.println("WiFi connected");
#endif 
}
 
void loop() 
{
 
float h = dht.readHumidity();
float t = dht.readTemperature();

if (isnan(h) || isnan(t)) 
{
#ifdef DEBUG
  Serial.println("Failed to read from DHT sensor!");
#endif

return;
}
 
if (client.connect(server,80)) {
String postStr = apiKey;
postStr +="&field1=";
postStr += String(t);
postStr +="&field2=";
postStr += String(h);
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


#ifdef DEBUG 
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" degrees Celsius Humidity: ");
  Serial.print(h);
  Serial.println("Sending data to Thingspeak");
#endif
}
client.stop();
#ifdef DEBUG  
Serial.println("Waiting 20 secs");
#endif
// thingspeak needs at least a 15 sec delay between updates
// 20 seconds to be safe
digitalWrite(14, HIGH);
delay(30000);
digitalWrite(14, LOW);
delay(30000);

}
