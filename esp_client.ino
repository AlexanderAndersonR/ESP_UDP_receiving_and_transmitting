//#include <Wire.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid = "diplomnik";
const char* password = "12345678";
//Wi-Fi
WiFiUDP Udp;
IPAddress apIP(192,168,1,10);
unsigned int localUdpPort = 80;  // local port to listen on
char incomingPacket[5];  // buffer for incoming packets
char  data[50];
char con[] = "conected";
bool connected_bool = false;
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];
void setup() 
{
  //Настройка и запуск Wi-Fi
Serial.begin(74880);
Serial.printf("Connecting to %s ", ssid);
WiFi.begin(ssid, password);delay(7000);
if (WiFi.status() != WL_CONNECTED)
{
  delay(500);
  Serial.println("Not connected");
}
  else 
{
  Serial.println(" connected");
  delay(1000);
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  Serial.println(apIP);
  
  Udp.beginPacket(apIP, 80);
  Udp.write(con);  
  Udp.endPacket();
   
  connected_bool = true;
}
}

void loop() 
{
if (WiFi.status() != WL_CONNECTED)
{
  delay(500);
  Serial.println("Not connected");
  connected_bool = false;
}
 else 
{
  if (!connected_bool)
  {
  Serial.println(" connected");
  delay(1000);
  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  Serial.println(apIP);

  Udp.beginPacket(apIP, 80);
  Udp.write(con);  
  Udp.endPacket();
  connected_bool = true;
  }
    int packetSize = Udp.parsePacket();//delay(autodelay);
  if (packetSize)
  { 
    int len = Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    if (len > 0)
    {
      packetBuffer[len] = 0;
    }
    Serial.println(packetBuffer);
  }
    while(Serial.available()>0)
   {
    String data_str = Serial.readStringUntil('\n');
    data_str.toCharArray(data,50);
    Udp.beginPacket(apIP, 80);
    Udp.write(data);  
    Udp.endPacket();
   }
}
}
