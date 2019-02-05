#include <WiFi.h>

const char* ssid     = "Tufts_Wireless";
const char* password = "";

// Public URL
// http://phant.it.tufts.edu/streams/pgwXVQg9zKF9Xb2kzA6dh3E30vo
// https://phant.it.tufts.edu/input/pgwXVQg9zKF9Xb2kzA6dh3E30vo?private_key=lEderpEYVRSMj2pPBW7zuE3Ea7b&gy_x=0&gy_y=0&gy_z=261


const char* title = "Posture Stream";    //Stream title (not used in the sketch)
const char* alias = "posture_stream";          // stream alias: you can see the data by accessing silklab.fctn.io:8080/big_t
const char* host = "phant.it.tufts.edu"; 
const char* streamId   = "pgwXVQg9zKF9Xb2kzA6dh3E30vo"; // the public key
const char* privateKey = "lEderpEYVRSMj2pPBW7zuE3Ea7b"; // private key
// Delete key: qJlz4aJb8WcAwG7pdoencxexJ89
//
//const char* title = "Test Stream";    //Stream title (not used in the sketch)
//const char* alias = "tes";          // stream alias: you can see the data by accessing silklab.fctn.io:8080/big_t
//const char* host = "phant-dev.it.tufts.edu"; 
//const char* streamId   = "lNgVDkM4o0TGQ0D41pXEu6w0PYKP"; // the public key
//const char* privateKey = "6Rzx6gYbVXTnD7zV3Bbdiej5PXMP"; // private key

void setup()
{
  // As usual start serial for debugging purposes
    Serial.begin(115200);
    delay(10);

    // Gemerate random 
    randomSeed(analogRead(0));

    // We start by connecting to a WiFi network

    Serial.println();
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
}

// A fake temperature value to log
int GyX, GyY, GyZ;

void loop()
{
    delay(2000);
    // let's pretend the temp increases of 1 every 5 seconds
    GyX = random(-5, 5);
    GyY = random(-5, 5);
    GyZ = random(-5, 5);

    Serial.print("connecting to ");
    Serial.println(host);

    // Use WiFiClient class to create TCP connections
    WiFiClient client;
    const int httpPort = 80;
    if (!client.connect(host, httpPort)) {
        Serial.println("connection failed");
        return;
    }

    // We now create a URI for the request
    String url = "/input/";
    url += streamId;
    url += "?private_key=";
    url += privateKey;
    url += "&gy_x=";    
    url += GyX;
    url += "&gy_y=";    
    url += GyY;
    url += "&gy_z=";    
    url += GyZ;

    Serial.print("Requesting URL: ");
    Serial.println(url);

    // This will send the request to the server
    // we built the URI (here called url) before, now we include it in the HTTP request
    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
    // Let's wait and see if we get a HTTP response from the server (database)
    unsigned long timeout = millis();

    // check if response is present
    while (client.available() == 0) {
      // but wait no more than 5 seconds
        if (millis() - timeout > 5000) {
            Serial.println(">>> Client Timeout !");
            client.stop();
            return;
        }
    }

    // Read all the lines of the reply from server and print them to Serial
    while(client.available()) {
        String line = client.readStringUntil('\r');
        Serial.print(line);
    }

    Serial.println();
    Serial.println("closing connection");
}
