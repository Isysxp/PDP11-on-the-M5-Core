/* ------------------------------------------------- */

#include "ESPTelnetStream.h"

/* ------------------------------------------------- */

#define SERIAL_SPEED  115200
#define INFRA_SSID    "Your SSID"
#define INFRA_PSWD    "Your Password"
//const char noecho[]={0xFF,0xFD,0x2D,0}; // IAC DO SUPPRESS-LOCAL-ECHO Required for Windows telnet client.
const uint8_t noecho[]={0xFF,0xFB,0x01,0};   // IAC WILL ECHO Seems to work for most clients
/*
IAC DO SUPPRESS-LOCAL-ECHO

        The sender of this command, generally a host computer, REQUESTS that a
client NVT terminal suspend the local echoing of text typed on its
keyboard. This request makes good sense only when the NVT and host are
operating in an asymmetric, half-duplex terminal mode with a
co-operating host. The command should have no effect on an NVT terminal
operating in full-duplex mode.
*/

/* ------------------------------------------------- */

ESPTelnetStream telnet;
ESPTelnetStream telnet1;

/* ------------------------------------------------- */

void telnetConnected(String ip) {
  Serial.print(ip);
  Serial.println(" connected.");
  telnet.print((char *)noecho);
  telnet1.print((char*)noecho);
}

void telnetDisconnected(String ip) {
  Serial.print(ip);
  Serial.println(" disconnected.");
}

void telnetReconnect(String ip) {
  Serial.print(ip);
  Serial.println(" reconnected.");
}

/* ------------------------------------------------- */

void TStart() {

  Serial.println("ESP Telnet server");

  WiFi.setHostname("M5PDP11");
  WiFi.mode(WIFI_STA);
  WiFi.begin(INFRA_SSID, INFRA_PSWD);
  while(WiFi.status() != WL_CONNECTED) {
    delay(100);
  }

  telnet.onConnect(telnetConnected);
  telnet.onDisconnect(telnetDisconnected);
  telnet.onReconnect(telnetReconnect);
  telnet1.onConnect(telnetConnected);
  telnet1.onDisconnect(telnetDisconnected);
  telnet1.onReconnect(telnetReconnect);

  Serial.print("Telnet.begin: ");
  if(telnet.begin()) {
    Serial.println("Successful");
  } else {
    Serial.println("Failed");
  }
  IPAddress ip = WiFi.localIP();
  Serial.print("IP:");
  Serial.print(ip);
  Serial.println("/M5PDP11");

  Serial.print("Telnet1.begin: ");
  if (telnet1.begin(2000)) {
      Serial.println("Successful");
  }
  else {
      Serial.println("Failed");
  }
}

/* ------------------------------------------------- */

void Tloop() {
  telnet.loop();
  telnet1.loop();
/*
  if(Serial.available() > 0) {
    telnet.write(Serial.read());
  }
  if (telnet.available() > 0) {    
    Serial.print(char(telnet.read()));
  }
*/
}

/* ------------------------------------------------- */
