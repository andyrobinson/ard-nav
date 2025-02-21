#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"

// Pin definition for CAMERA_MODEL_AI_THINKER
#define PWDN_GPIO_NUM     32
#define RESET_GPIO_NUM    -1
#define XCLK_GPIO_NUM      0
#define SIOD_GPIO_NUM     26
#define SIOC_GPIO_NUM     27

#define Y9_GPIO_NUM       35
#define Y8_GPIO_NUM       34
#define Y7_GPIO_NUM       39
#define Y6_GPIO_NUM       36
#define Y5_GPIO_NUM       21
#define Y4_GPIO_NUM       19
#define Y3_GPIO_NUM       18
#define Y2_GPIO_NUM        5
#define VSYNC_GPIO_NUM    25
#define HREF_GPIO_NUM     23
#define PCLK_GPIO_NUM     22

#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  30          /* Time between shorts ESP32 will go to sleep (in seconds) */
#define SKIP_COUNT 30
#define STABLISATION_WAIT 6000 /* for camera stablisation */
#define WHITE_LED_GPIO 4

camera_fb_t * fb = NULL;
fs::FS &filesys = SD_MMC; 

// TODO:
// Next - 
//   Use a GPIO pin to determine sleep frequency (1 hour or 12 hours)
// 
// turn off bluetooth and wifi
// Test for 48 hours
// Independent power supply and test for 48 hours, with switch, both positions
// Installation

// TIME functions
void setTimezone(String timezone){
  Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

void initTime(String timezone){
  struct tm timeinfo;
  Serial.println("Setting up time");
  setTime(2025,02,06,0,0,0,0); 
  setTimezone(timezone);
}

void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time 1");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
}

void setTime(int yr, int month, int mday, int hr, int minute, int sec, int isDst){
  struct tm tm;

  tm.tm_year = yr - 1900;   // Set date
  tm.tm_mon = month-1;
  tm.tm_mday = mday;
  tm.tm_hour = hr;      // Set time
  tm.tm_min = minute;
  tm.tm_sec = sec;
  tm.tm_isdst = isDst;  // 1 or 0
  time_t t = mktime(&tm);
  Serial.printf("Setting time: %s", asctime(&tm));
  struct timeval now = { .tv_sec = t };
  settimeofday(&now, NULL);
}

// CAMERA functions
void initCamera() {

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
    
    // assume we have PSRAM, because we do
    config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    config.jpeg_quality = 10;
    config.fb_count = 2;
    
    // Init Camera
    Serial.println("Camera init");
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
      Serial.printf("Camera init failed with error 0x%x", err);
      return;
    }

}

void stabliseCamera() {
  // skip to allow auto balance
  Serial.println("Wait, then Camera read and skip to allow autoadjustment");
  delay(STABLISATION_WAIT);
  for (int i=0;i<SKIP_COUNT;i++) {
    fb = esp_camera_fb_get();  
    if(fb) {
      esp_camera_fb_return(fb); 
    } else {
      Serial.println("Camera capture failed during skip");
    }
  }

}

// SD functions
void initSD() {
  // Initialize the SD card to not use GPIO4
  Serial.println("SD card init");
  if (!SD_MMC.begin("/sdcard", true)){
      Serial.println("Failed to mount SD card");
  }

  // Turns off the ESP32-CAM white on-board LED (flash) connected to GPIO 4
  pinMode(WHITE_LED_GPIO, OUTPUT);
  digitalWrite(WHITE_LED_GPIO, LOW);

  // Check for an SD card
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE){
      Serial.println("No SD card attached");
  }

  Serial.println("Starting SD Card");
  if(!SD_MMC.begin()){
    Serial.println("SD Card Mount Failed");
  }

}

void writeImage(camera_fb_t * image) {

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time");
    return;
  }

  char timeString[20];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d_%H-%M-%S", &timeinfo);
  String path = "/img-" + String(timeString) +".jpg";
  Serial.printf("Picture file name: %s\n", path.c_str());
  
  File file = filesys.open(path.c_str(), FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file in writing mode");
  } 
  else {
    file.write(image->buf, image->len); // payload (image), payload length
  }
  file.close();

}

// SLEEP functions
void print_wakeup_reason(){
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 : Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 : Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER : Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP : Serial.println("Wakeup caused by ULP program"); break;
    default : Serial.printf("Wakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}


void setup() {
  unsigned long start = millis();
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  Serial.begin(115200);

  print_wakeup_reason();


  // don't init time if it's a timer wake up
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  if (wakeup_reason != ESP_SLEEP_WAKEUP_TIMER) initTime("GMT0");

  printLocalTime();
  initCamera();
  initSD();

  stabliseCamera();

  Serial.println("Click!");
  fb = esp_camera_fb_get();  

  if(fb) {
    writeImage(fb);
    esp_camera_fb_return(fb);
  } 
  else {
    Serial.println("Camera capture failed");
  }

  // Turns off the ESP32-CAM white on-board LED (flash) connected to GPIO 4
  pinMode(WHITE_LED_GPIO, OUTPUT);
  digitalWrite(WHITE_LED_GPIO, LOW);

  Serial.println("Going to sleep now");
  Serial.flush(); 

  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR - ((millis() - start) * 1000));
  esp_deep_sleep_start();
}

void loop() {}
