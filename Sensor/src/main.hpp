#include <Arduino.h>
#include <time.h>
#include <ESP8266WiFi.h>
#include <MySQL_Connection.h>
#include <MySQL_Cursor.h>
#include <Wire.h>
#include <BH1750.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

// 데이터베이스 서버 IP, user, password 설정
IPAddress server_addr(192,168,10,156);
char user[] = "node";
char password[] = "node";

// 데이터베이스 insert query 설정
char INSERT_syn[] = "insert into test.sensor (mac, light, pre, tem, hum, year, month, day, hour, min, sec) values";
char INSERT_val[100];
char INSERT_SQL[150];

// WiFi 설정
char ssid[] = "iptime";
char pwd[] = "";

WiFiClient client;
MySQL_Connection conn((Client *)&client);
MySQL_Cursor* cursor;

// 센서 변수 초기화
BH1750 LightSensor(0x23);
float light = 0.0;

Adafruit_BMP085 PreTemSensor;
float pressure = 0.0;
float temperature = 0.0;

DHT HumiditySensor(D5, DHT11);
float humidity = 0.0;

// 센서보드 장치 MAC 주소 변수 선언
String devmac;
char mac[20];

// 센서값 입력 일시 변수 선언
int year;
int month;
int day;
int t_hour;
int t_min;
int t_sec;

// 함수 선언
void wifi_set(void);
void insert_mod(void);
void getTime(time_t);
void readData(void);

int count;