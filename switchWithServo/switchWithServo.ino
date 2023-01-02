#include <WiFi.h>
#include <Servo.h>
#include <SPI.h>
#include <RFID.h>



#define SS_PIN    21
#define RST_PIN   22
#define SIZE_BUFFER     18
#define MAX_SIZE_BLOCK  16
#define greenPin     12
#define redPin       32


RFID rfid(SS_PIN, RST_PIN);
int validCard[][5] = {
  {52, 131, 231, 246, 166},
  {145, 20, 209, 36, 112},
  {3, 94, 69, 144, 136}
};

int serNum[5];
bool isValid = false;

int pos;


Servo myservo,myservo1;

// Replace with your network credentials
const char* ssid = "UIU-STUDENT";
const char* password = "12345678";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String output26State = "Available";
String output27State = "Available";

// Assign output variables to GPIO pins
const int output26 = 13;
const int output27 = 12;

// Current time
unsigned long currentTime = millis();
// Previous time
unsigned long previousTime = 0;
// Define timeout time in milliseconds (example: 2000ms = 2s)
const long timeoutTime = 2000;

void setup() {
  myservo.attach(output26);
  myservo1.attach(output27);
  Serial.begin(115200);
  SPI.begin();
  rfid.init();

  // Connect to Wi-Fi network with SSID and password
  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  // Print local IP address and start web server
  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void loop() {
  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    currentTime = millis();
    previousTime = currentTime;
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    while (client.connected() && currentTime - previousTime <= timeoutTime) {  // loop while the client's connected
      currentTime = millis();
      if (client.available()) {             // if there's bytes to read from the client,
        char c = client.read();             // read a byte, then
        Serial.write(c);                    // print it out the serial monitor
        header += c;
        if (c == '\n') {                    // if the byte is a newline character
          // if the current line is blank, you got two newline characters in a row.
          // that's the end of the client HTTP request, so send a response:
          if (currentLine.length() == 0) {
            // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
            // and a content-type so the client knows what's coming, then a blank line:
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println("Connection: close");
            client.println();

            Serial.println("Check RFID CARD : ");

            if (rfid.isCard()) {
              Serial.println("Tag detected");
              if (rfid.readCardSerial()) {

                for (int x = 0; x < sizeof(validCard); x++) {
                  for (int i = 0; i < sizeof(rfid.serNum); i++ ) {
                    if (rfid.serNum[i] != validCard[x][i]) {
                      isValid = false;
                      break;
                    }
                    else {
                      isValid = true;
                    }
                  }
                  if (isValid) break;
                }

              }

            }



            Serial.print("IS Valid : ");
            Serial.println(isValid);

            Serial.println("Verify Done");

            delay(3000);

            if (isValid) {
              // turns the GPIOs on and off
              if (header.indexOf("GET /26/on") >= 0) {
                //Serial.println("GPIO 26 on");
                output26State = "No Available";
                //digitalWrite(output26, HIGH);

                for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
                  // in steps of 1 degree
                  myservo.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(15);

                }

                delay(500);


                for (pos = 180; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
                  // in steps of 1 degree
                  myservo.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(15);

                }

                isValid = false;
              }

               else if (header.indexOf("GET /27/on") >= 0) {
               
                output27State = "No Available";
                

                for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
                  // in steps of 1 degree
                  myservo1.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(15);

                }

                delay(500);


                for (pos = 180; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
                  // in steps of 1 degree
                  myservo1.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(15);

                }

                isValid = false;
              }

            }


            else
            {
              if (header.indexOf("GET /26/off") >= 0) {
                //Serial.println("GPIO 26 off");
                output26State = "Available";
                for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
                  // in steps of 1 degree
                  myservo.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(15);

                }

                delay(500);


                for (pos = 180; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
                  // in steps of 1 degree
                  myservo.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(15);
                }

              }

              else if (header.indexOf("GET /27/off") >= 0) {
        
                output27State = "Available";
                for (pos = 0; pos <= 180; pos += 1) { // goes from 0 degrees to 180 degrees
                  // in steps of 1 degree
                  myservo1.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(15);

                }

                delay(500);


                for (pos = 180; pos >= 0; pos -= 1) { // goes from 0 degrees to 180 degrees
                  // in steps of 1 degree
                  myservo1.write(pos);              // tell servo to go to position in variable 'pos'
                  delay(15);
                }

              }

            }



            /*else if (header.indexOf("GET /26/off") >= 0) {
              Serial.println("GPIO 26 off");
              output26State = "off";
              digitalWrite(output26, LOW);
              }*/

            /* else if (header.indexOf("GET /27/on") >= 0) {
               Serial.println("GPIO 27 on");
               output27State = "on";
               digitalWrite(output27, HIGH);
              }

              else if (header.indexOf("GET /27/off") >= 0) {
               Serial.println("GPIO 27 off");
               output27State = "off";
               digitalWrite(output27, LOW);
              }*/


            // Display the HTML web page
            client.println("<!DOCTYPE html><html>");
            client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
            client.println("<link rel=\"icon\" href=\"data:,\">");
            // CSS to style the on/off buttons
            // Feel free to change the background-color and font-size attributes to fit your preferences
            client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
            client.println(".button { background-color: #4CAF50; border: none; color: white; padding: 16px 40px;");
            client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
            client.println(".button2 {background-color: #555555;}</style></head>");

            // Web Page Heading
            client.println("<body><marquee><h3>As-Salamu Alaykum</h3></marquee>");
            client.println("<br><h1>WELCOME TO </h1><br>");
            client.println("<h2>UIU LIBARY</h2>");

            // Display current state, and ON/OFF buttons for GPIO 26
            client.println("<p>BOOK " + output26State + "</p>");
            // If the output26State is off, it displays the ON button
            if (output26State == "Available") {
              client.println("<p><a href=\"/26/on\"><button class=\"button\">OUT THE BOOK</button></a></p>");
            } else if(output26State == "No Available") {
              client.println("<p><a href=\"/26/off\"><button class=\"button button2\">IN THE BOOK</button></a></p>");
            }

            // Display current state, and ON/OFF buttons for GPIO 27
            client.println("<p>BOOK " + output27State + "</p>");
            // If the output27State is off, it displays the ON button
            if (output27State == "Available") {
              client.println("<p><a href=\"/27/on\"><button class=\"button\">OUT THE BOOK</button></a></p>");
            } else if(output27State == "No Available") {
              client.println("<p><a href=\"/27/off\"><button class=\"button button2\">IN THE BOOK</button></a></p>");
            }
            client.println("</body></html>");

            // The HTTP response ends with another blank line
            client.println();
            // Break out of the while loop
            break;
          } else { // if you got a newline, then clear currentLine
            currentLine = "";
          }
        }
      else if (c != '\r') {  // if you got anything else but a carriage return character,
        currentLine += c;      // add it to the end of the currentLine
      }
    }
  }
  // Clear the header variable
  header = "";
  // Close the connection
  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
}
}
