#include <Arduino.h>
#include <ESP8266WiFi.h>
#include "WIFI_SET.h" //wifi帳號密碼儲存位置

int ENB = 15;
int IN3 = 0;
int IN4 = 2;

WiFiServer server(80);

void setup() {
  Serial.begin(115200);
  pinMode(ENB, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  // 連接wifi
  WIFI();
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  Serial.println("");
  Serial.println("WiFi connected");
 
  server.begin();
  Serial.println("Server started");
 
  // 輸出IP位置
  Serial.print("Use this URL to connect: ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
}

void loop() {
  // 確認是否連接
  WiFiClient client = server.available();
  if (!client) 
  {
    return;
  }
 
  // 等待發送資料
  Serial.println("new client");
  while(!client.available())
  {
    delay(1);
  }

  // 讀取請求
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();
 
  // 依照請求去連接
  int value = 1;
  if (request.indexOf("/doorlock?=1") != -1)  
  {
    Serial.println("Door Lock=OPEN");
    analogWrite(ENB, 255);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(5000);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    value = 1;
  }
  if (request.indexOf("/doorlock?=0") != -1)  
  {
    Serial.println("Door Lock=CLOSE");
    analogWrite(ENB, 255);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(5000);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    value = 0;
  }
  /*if (request.indexOf("/door?=2") != -1)  
  {
    Serial.println("Door=STOP");
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    value = 2;
  }*/
  //回復
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println("");
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.println("<head><title>ESP8266 DOORLOCK Control</title></head>");
  client.print("Door lock is now: ");
 
  if(value == 1) 
  {
    client.print("OPEN");
  } 
  else if(value == 0)
  {
    client.print("CLOSE");
  }
  /*else
  {
    client.print("STOP");
  }*/
  client.println("<br><br>");
  client.println("Turn <a href=\"/doorlock?=1\">OPEN</a> Doorlock<br>");
  client.println("Turn <a href=\"/doorlock?=0\">CLOSE</a> Doorlock<br>");
  /*client.println("Turn <a href=\"/door?=2\">STOP</a> Door<br>");*/
  client.println("</html>");
 
  delay(1);
  Serial.println("Client disonnected");
  Serial.println("");
}
