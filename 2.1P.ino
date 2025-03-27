#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include <DHT.h>

// wifi credentials
char ssid[] = SECRET_SSID;
char pass[] = SECRET_PASS;
WiFiClient client;

// ThingSpeak settings
unsigned long myChannelNumber = SECRET_CHAN_NUM
const char * myWriteAPIKey = SECRET_APIKEY;

// sensor Setup
#define DHTPIN 2 // it's in pin D2
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);

void setup() {
    Serial.begin(115200);
    while (!Serial);

    Serial.println("starting");
    dht.begin();

    // connect to WiFi
    connectWiFi();

    ThingSpeak.begin(client); // start ThingSpeak
}

void loop() {
    // read temp and humidity and assign to variables
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // print on serial monitor
    Serial.print("Temperature: "); Serial.print(temperature); Serial.println(" °C");
    Serial.print("Humidity: "); Serial.print(humidity); Serial.println(" %");

    // send data to ThingSpeak API
    ThingSpeak.setField(1, temperature);
    ThingSpeak.setField(2, humidity);

    int status = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

    // display https codes
    if (status == 200) {
        Serial.println("data sent successfully");
    } else {
        Serial.print("error when sending, code: ");
        Serial.println(status);
    }

    delay(20000); // Wait 20 seconds before next update
}

// function to connect to wifi
void connectWiFi() {
    Serial.print("connecting to wifi");
    
    while (WiFi.status() != WL_CONNECTED) {
        WiFi.begin(ssid, pass);
        int attempt = 0;

        // Try for 10 seconds
        while (WiFi.status() != WL_CONNECTED && attempt < 10) {
            Serial.print(".");
            delay(1000);
            attempt++;
        }

        if (WiFi.status() == WL_CONNECTED) {
            Serial.println("\n connected to wifi successfully");
        } else {
            Serial.println("\n wifi connection failed, retrying in 5 secs");
            delay(5000); 
        }
    }
}
