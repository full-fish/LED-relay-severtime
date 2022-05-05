#include <NTPClient.h>

#include <ESP8266WiFi.h>

#include <WiFiUdp.h>

//

const char *ssid = "*****";

const char *password = "******";

unsigned int deepsleep_Time = 0;

//

WiFiUDP udp;

NTPClient timeClient(udp, "kr.pool.ntp.org", 32400, 3500000);

//

​

void connectwifi() {

// Wi-Fi 연결 함수

WiFi.mode(WIFI_STA);

WiFi.begin(ssid, password);

while (WiFi.status() != WL_CONNECTED) {

delay(500);

// Serial.print(".");

}

/*Serial.println("");

Serial.print("Connected to ");

Serial.println(ssid);

Serial.print("IP address: ");

Serial.println(WiFi.localIP());*/

}

​

​

​

​

void setup() {

//Serial.begin(115200);

//

connectwifi(); // Wi-Fi 연결

​

timeClient.begin();

//

timeClient.update();

//

pinMode(D1, OUTPUT);

}

​

​

​

void loop() {

timeClient.update();

if (8 <= timeClient.getHours() && timeClient.getHours() < 17) {

digitalWrite(D1, LOW);

deepsleep_Time = (60 - timeClient.getMinutes()) * 60 + (30 - timeClient.getSeconds()); //깨어나는 시간을 0분 30초로 하기위해

deepsleep_Time *= 1e6;

deepsleep_Time *= 1.05;

// Deep-Sleep 시작 각 시간마다 0분 30초에 깨움 왜냐하면 deepsleep은 마이크로초라 최대가 71분이라서 약 한시간간격으로 하기 위해

ESP.deepSleep(deepsleep_Time); //

}

else {

digitalWrite(D1, HIGH);

// Moderm-Sleep 시작

WiFi.disconnect(); // Wi-Fi AP 연결 해제

WiFi.forceSleepBegin(); // Wi-Fi 종료

delay(10); // Wi-Fi가 종료될 때 까지 대기

// normal 모드로 복귀

if (timeClient.getMinutes() == 59 && timeClient.getSeconds() <= 30) { //1시간마다 복귀

WiFi.forceSleepWake(); // Wi-Fi 실행

connectwifi(); // Wi-Fi 연결

delay(20000);

}

}

delay(10000);

}