#include <Arduino.h>
#include <WiFi.h>
#include "time.h"
#include <PubSubClient.h>

#define wifi_ssid "ITS1"
#define wifi_password "0313537387"

#define mqtt_server "mqtt-cleen.ptpjb.com"
#define mqtt_port 1883
#define mqtt_user "bbtk"
#define mqtt_password "ebtbbtk@PJB"

#define TAG1 "TA-WS1-IR" // Iradian
#define TAG2 "TA-WS1-WS" // Wind Speed
#define TAG3 "TA-WS1-WD" // Wind Direction

//#define mqtt_server "mqtt.dioty.co"
//#define mqtt_port 1883
//#define mqtt_user "ujangsprr@gmail.com"
//#define mqtt_password "a1e2b36d"

//#define TAG1 "/ujangsprr@gmail.com/TA-WS1-IR" // Iradian
//#define TAG2 "/ujangsprr@gmail.com/TA-WS1-WS" // Wind Speed
//#define TAG3 "/ujangsprr@gmail.com/TA-WS1-WD" // Wind Direction

WiFiClient espClient;
PubSubClient client;

// ====== Global Variable ======
int count = 0;
long currentTime, lastTime;
int iradian;
int windDir;
int windSpeed;
String data;
String Send;

int counter = 0;
int lastIndex = 0;

String pieces[3];

char dataIr[60];
char dataWs[60];
char dataWd[60];

char TimeNow[30];

const char* ntpServer = "pool.ntp.org";
const long  gmtOffset_sec = 21600;
const int   daylightOffset_sec = 3600;

//======== Timing ========
unsigned long currentMillis;
unsigned long previousMillisSend = 0;
unsigned long previousMillisR = 0;
unsigned long previousMillisSD = 0;
const int IntervalSend = 1000;
