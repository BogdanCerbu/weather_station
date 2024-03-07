/**
 * WiFi Web Server Light Sensor (adapted from WiFi Web Server LED Blink by Tom Igoe)
 * 
 * A simple web server that lets you read a light sensor value via the web. It
 * uses the WiFi module to connect to a WiFi network and creates a server that
 * listens for incoming HTTP requests. If the IP address of your board is
 * yourAddress:
 * 
 * - http://yourAddress:serverPort/ returns the light sensor value
 * 
 * Configuration:
 * - Light sensor: analog pin A0 (lightPin)
 * - Server port: 8080 (serverPort) - Change this value to adjust the server port
 * - Wait time: 500 ms (waitWiFiMs) - Change this value to adjust the delay between
 * WiFi connection attempts
 * - Wait time: 1 ms (waitClientMs) - Change this value to adjust the delay between
 * client checks
 */

#define DEBUG // Comment this line to disable debug output
#define DEBUG_BAUDS 115200 // Debug output baud rate
#ifdef DEBUG
    #define DEBUG_BEGIN Serial.begin(DEBUG_BAUDS)
    #define DEBUG_PRINT(x) Serial.print(x)
    #define DEBUG_PRINTLN(x) Serial.println(x)
#else
    #define DEBUG_BEGIN
    #define DEBUG_PRINT(x)
    #define DEBUG_PRINTLN(x)
#endif

#define SECRET_SSID "network" // Change this value to your WiFi network name
#define SECRET_PASS "password" // Change this value to your WiFi network password

#include <WiFiS3.h>

const int lightPin = A0; // Light sensor PIN
const int serverPort = 8080; // Server port. Can be any port from 1 to 65535
const int waitWiFiMs = 500; // Delay between WiFi connection attempts
const int waitClientMs = 1; // Delay between client checks
WiFiServer server(serverPort); // Create a server on the specified port

void setup()
{ // Setup, runs only once, after reset or power on:
    DEBUG_BEGIN; // Start serial monitor, if DEBUG is enabled
    pinMode(lightPin, INPUT); // Set light sensor pin as input

    if (WiFi.status() == WL_NO_MODULE) { // Check if the WiFi module is available
        DEBUG_PRINTLN("Communication with WiFi module failed!"); // Print error message
        while (true); // Stay here forever
    }

    WiFi.begin(SECRET_SSID, SECRET_PASS); // Connect to the WiFi network
    while (WiFi.status() != WL_CONNECTED) { // Wait for the WiFi connection
        delay(waitWiFiMs); // If not connected, wait and try again
        DEBUG_PRINT("."); // Print a dot to show the connection is in progress
    }
    DEBUG_PRINTLN(""); // Print a new line
    DEBUG_PRINTLN("WiFi connected"); // Print a message to show the connection is successful
    DEBUG_PRINT("IP address: ");
    DEBUG_PRINTLN(WiFi.localIP()); // Print the IP address

    server.begin(); // Start the server
    DEBUG_PRINTLN("Server started"); // Print a message to show the server is running
}


void loop()
{ // Main loop, runs code repeatedly, after setup, until reset or power off:
    WiFiClient client = server.available(); // Check if a client has connected
    if (!client) { // If no client has connected, return to the beginning of the loop
        return;
    }
    DEBUG_PRINTLN("New client"); // Print a message to show a new client has connected
    while (!client.available()) { // Wait for the client to send data
        delay(waitClientMs); // If no data is available, wait and try again
    }
    String request = client.readStringUntil('\r'); // Read the first line of the request
    DEBUG_PRINTLN("Request: " + request); // Print the request to the serial monitor
    client.flush(); // Clear the client buffer
    client.println("HTTP/1.1 200 OK"); // Send a response to the client
    client.println("Content-Type: text/html");
    client.println("");
    client.println("<!DOCTYPE HTML>");
    client.println("<html lang=\"en\">");
    client.println("<body>");
    client.println("<h1>Light Sensor</h1>");
    // Add light sensor value to the response
    client.println("<p>Light sensor value: " + String(analogRead(lightPin)) + "</p>");
    client.println("</body>");
    client.println("</html>");
    client.stop(); // Close the connection
}
