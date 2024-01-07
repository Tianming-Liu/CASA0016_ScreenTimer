#include "WiFiS3.h"
#include <PubSubClient.h>

// WiFi信息
char ssid[] = "PLUSNET-104SW";   // 你的网络SSID
char pass[] = "3SquadronRE";     // 你的网络密码

char receivedValue[50];  // 用于存储接收到的MQTT消息的变量
int durationSeconds = -1;     // 存储从消息中提取的秒数

WiFiClient wifiClient;
PubSubClient client(wifiClient);

// MQTT服务器信息
const char* mqtt_server = "mqtt.cetools.org";
const int mqtt_port = 1883;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  
  // 清空receivedValue以便存储新的消息
  memset(receivedValue, 0, sizeof(receivedValue));

  // 确保接收到的消息不会超过receivedValue的大小
  int size = length < sizeof(receivedValue) - 1 ? length : sizeof(receivedValue) - 1;
  
  // 将接收到的负载复制到receivedValue变量中
  strncpy(receivedValue, (char*)payload, size);
  
  // 在字符串末尾添加空字符，以确保它是一个正确的C字符串
  receivedValue[size] = '\0';

  // 打印原始消息
  Serial.println(receivedValue);
  
  // 提取数字部分
  if(sscanf(receivedValue, "Total screen usage duration: %d seconds", &durationSeconds) == 1){
    // 如果成功提取到数字，则将该数字打印在Serial Monitor中
    // 转换和打印的工作将在loop()中完成
  } else {
    // 如果提取失败，则重置durationSeconds为-1
    durationSeconds = -1;
  }
}

void reconnect() {
  // 循环直到重新连接
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // 尝试连接
    if (client.connect("arduinoClient")) {
      Serial.println("connected");
      // 一旦连接成功，重新订阅主题
      client.subscribe("student/CASA0016/ucfntl0/ScreenTimer");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // 5秒后重试
      delay(5000);
    }
  }
}

void setup() {
  Serial.begin(9600);
  Serial.println("Connecting to WiFi");
  WiFi.begin(ssid, pass);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.print(".");
  }
  Serial.println("WiFi connected");

  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);

  reconnect(); // 确保启动时连接到MQTT
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  // 检查durationSeconds是否有新值
  if (durationSeconds >= 0) {
    // 将秒数转换为小时、分钟和秒
    int hours = durationSeconds / 3600;
    int minutes = (durationSeconds % 3600) / 60;
    int seconds = durationSeconds % 60;

    // 输出转换后的时间
    Serial.print("Usage: ");
    Serial.print(hours);
    Serial.print(" hours, ");
    Serial.print(minutes);
    Serial.print(" minutes, ");
    Serial.print(seconds);
    Serial.println(" seconds.");

    // 重置durationSeconds以便接收下一个消息
    durationSeconds = -1;
  }
}
