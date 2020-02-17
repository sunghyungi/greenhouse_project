#include "main.hpp"

void setup() {
  count = 0;
  pinMode(D6, OUTPUT);
  digitalWrite(D6, HIGH);             // 초기 셋팅 중 LED 켜짐

  Serial.begin(9600);
  Wire.begin();
  LightSensor.begin(BH1750::CONTINUOUS_HIGH_RES_MODE);
  PreTemSensor.begin();
  HumiditySensor.begin();
  
  readData();                         // 초기화 후 첫번째 값은 버림(ex 조도값은 첫번째 값이 0이 나오는 경우가 있음
  wifi_set();                         // 와이파이 셋팅
  delay(10000);                       // 디바이스 등록메뉴에서 쓰레기값을 읽어오지 않도록 일정 시간을 줌
  Serial.print(mac);
  configTime(9 * 3600, 0, "pool.ntp.org", "time.nist.gov");
  digitalWrite(D6, LOW);
}

void loop() {
  delay(59800);                       // 데이터베이스 업데이트 주기(1분))
  time_t now = time(nullptr);
  readData();                         // 센서값 읽어옴
  getTime(now);                       // 현재 일시를 받아옴
  insert_mod();                       // insert query 셋팅
  cursor = new MySQL_Cursor(&conn);
  cursor->execute(INSERT_SQL); 
  cursor->close();
  digitalWrite(D6, HIGH);             // sql실행 후 1회 깜빡임
  delay(200);
  digitalWrite(D6, LOW);

  count++;
  if(count == 59) ESP.reset();        // 1시간 간격 reset
}


// 함수 정의부

void wifi_set(void) {
  WiFi.begin(ssid, pwd);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
  }
  devmac = WiFi.macAddress();
  devmac.toCharArray(mac, devmac.length()+1);
  
  conn.connect(server_addr, 3306, user, password);
  cursor = new MySQL_Cursor(&conn);  
}

void getTime(time_t now) {
  struct tm *timeinfo;
  timeinfo = localtime(&now);
  year = timeinfo->tm_year + 1900;
  month = timeinfo->tm_mon + 1;
  day = timeinfo->tm_mday;
  t_hour = timeinfo->tm_hour;
  t_min = timeinfo->tm_min;
  t_sec = timeinfo->tm_sec;
}

void insert_mod() {
  sprintf(INSERT_val, "( '%s', %.2f, %.2f, %.2f, %.2f, %d, %d, %d, %d, %d, %d )", \
          mac, light, pressure, temperature, humidity, year, month, day, t_hour, t_min, t_sec);
  sprintf(INSERT_SQL, "%s %s", INSERT_syn, INSERT_val);
}

void readData() {
  light = LightSensor.readLightLevel();
  pressure = PreTemSensor.readPressure();
  temperature = PreTemSensor.readTemperature();
  humidity = HumiditySensor.readHumidity();
}