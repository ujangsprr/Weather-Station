#include "Header.h"

void ReceiveData(){
  while(Serial2.available() > 0){
    delay(10);
    char d = Serial2.read();
    data += d;
  }
  
  if (data.length() > 0){
    Serial.println(data);

    iradian = getValue(data,',',0).toInt();
    windSpeed = getValue(data,',',1).toInt();
    windDir = getValue(data,',',2).toInt();
    
    data = "";
  }
}

void SendSerial(){
  Serial2.println(TimeNow);
}

String getValue(String dataA, char separator, int index) 
{ 
  int found = 0; 
  int strIndex[] = {0, -1}; 
  int maxIndex = dataA.length()-1; 
 
  for(int i=0; i<=maxIndex && found<=index; i++){ 
    if(dataA.charAt(i)==separator || i==maxIndex){ 
        found++; 
        strIndex[0] = strIndex[1]+1; 
        strIndex[1] = (i == maxIndex) ? i+1 : i; 
    } 
  } 
 
  return found>index ? dataA.substring(strIndex[0], strIndex[1]) : ""; 
}

void LocalTime()
{
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }
  strftime(TimeNow,30, "%H:%M:%S %x", &timeinfo); 
  Serial.println(TimeNow);
}

void SetData(){
  LocalTime();
  sprintf(dataIr, "%d", iradian);
  sprintf(dataWs, "%d", windSpeed);
  sprintf(dataWd, "%d", windDir);
}


void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(wifi_ssid);

  WiFi.begin(wifi_ssid, wifi_password);

  if (WiFi.status() != WL_CONNECTED) {
    Serial.print("WIFI CONNECTING");
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
    delay(500);
  }
  if (WiFi.status() == WL_CONNECTED) {
    Serial.print("WIFI CONNECTED");
    delay(1000);
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  Serial.println();
}

void sendData() {
  SetData();
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  client.publish(TAG1, dataIr);
  client.publish(TAG2, dataWs);
  client.publish(TAG3, dataWd);
  delay(100);
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  Serial2.begin(9600);

  setup_wifi();
  client.setClient(espClient);
  client.setServer(mqtt_server, mqtt_port);

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  LocalTime();
//  client.setCallback(callback);
}

void loop() {
  // put your main code here, to run repeatedly:
  SendSerial();

  currentMillis = millis();
  if (currentMillis - previousMillisR >= 500) {
    previousMillisR = currentMillis;
    ReceiveData();
  }
  
  currentMillis = millis();
  if (currentMillis - previousMillisSend >= IntervalSend) {
    previousMillisSend = currentMillis;
    sendData();
  }

//  Serial.println(dataIr);
//  Serial.println(dataWs);
//  Serial.println(dataWd);
}
