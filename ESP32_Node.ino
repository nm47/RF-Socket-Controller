/*
  Basic ESPiLight transmit example

  https://github.com/puuu/espilight
*/

#include <ESPiLight.h>
#include <WiFi.h>
#include<WiFiServer.h>
#include<WiFiClient.h>

#define TRANSMITTER_PIN 32
#define PORT 23
#define SENDREPEATS 2

WiFiClient RemoteClient;
WiFiServer server(PORT);
ESPiLight rf(TRANSMITTER_PIN);
IPAddress Ip(192, 168, 1, 123);
IPAddress NMask(255, 255, 255, 0); // netmask
IPAddress GWDNS(192, 168, 1, 1); // gateway and dns

const char* ssid = "WIN_5615";
const char* passwd = "2948527807";

void setup() {
  Serial.begin(115200);
  pinMode(TRANSMITTER_PIN, OUTPUT);
  WiFi.config(Ip, GWDNS, NMask, GWDNS);
  WiFi.begin(ssid,passwd);
  WiFi.setSleep(false);

  ReconnectIfDown();
  Serial.println("Connected");
  server.begin();
}


void loop() {
  CheckForConnections();
  ReconnectIfDown();
}

void send(int socket, int state) {  
  for (int i = 0; i < SENDREPEATS; i++){
    switch (socket) {
      case 1:
        if (state == 1)rf.send("clarus_switch", "{\"id\":\"A2\",\"unit\":48,\"on\":1}");
        else {rf.send("clarus_switch", "{\"id\":\"A2\",\"unit\":48,\"off\":1}");}
        break;
      case 2:
        if (state == 1)rf.send("clarus_switch", "{\"id\":\"B1\",\"unit\":48,\"on\":1}");
        else {rf.send("clarus_switch", "{\"id\":\"B1\",\"unit\":48,\"off\":1}");}
        break;
      case 3:
        if (state == 1)rf.send("clarus_switch", "{\"id\":\"C3\",\"unit\":48,\"on\":1}");
        else {rf.send("clarus_switch", "{\"id\":\"C3\",\"unit\":48,\"off\":1}");}
        break;
      case 4:
        if (state == 1)rf.send("clarus_switch", "{\"id\":\"D3\",\"unit\":48,\"on\":1}");
        else {rf.send("clarus_switch", "{\"id\":\"D3\",\"unit\":48,\"off\":1}");}
        break;
      case 5:
        if (state == 1)rf.send("clarus_switch", "{\"id\":\"E3\",\"unit\":48,\"on\":1}");
        else {rf.send("clarus_switch", "{\"id\":\"E3\",\"unit\":48,\"off\":1}");}
        break;
      case 49:
        rf.send("clarus_switch", "{\"id\":\"A2\",\"unit\":48,\"off\":1}");
        break;
      case 50:
        rf.send("clarus_switch", "{\"id\":\"B1\",\"unit\":48,\"off\":1}");
        break;
      case 51:
        rf.send("clarus_switch", "{\"id\":\"C3\",\"unit\":48,\"off\":1}");
        break;
      case 52:
        rf.send("clarus_switch", "{\"id\":\"D3\",\"unit\":48,\"off\":1}");
        break;
      case 53:
        rf.send("clarus_switch", "{\"id\":\"E3\",\"unit\":48,\"off\":1}");
        break;
        delay(50);
	  }
  }
}
 
void CheckForConnections()
{
  if (server.hasClient())
  {
    // If we are already connected to another computer, 
    // then reject the new connection. Otherwise accept
    // the connection. 
    if (RemoteClient.connected())
    {
      Serial.println("Connection rejected");
      server.available().stop();
    }
    else
    {
      Serial.println("Connection accepted");
      RemoteClient = server.available();
      ReadData();
    }
  }
}
void ReconnectIfDown(){
  int x = 0;
  while (WiFi.status() != WL_CONNECTED) {
     delay(1000);
     Serial.print(".");
     x++;
     if (x % 5 == 0)WiFi.reconnect();
  }
}
void ReadData(){
   uint8_t ReceiveBuffer[30];
   int n = 0;
   int request[2];
   
   while (RemoteClient.connected() && RemoteClient.available())
   {
      char c = RemoteClient.read();
      int i = c-'0';
      request[n] = i;
      Serial.print(i);
      n++;
   }
   if(n==1)request[1]=1;
   RemoteClient.stop();
   Serial.println("\nClient Disconnected");
   send(request[0],request[1]);
}
