#include <SPI.h>
#include <Ethernet.h>
#include "HX711.h"

#define DOUT  3
#define CLK  2

HX711 scale(DOUT, CLK);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 0, 24);

float calibration_factor = -7050;
float ActualWeight;
//double FinalWeight;
int FinalWeight;


const double LB2GRM = 453.592;

EthernetServer server(80);

void setup()
{
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  // start the Ethernet connection and the server:
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print("server is at ");
  Serial.println(Ethernet.localIP());
  Serial.println(" ");

  scale.set_scale(calibration_factor);
  scale.tare();  //Reset the scale to 0

  Serial.println("Readings:");
}

/*void loop(){
  Ethernet_Class();
 // LoadCell();
} */

//void Ethernet_Class() {


void loop(){
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client) {
    Serial.println("new client");
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n' && currentLineIsBlank) {
          // send a standard http response header
          client.println("HTTP/1.1 200 OK");
          client.println("Content-Type: application/json");
          client.println("Connection: close");  // the connection will be closed after completion of the response
          client.println("Refresh: 1");  // refresh the page automatically every 5 sec
          client.println();
          //client.println("<!DOCTYPE HTML>");
          //client.println("<html>");
          // output the value of each analog input pin

          int analogChannel = 0;
          LoadCell();
          client.print(FinalWeight);
          /*for (int analogChannel = 0; analogChannel < 6; analogChannel++) {
            int sensorReading = analogRead(analogChannel);
            //client.print("analog input ");
            //client.print(analogChannel);
            
            //client.print("Actual weight is ");
            LoadCell();
            client.print(ActualWeight);
            //client.print(sensorReading);
            client.println("<br />");
          }*/
          //client.println("</html>");
          break;
        }
        if (c == '\n') {
          // you're starting a new line
          currentLineIsBlank = true;
        } else if (c != '\r') {
          // you've gotten a character on the current line
          currentLineIsBlank = false;
        }
      }
    }
    // give the web browser time to receive the data
    delay(1);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
    Ethernet.maintain();
  }
}


