#include <esp_camera.h>
#include <WiFi.h>
#include <WebServer.h>
#include <ESPAsyncWebServer.h>
#include <AsyncTCP.h>
#include <HTTPClient.h>
#include <FS.h>
#include <SD_MMC.h>
#include <ArduinoJson.h>
#include <TimeLib.h>
#include "soc/soc.h"
#include "soc/rtc_cntl_reg.h"

#define CAMERA_MODEL_AI_THINKER
#include "camera_pins.h"

#define BUTTON_PIN 14
#define FLASH_LED_PIN 4
int flash_on = 0;

// Thông tin mạng
const char* ssid_home = "TP-Link_CDB8";
const char* password_home = "05061754";

// const char* ssid_home = "Từ 1 đến 8 :)))";
// const char* password_home = "tumotden8:)))";

// Thông tin web api
const char* serverIP = "192.168.0.232";
const int serverPort = 8011;

// NTP server
const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = 25200;
const int daylightOffset_sec = 0;

WebServer server(80);
// AsyncWebServer asyncServer(80);
IPAddress esp32ServerIP;
IPAddress IPv4(192, 168, 0, 113);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress DNS1(192, 168, 0, 1);  
IPAddress DNS2(0, 0, 0, 0);

// setup function_________________________________________________
void setup() {
  // WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);

  Serial.begin(115200);
  Serial.setDebugOutput(true);
  Serial.println();

  if (!WiFi.config(IPv4, gateway, subnet, DNS1, DNS2)) {
    Serial.println("Static IP Failed to configure");
  }

  pinMode(BUTTON_PIN, INPUT);
  pinMode(FLASH_LED_PIN, OUTPUT);
  // digitalWrite(FLASH_LED_PIN, LOW);

  // Cấu hình camera
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // -UXGA   = 1600 x 1200 pixels
  // -SXGA   = 1280 x 1024 pixels
  // -XGA    = 1024 x 768  pixels
  // -SVGA   = 800 x 600   pixels
  // -VGA    = 640 x 480   pixels
  // -CIF    = 352 x 288   pixels
  // -QVGA   = 320 x 240   pixels
  // -HQVGA  = 240 x 160   pixels
  // -QQVGA  = 160 x 120   pixels
  if (psramFound()) {
    config.frame_size = FRAMESIZE_UXGA;
    config.jpeg_quality = 10;
    config.fb_count = 2;
  } else {
    config.frame_size = FRAMESIZE_SVGA;
    config.jpeg_quality = 12;
    config.fb_count = 1;
  }

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Error: Camera init failed with error 0x%x", err);
    return;
  }

  if (!SD_MMC.begin()) {
    Serial.println("Error: SD Card Mount Failed");
    return;
  }
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE) {
    Serial.println("Error: No SD card attached");
    return;
  }
  Serial.println("SD card attched");

  // Kết nối đến mạng WiFi
  WiFi.begin(ssid_home, password_home);
  // WiFi.begin(ssid_phone, password_phone);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi.");

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  while (!time(nullptr)) {
    delay(500);
    Serial.println("Waiting for time sync...");
  }
  Serial.println("Time synced!");
  // printLocalTime();

  // Địa chỉ ip của esp32-cam server
  esp32ServerIP = WiFi.localIP();
  Serial.print("Local IP: ");
  Serial.println(WiFi.localIP());
  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());
  Serial.print("Subnet Mask: " );
  Serial.println(WiFi.subnetMask());
  Serial.print("DNS 1: ");
  Serial.println(WiFi.dnsIP(0));
  Serial.print("DNS 2: ");
  Serial.println(WiFi.dnsIP(1));

  Serial.print("Capture: http://");
  Serial.print(esp32ServerIP);
  Serial.println("/NewImage");

  server.on("/", HTTP_GET, handleServerActive);
  server.on("/NewImage", HTTP_GET, handleCapture);
  server.on("/Images", HTTP_GET, handleImage);
  server.on("/Videos", HTTP_GET, handleVideo);
  server.on("/Doors", HTTP_GET, handleDoor);
  server.onNotFound(handleNotFound);
  server.begin();
  // asyncServer.on("/NewImage", HTTP_GET, AsyncHandleCapture);
  // asyncServer.on("/Images", HTTP_GET, AsyncHandleImage);
  // asyncServer.on("/Videos", HTTP_GET, AsyncHandleVideo);
  // asyncServer.begin();
}
//________________________________________________________________

// loop function__________________________________________________
void loop() {
  server.handleClient();
  if (digitalRead(BUTTON_PIN) == 1) {
    handleCapture();
    // AsyncWebServerRequest *request = nullptr;
    // AsyncHandleCapture(request);
  }
}
//_______________________________________________________________ _

// In ra ngày giờ_________________________________________________
void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  int year, month, day, hour, minute, second;
  getDateTimeFromTimeInfo(&timeinfo, &year, &month, &day, &hour, &minute, &second);
  Serial.print("Year: ");
  Serial.println(year);
  Serial.print("Month: ");
  Serial.println(month);
  Serial.print("Day: ");
  Serial.println(day);
  Serial.println(hour);
  Serial.println(minute);
  Serial.println(second);
}
//________________________________________________________________

// Lấy ra ngày giờ________________________________________________
void getDateTimeFromTimeInfo(struct tm* timeinfo, int* year, int* month, int* day, int* hour, int* minute, int* second) {
  *year = timeinfo->tm_year + 1900;
  *month = timeinfo->tm_mon + 1;
  *day = timeinfo->tm_mday;
  *hour = timeinfo->tm_hour;
  *minute = timeinfo->tm_min;
  *second = timeinfo->tm_sec;
}
//________________________________________________________________

// Hàm chụp ảnh___________________________________________________
camera_fb_t* takePhoto(int* year, int* month, int* day, int* hour, int* minute, int* second) {
  camera_fb_t* fb = nullptr;
  for (int i = 0; i < 3; i++) {
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Error: Camera capture failed");
    }
    esp_camera_fb_return(fb);
    delay(100);
  }
  fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Error: Camera capture failed");
    return nullptr;
  }

  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
  }
  int nyear, nmonth, nday, nhour, nminute, nsecond;
  getDateTimeFromTimeInfo(&timeinfo, &nyear, &nmonth, &nday, &nhour, &nminute, &nsecond);
  *year = nyear;
  *month = nmonth;
  *day = nday;
  *hour = nhour;
  *minute = nminute;
  *second = nsecond;

  Serial.println("Camera captured");
  return fb;
}
//________________________________________________________________

// Hàm lưu ảnh vào thẻ nhớ________________________________________
void saveToSDCard(camera_fb_t* fb, String folderName, String fileName) {
  String folderPath = "/" + folderName;
  if (!SD_MMC.exists(folderPath)) {
    if (SD_MMC.mkdir(folderPath)) {
      Serial.println("Folder created successfully");
    } else {
      Serial.println("Failed to create folder");
      // Xử lý lỗi khi không tạo được thư mục
    }
  } else {
    Serial.println("Folder already exists");
  }

  String filePath = folderPath + "/" + fileName + ".jpg";
  File file = SD_MMC.open(filePath, "w");
  if (!file) {
    Serial.println("Error: Failed to open file for writing");
    return;
  }
  file.write(fb->buf, fb->len);
  Serial.println("Image was saved in SD card");
  file.close();
  return;
}
//________________________________________________________________

// Hàm gửi ảnh đến back-end_______________________________________
void sendImageToBE(String dataJson) {
  WiFiClient clientSend;
  if (clientSend.connect(serverIP, serverPort)) {
    Serial.println("Connected to server");

    String requestHeader = "POST /ESP32Cams HTTP/1.1\r\n";
    requestHeader += "Host: " + String(serverIP) + "\r\n";
    requestHeader += "Content-Type: application/json\r\n";
    requestHeader += "Content-Length: " + String(dataJson.length()) + "\r\n\r\n";

    clientSend.print(requestHeader);
    clientSend.print(dataJson);

    clientSend.stop();
  } else {
    Serial.println("Connection to server failed");
  }
}
//________________________________________________________________

//Chụp ảnh mới khi có yêu cầu đến server__________________________
const char headerCapture[] = "HTTP/1.1 200 OK\r\n"
                             "Access-Control-Allow-Origin: *\r\n"
                             "Content-type: application/json\r\n\r\n";
void handleCapture(void) {
  int year, month, day, hour, minute, second;
  camera_fb_t* fb = takePhoto(&year, &month, &day, &hour, &minute, &second);

  String smonth = month < 10 ? "0" + String(month) : String(month);
  String sday = day < 10 ? "0" + String(day) : String(day);
  String shour = hour < 10 ? "0" + String(hour) : String(hour);
  String smin = minute < 10 ? "0" + String(minute) : String(minute);
  String ssec = second < 10 ? "0" + String(second) : String(second);

  String folderName = String(sday) + "." + String(smonth) + "." + String(year);
  String fileName = folderName + "-" + shour + smin + ssec;
  saveToSDCard(fb, folderName, fileName);
  
  String timeString = String(year) + "-" + String(smonth) + "-" + String(sday) + "T" + String(shour) + ":" + String(smin) + ":" + String(ssec);
  String IPString = "";
  for (int i = 0; i < 4; ++i) {
    IPString += String(esp32ServerIP[i]);
    if (i < 3) {
      IPString += ".";
    }
  }

  DynamicJsonDocument doc(1024);
  String jsonStr;
  JsonObject root = doc.to<JsonObject>();
  root["ImageId"] = "3fa85f64-5717-4562-b3fc-2c963f66afa6";
  root["ImageName"] = fileName + ".jpg";
  root["ImageTime"] = timeString;
  root["ImageUrl"] = "http://" + IPString + "/Images?name=" + fileName;
  serializeJson(doc, jsonStr);

  WiFiClient clientConnect = server.client();
  if (clientConnect.connected()) {
    clientConnect.write(headerCapture, strlen(headerCapture));
    clientConnect.print(jsonStr);
  }

  sendImageToBE(jsonStr);
  esp_camera_fb_return(fb);
}
//________________________________________________________________

// Gửi ảnh từ esp server tới client_______________________________
const char headerImage[] = "HTTP/1.1 200 OK\r\n"
                           "Access-Control-Allow-Origin: *\r\n"
                           "Content-disposition: inline\r\n"
                           "Content-type: image/jpeg\r\n\r\n";
void handleImage(void) {
  WiFiClient clientConnect = server.client();

  int count = server.args();
  if (count == 1) {
    String fileName = server.arg("name");
    int indexOfDash = fileName.indexOf('-');
    if (indexOfDash != -1) {
      String folderName = fileName.substring(0, indexOfDash);
      String filePath = "/" + folderName + "/" + fileName + ".jpg";

      File file = SD_MMC.open(filePath);
      if (file) {
        size_t fileSize = file.size();
        uint8_t* imageData = (uint8_t*)malloc(fileSize);
        if (!imageData) {
          Serial.println("Failed to allocate memory");
          server.send(500, "text/plain", "Failed to allocate memory");
          file.close();
          ESP.restart();
          return;
        }
        file.read(imageData, fileSize);
        file.close();

        if (clientConnect.connected()) {
          clientConnect.write(headerImage, strlen(headerImage));
          clientConnect.write(imageData, fileSize);
        } else {
          return;
        }
      }
    } else {
      server.send(404, "text/plain", "Image not found");
    }
  } else {
    server.send(400, "text/plain", "Parameter error");
  }
}
//________________________________________________________________

// Live video_____________________________________________________
const char headerVideo[] = "HTTP/1.1 200 OK\r\n"
                           "Access-Control-Allow-Origin: *\r\n"
                           "Content-Type: multipart/x-mixed-replace; boundary=123456789000000000000987654321\r\n";
const char boundary[] = "\r\n--123456789000000000000987654321\r\n";
const char contentType[] = "Content-Type: image/jpeg\r\n";
const int hdrLen = strlen(headerVideo);
const int bdrLen = strlen(boundary);
const int cntLen = strlen(contentType);
void handleVideo(void) {
  WiFiClient clientConnect = server.client();

  clientConnect.write(headerVideo, hdrLen);
  clientConnect.write(boundary, bdrLen);

  camera_fb_t* fb;
  while (clientConnect.connected()) {
    fb = esp_camera_fb_get();
    if (!fb) {
      ESP.restart();
    }
    clientConnect.write(contentType, cntLen);
    clientConnect.printf("Content-Length: %d\r\n\r\n", fb->len);
    clientConnect.write(fb->buf, fb->len);
    clientConnect.write(boundary, bdrLen);
    esp_camera_fb_return(fb);
    delay(10);
  }
  clientConnect.stop();
}
//________________________________________________________________

// Hàm xử lí đóng mở cửa__________________________________________
const char headerDoor[] = "HTTP/1.1 200 OK\r\n"
                          "Access-Control-Allow-Origin: *\r\n"
                          "Content-type: text/plain\r\n\r\n";
void handleDoor(void) {
  WiFiClient clientConnect = server.client();

  int count = server.args();
  if (count == 1) {
    int status = server.arg("status").toInt();

    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) {
      Serial.println("Failed to obtain time");
    }
    int year, month, day, hour, minute, second;
    getDateTimeFromTimeInfo(&timeinfo, &year, &month, &day, &hour, &minute, &second);

    String smonth = month < 10 ? "0" + String(month) : String(month);
    String sday = day < 10 ? "0" + String(day) : String(day);
    String shour = hour < 10 ? "0" + String(hour) : String(hour);
    String smin = minute < 10 ? "0" + String(minute) : String(minute);
    String ssec = second < 10 ? "0" + String(second) : String(second);
    String timeString = String(year) + "-" + String(smonth) + "-" + String(sday) + "T" + String(shour) + ":" + String(smin) + ":" + String(ssec);

    if (status == 1) {
      flash_on = 1;
      digitalWrite(FLASH_LED_PIN, HIGH);
      if (clientConnect.connected()) {
        clientConnect.write(headerDoor, strlen(headerDoor));
        clientConnect.print(flash_on);
      }
      // delay(1000);
      // flash_on = 0;
      // digitalWrite(FLASH_LED_PIN, LOW);
    } else {
      flash_on = 0;
      digitalWrite(FLASH_LED_PIN, LOW);
      if (clientConnect.connected()) {
        clientConnect.write(headerDoor, strlen(headerDoor));
        clientConnect.print(flash_on);
      }
    }

    DynamicJsonDocument doc(1024);
    String jsonStr;
    JsonObject root = doc.to<JsonObject>();
    root["DoorStatusId"] = "3fa85f64-5717-4562-b3fc-2c963f66afa6";
    root["DoorStatusTime"] = timeString;
    root["DoorStatusValue"] = flash_on;
    serializeJson(doc, jsonStr);
    sendDoorStatusToBE(jsonStr);
  } else {
    server.send(400, "text/plain", "Invalid parameters");
  }
}
//________________________________________________________________

// Hàm gửi trạng thái cửa đến back-end_______________________________________
void sendDoorStatusToBE(String dataJson) {
  WiFiClient clientSend;
  if (clientSend.connect(serverIP, serverPort)) {
    Serial.println("Connected to server");

    String requestHeader = "POST /ESP32Cams/DoorStatus HTTP/1.1\r\n";
    requestHeader += "Host: " + String(serverIP) + "\r\n";
    requestHeader += "Content-Type: application/json\r\n";
    requestHeader += "Content-Length: " + String(dataJson.length()) + "\r\n\r\n";

    clientSend.print(requestHeader);
    clientSend.print(dataJson);

    clientSend.stop();
  } else {
    Serial.println("Connection to server failed");
  }
}
//________________________________________________________________

// Server active__________________________________________________
const char headerActive[] = "HTTP/1.1 200 OK\r\n"
                            "Access-Control-Allow-Origin: *\r\n"
                            "Content-type: text/plain\r\n\r\n";
void handleServerActive(void) {
  WiFiClient clientConnect = server.client();

  DynamicJsonDocument doc(1024);
  String jsonStr;
  JsonObject root = doc.to<JsonObject>();
  root["DeviceStatus"] = 1;
  root["DoorStatus"] = flash_on;
  serializeJson(doc, jsonStr);

  clientConnect.write(headerActive, strlen(headerActive));
  clientConnect.print(jsonStr);
}

// Xử lí đường dẫn không hợp lệ___________________________________
const char headerNotFound[] = "HTTP/1.1 404 Not Found\r\n"
                              "Access-Control-Allow-Origin: *\r\n"
                              "Content-type: text/plain\r\n\r\n";
void handleNotFound() {
  WiFiClient clientConnect = server.client();
  clientConnect.write(headerNotFound, strlen(headerNotFound));
}
//________________________________________________________________

//-----
// Hàm xử lí chụp ảnh bất đồng bộ_________________________________
// void AsyncHandleCapture(AsyncWebServerRequest* request) {
//   int year, month, day, hour, minute, second;
//   camera_fb_t* fb = takePhoto(&year, &month, &day, &hour, &minute, &second);

//   String folderName = String(day) + "." + String(month) + "." + String(year);
//   String shour = hour < 10 ? "0" + String(hour) : String(hour);
//   String smin = minute < 10 ? "0" + String(minute) : String(minute);
//   String ssec = second < 10 ? "0" + String(second) : String(second);
//   String fileName = folderName + "-" + shour + smin + ssec;
//   saveToSDCard(fb, folderName, fileName);

//   String smonth = month < 10 ? "0" + String(month) : String(month);
//   String sday = day < 10 ? "0" + String(day) : String(day);
//   String timeString = String(year) + "-" + String(smonth) + "-" + String(sday) + "T" + String(shour) + ":" + String(smin) + ":" + String(ssec) + "Z";
//   String IPString = "";
//   for (int i = 0; i < 4; ++i) {
//     IPString += String(esp32ServerIP[i]);
//     if (i < 3) {
//       IPString += ".";
//     }
//   }

//   DynamicJsonDocument doc(1024);
//   String jsonStr;
//   JsonObject root = doc.to<JsonObject>();
//   root["ImageId"] = "3fa85f64-5717-4562-b3fc-2c963f66afa6";
//   root["ImageName"] = fileName;
//   root["ImageTime"] = timeString;
//   root["ImageUrl"] = "http://" + IPString + "/Images?name=" + fileName;
//   serializeJson(doc, jsonStr);

//   request->send(200, "application/json", jsonStr);

//   sendImageToBE(jsonStr);
//   esp_camera_fb_return(fb);
// }
//________________________________________________________________

// Hàm xử lí gửi ảnh bất đồng bộ__________________________________
// void AsyncHandleImage(AsyncWebServerRequest* request) { 
//   int countArgs = request->args();
//   if (countArgs == 1) {
//     String fileName = request->arg("name");
//     int indexOfDash = fileName.indexOf('-');
//     if (indexOfDash != -1) {
//       String folderName = fileName.substring(0, indexOfDash);
//       String filePath = "/" + folderName + "/" + fileName + ".jpg";

//       if (SD_MMC.exists(filePath)) {
//         request->send(SD_MMC, filePath, "image/jpeg", false);
//       } else {
//         request->send(404, "text/plain", "Image not found");
//       }
//     } else {
//       request->send(404, "text/plain", "Image not found");
//     }
//   } else {
//     request->send(404, "text/plain", "Invalid parameters");
//   }
// }
//________________________________________________________________

// Hàm xử lí video trực tiếp bất đồng bộ__________________________
// void AsyncHandleVideo(AsyncWebServerRequest *request) {
// }
//________________________________________________________________
// -----