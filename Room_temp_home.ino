//Mohan
// Use this setup for home network
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define myPeriodic 30 //in sec | Thingspeak pub is 15sec
#define ONE_WIRE_BUS 2  // DS18B20 on arduino pin2 corresponds to D4 on physical board

#define FIELD1 1
#define FIELD2 2

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);

// Sensor addresses
char sensor1_addr[8] = {'\x28', '\xFF', '\x5C', '\xCE', '\x8C', '\x16', '\x03', '\xAD'};
char sensor2_addr[8] = {'\x28', '\xFF', '\xBD', '\x9C', '\xB0',  '\x16', '\x04', '\x83'};
byte present = 0;

float prevTemp = 0;
const char* server =  "api.thingspeak.com";//"184.106.153.149";
String apiKey ="AOJ391BJ6P2P51ZW";
const char* MY_SSID ="MelbPC-NUE";        // "BigPondFE3FD6"; 
const char* MY_PWD ="peachspeak38";       //"5052B9674C";
int sent = 0;
void setup() {
  Serial.begin(115200);
  connectWifi();
}

void loop() {
  float temp, temp2;

  DS18B20.requestTemperatures(); 
  temp = DS18B20.getTempCByIndex(0);
  temp2 = DS18B20.getTempCByIndex(1);


  
  //String tempC = dtostrf(temp, 4, 1, buffer);//handled in sendTemp()
  Serial.print(String(sent)+" Temperature: ");
  Serial.println(temp);

  Serial.print(" Temperature2: ");
  Serial.println(temp2);
  
  //if (temp != prevTemp)
  //{
  //sendTeperatureTS(temp);
  //prevTemp = temp;
  //}
  
  sendTeperatureTS(temp, FIELD1);
  int count = (int) ((myPeriodic+1)/2);
  Serial.print("Delay between temp uploads:");
  Serial.println(count);
  while(count--)
    delay(1000);
  sendTeperatureTS(temp2, FIELD2);
  count = (int) ((myPeriodic+1)/2);
  while(count--)
    delay(1000);
  
}

void connectWifi()
{
  Serial.print("Connecting to "+*MY_SSID);
  WiFi.begin(MY_SSID, MY_PWD);
  while (WiFi.status() != WL_CONNECTED) {
  delay(1000);
  Serial.print(".");
  }
  
  Serial.println("");
  Serial.println("Connected");
  Serial.println("");  
}//end connect

void sendTeperatureTS(float tempReading, int field_number)
{  
   WiFiClient client;
  
   if (client.connect(server, 80)) { // use ip 184.106.153.149 or api.thingspeak.com
   Serial.println("WiFi Client connected ");
   
   String postStr = apiKey;
   postStr += "&field" + String(field_number) + "=";
   postStr += String(tempReading);
   postStr += "\r\n\r\n";

   //Serial.println(postStr);
   
   client.print("POST /update HTTP/1.1\n");
   client.print("Host: api.thingspeak.com\n");
   client.print("Connection: close\n");
   client.print("X-THINGSPEAKAPIKEY: " + apiKey + "\n");
   client.print("Content-Type: application/x-www-form-urlencoded\n");
   client.print("Content-Length: ");
   client.print(postStr.length());
   client.print("\n\n");
   client.print(postStr);
   delay(1000);
   
   }//end if
   sent++;
 client.stop();
}//end send


