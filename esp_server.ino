//#include <ESP8266WiFi.h>
//#include <WiFi.h>
#include <WiFiUdp.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#define MAX_CLIENT 3
WiFiServer server(80);
WiFiClient client[MAX_CLIENT];


IPAddress apIP(192,168,1,10);
IPAddress gateway(192,168,1,1);
IPAddress subnet(255,255,255,0);
const char* ssid = "diplomnik";
const char* password = "12345678";
WiFiUDP Udp;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE]; //буфер для хранения входящих пакетов
unsigned int localUdpPort = 80;  // local port to listen on
char  data[50];
void StartAPMode()
{
  delay(100);
  //WiFi.disconnect();
//  WiFi.mode(WIFI_STA);     // Режим клиента
//  WiFi.mode(WIFI_AP);      // Режим точки доступа
//  WiFi.mode(WIFI_AP_STA);  // Включены оба режима
  WiFi.mode(WIFI_AP_STA);
  WiFi.softAPConfig(apIP, apIP, subnet);
  WiFi.softAP(ssid, password);
  delay(500);
  server.begin();
  Serial.println("Server started");
}

void setup()
{
  Serial.begin(74880);
  StartAPMode();
  
  Serial.println("AP Mode Activated");
  Serial.println( IPAddress(192, 168, 1, 10));

  Udp.begin(localUdpPort);
  Serial.printf("Now Streaming at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  delay(1000);
}

void loop()
{     
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
    Udp.beginPacket(Udp.remoteIP(), localUdpPort);
    Udp.write(data);  
    Udp.endPacket();
   }
}
