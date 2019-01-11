#include <SPI.h>
#include <Dhcp.h>
#include <Dns.h>
#include <Ethernet.h>
#include <EthernetUdp.h>
#include <coap.h>

#include <DHT_U.h>
#include <DHT.h>
#include <Adafruit_Sensor.h>

#define DHTTYPE DHT22
#define DHTPIN 0 
DHT dht(DHTPIN, DHTTYPE);
#define LUM A0
unsigned long readTime;

byte mac[] = { 0xF8, 0xF0, 0x05, 0xEA, 0x51, 0x11 };

// UDP and CoAP class
EthernetUDP Udp;
Coap coap(Udp);
// GET humidity
void callback_hum(){
  
  char reponse[50];
  char char_h;
  
  float h = dht.readHumidity();

  if (isnan(h)) {
    Serial.println("Failed to read humidity from DHT sensor!");
    sprintf(reponse, "erreur lecture capteur");
    client.publish("humidite", reponse);
    return;
  }
  
  Serial.println(h);
  //dtostrf(h, 2, 0, char_h);
  
  sprintf(reponse, "%d", (int) h);
  client.publish("humidite", reponse);
  
}

// GET temperature
void callback_temp(){
  char reponse[50];
  float t = dht.readTemperature();

  if (isnan(t)) {
    Serial.println("Failed to read temperature from DHT sensor!");
    client.publish("temperature", reponse);
    return;
  }
  
  Serial.println(t);
  //dtostrf(t, 2, 2, char_t);
  //dtostrf(h, 2, 0, char_h);  
  sprintf(reponse, "%d" , (int) t);

  client.publish("temperature", reponse);

}
// GET luminosity
void callback_lum(){
  char reponse[50];
  int l = analogRead(LUM);
  if (isnan(l)) {
    Serial.println("Failed to read from light sensor!");
    sprintf(reponse, "erreur lecture capteur");
    client.publish("luminosite", reponse);
    return;
  }
  Serial.println(l);
  if(l>500){  
    sprintf(reponse, "Jour");
  }
  else if(l>200){
    sprintf(reponse, "Soir");
  }
  else {
    sprintf(reponse, "Nuit");
  }
  sprintf(reponse, "%d", l);
  client.publish("luminosite", reponse);
}

void setup() {
  Serial.begin(9600);

  Ethernet.begin(mac);
  Serial.print("My IP address: ");
  for (byte thisByte = 0; thisByte < 4; thisByte++) {
    Serial.print(Ethernet.localIP()[thisByte], DEC);
    Serial.print(".");
  }
  Serial.println();

  coap.server(callback_temp, "temperature");
  coap.server(callback_hum, "humidity");
  coap.server(callback_lum, "luminosite");
  
  Serial.println("Setup Response Callback");
  coap.response(callback_response);
  coap.start();
}

void loop() {
 
  Serial.println("Send Request");
  int msgid = coap.get(IPAddress(192, 168, 0, 1), 5683, "time");

  delay(1000);
  coap.loop();
}

