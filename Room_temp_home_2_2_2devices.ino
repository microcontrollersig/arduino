// John v2
// Use this setup for home network
#include <ESP8266WiFi.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define myPeriodic 30 //in sec | Thingspeak pub is 15sec
#define ONE_WIRE_BUS 2  // DS18B20 on arduino pin2 corresponds to D4 on physical board

#define FIELD1 1    // Room temp
#define FIELD2 2    // Outside temp 
#define FIELD3 3    // Heat exchanger
#define FIELD4 4    // Solar panel 
#define FIELD5 5    // Roof space
#define FIELD6 6    // AirCon

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature DS18B20(&oneWire);


byte present = 0;

float prevTemp = 0;
const char* server =  "api.thingspeak.com";//"184.106.153.149";
String apiKey ="AOJ391BJ6P2P51ZW";
const char* MY_SSID ="MelbPC-NUE";        // Comment out to use @ Home
const char* MY_PWD ="peachspeak38";       // Comment out to use @ Home
//const char* MY_SSID ="BigPondFE3FD6";     // Comment out to use @ Melb-PC
//const char* MY_PWD ="5052B9674C";         // Comment out to use @ Melb-PC

int sent = 0;
int batch = 1;

void setup() {
  Serial.begin(115200);
  connectWifi();
}

void loop() {
  float temp, temp2, temp3, temp4, temp5, temp6;  //  more sensors

  DS18B20.requestTemperatures(); 
  temp = DS18B20.getTempCByIndex(0);    // Room temp
  temp2 = DS18B20.getTempCByIndex(1);   // Outside temp
  temp3 = DS18B20.getTempCByIndex(2);   // Heat exchanger    more sensors
  temp4 = DS18B20.getTempCByIndex(3);   // Solar panel 
  temp5 = DS18B20.getTempCByIndex(4);   // Roof space
  temp6 = DS18B20.getTempCByIndex(5);   // AirCon

  
  //String tempC = dtostrf(temp, 4, 1, buffer);   //handled in sendTemp()
  //Serial.print(String(sent)+" Temperature: ");    // Room temp
//  Serial.println(String(sent) + " - data sent");

  Serial.println(String(sent) + " - data sent this run");

  Serial.println("Batch of Sensor readings > " + String(batch));
  batch++;
  
  Serial.print(" Temperature:  ");    // Room temp
  Serial.println(temp);

  Serial.print(" Temperature2: ");    // Outside temp
  Serial.println(temp2);

  Serial.print(" Temperature3: ");     // Heat exchanger
  Serial.println(temp3);

  Serial.print(" Temperature4: ");    // Solar panel
  Serial.println(temp4);

  Serial.print(" Temperature5: ");    // Roof space
  Serial.println(temp5);
  
  Serial.print(" Temperature6: ");    // AirCon
  Serial.println(temp6);
  
//  Serial.println("Sample Sensor Sequence - " + String(batch));
//  batch++;
  
//  Serial.println(String(sent) + " - data sent");
  
  //if (temp != prevTemp)
  //{
  //sendTeperatureTS(temp);
  //prevTemp = temp;
  //}
  
  sendTeperatureTS(temp, FIELD1);           // Room temp
  int count = (int) ((myPeriodic+1)/2);
  Serial.print("Delay between temp uploads:");
  Serial.print(count);
  Serial.println("sec");
  while(count--)
    delay(1000);
  
  sendTeperatureTS(temp2, FIELD2);         // Outside temp
  count = (int) ((myPeriodic+1)/2);
  while(count--)
    delay(1000);

  sendTeperatureTS(temp3, FIELD3);         // Heat exchanger
  count = (int) ((myPeriodic+1)/2);
  while(count--)
    delay(1000);

  sendTeperatureTS(temp4, FIELD4);        // Solar panel
  count = (int) ((myPeriodic+1)/2);
  while(count--)
    delay(1000);

  sendTeperatureTS(temp5, FIELD5);
  count = (int) ((myPeriodic+1)/2);       // Roof space
    while(count--)
    delay(1000);
 
  sendTeperatureTS(temp6, FIELD6);        // AirCon
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
   Serial.println("WiFi Client connected - ");
   //Serial.println("WiFi Client connected - " + String(sent));
   //Serial.print(String(sent)+" Temperature: ");    // Room temp
   
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
   
   }    //end if
   sent++;
//  Serial.println("Sensor data sent "+String(sent) +("sent to Thinkspeak ");
   Serial.println("Sensor data sent "+String(sent));
 client.stop();
}       //end send


