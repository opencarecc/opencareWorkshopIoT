/*
  This sketch waits for an INPUT and then
  connects to the internet and prints
  the Arduino logo stored in WeMake's wiki

  To run this sketch you need:
  > Arduino UNO
  > Arduino Ethernet Shield
  > Arduino TinkerKit Shield
  > Tinkerkit LED module connected to O0
  > Tinkerkit BUTTON module connected to I0

  edited by WeMake 2017
  based on work by
  David A. Mellis
  Tom Igoe
  Adrian McEwen
 */

#include <SPI.h>
#include <Ethernet.h>
#include <TinkerKit.h>

// Enter a MAC address for your controller below.
// Newer Ethernet shields have a MAC address printed on a sticker on the shield
byte mac[] = { 0x90, 0xA2, 0xDA, 0x00, 0x00, 0x00 };
// if you don't want to use DNS (and reduce your sketch size)
// use the numeric IP instead of the name for the server:
//IPAddress server(74,125,232,128);  // numeric IP for Google (no DNS)
char server[] = "wiki.wemake.cc";    // name address for WeMake's wiki (using DNS)

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);

// Initialize the Ethernet client library
// with the IP address and port of the server
// that you want to connect to (port 80 is default for HTTP):
EthernetClient client;

TKButton button(I0);    // create the object 'button' that belongs to the 'TKButton' class
                        // and connect to I0

TKLed led(O0);    // create the object 'led' that belongs to the 'TKLed' class 
                  // and connect to O0

// flag to check if it's connected
boolean isConnected;

void setup() {
  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo only
  }

  led.off();    // turn LED off

  // start the Ethernet connection:
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Failed to configure Ethernet using DHCP");
    // no point in carrying on, so do nothing forevermore:
    // try to congifure using IP address instead of DHCP:
    Ethernet.begin(mac, ip);
  }
}

void loop()
{
  // if not connected to server wait for input
  if(isConnected==false){
    Serial.println("Waiting for input...");

    // if button pressed then connect
    if(button.pressed()==HIGH){
      connect();
    }
  }  
  
  // if there are incoming bytes available
  // from the server, read them and print them:
  if (client.available()) {
    char c = client.read();
    Serial.print(c);
  }

  if(isConnected==true){
    // if the server's disconnected, stop the client:
    if (!client.connected()) {
      Serial.println();
      Serial.println("disconnecting.");
      client.stop();
      isConnected = false;
    }
  }
}


// function to connect to server
void connect(){
  // give the Ethernet shield a second to initialize:
  delay(1000);
  Serial.println("connecting...");

  // if you get a connection, report back via serial:
  if (client.connect(server, 80)) {
    isConnected = true;
    Serial.println("connected");
    // Make a HTTP request:
    client.println("GET /w/success.txt HTTP/1.1");
    client.println("Host: wiki.wemake.cc");
    client.println("Connection: close");
    client.println();

    // Blink the LED when connected to server
    led.on();
    delay(700);
    led.off();
    delay(700);
    led.on();
    delay(700);
    led.off();
  }
  else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
}

