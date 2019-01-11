#include <SPI.h>
#include <Ethernet.h>

byte mac[] = { 0xF8, 0xF0, 0x05, 0xEA, 0x51, 0x11 };

// penser à faire la commande :
// ip a add 192.168.0.1/30 dev [où l'on a branché l'arduino]
IPAddress ip(192, 168, 0, 1);

EthernetServer server(80);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; 
  } 
  Serial.println("server connected");

  Ethernet.begin(mac, ip);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true) {
      delay(1); 
    }
  }
  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet cable is not connected.");
  }

  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
}


void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n' && currentLineIsBlank) {
          client.println("hello world");
          break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }

    delay(1);
    client.stop();
    Serial.println("client disconnected");
  }
}
