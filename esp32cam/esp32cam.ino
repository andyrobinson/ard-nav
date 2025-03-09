#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
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
#define TIME_TO_SLEEP_SHORT  3600       /* 1 hours */
#define TIME_TO_SLEEP_LONG   7200      /* 2 hours */
#define BOOT_TIME_US 500000ULL
#define SKIP_COUNT 15
#define STABILISATION_WAIT 6000 /* for camera stablisation */
#define WHITE_LED_GPIO 4
#define SWITCH_GPIO 13
//#define SERIAL_LOGGING 1

camera_fb_t * fb = NULL;
fs::FS &filesys = SD_MMC; 

// Acknolwedgement
//   This code is mainly copied from examples on the Internet, particularly
//       https://www.instructables.com/ESP32-CAM-Take-Photo-and-Save-to-MicroSD-Card-With/
//       https://randomnerdtutorials.com/esp32-cam-photo-microsd-card-timestamp/
//
// But with a few fixes from elsewhere
// 
// TODO:
// Observations:
// - used GPIO 13 in the end because it does not have an effect on boot

// TIME functions
void setTimezone(String timezone){
  #ifdef SERIAL_LOGGING
  Serial.printf("  Setting Timezone to %s\n",timezone.c_str());
  #endif
  setenv("TZ",timezone.c_str(),1);  //  Now adjust the TZ.  Clock settings are adjusted to show the new local time
  tzset();
}

void initTime(String timezone){
  struct tm timeinfo;
  #ifdef SERIAL_LOGGING
  Serial.println("Setting up time");
  #endif
  setTime(2025,3,8,0,0,0,0); 
  setTimezone(timezone);
}

#ifdef SERIAL_LOGGING
void printLocalTime(){
  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    Serial.println("Failed to obtain time 1");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S zone %Z %z ");
}
#endif

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
  #ifdef SERIAL_LOGGING
  Serial.printf("Setting time: %s", asctime(&tm));
  #endif
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
 
    // if(psramFound()){
    //   Serial.println("*** Psram found");
    //   config.frame_size = FRAMESIZE_UXGA; // FRAMESIZE_ + QVGA|CIF|VGA|SVGA|XGA|SXGA|UXGA
    //   config.jpeg_quality = 10;
    //   config.fb_count = 2;
    // } else {
    #ifdef SERIAL_LOGGING
      Serial.println("*** Psram NOT found");
    #endif
      config.frame_size = FRAMESIZE_SVGA;
      config.jpeg_quality = 12;
      config.fb_count = 1;
    // }
    
    // Init Camera
    #ifdef SERIAL_LOGGING
    Serial.println("Camera init");
    #endif

    // turn on camera power
    rtc_gpio_hold_dis(gpio_num_t(PWDN_GPIO_NUM));
    pinMode(PWDN_GPIO_NUM, OUTPUT);
    digitalWrite(PWDN_GPIO_NUM, LOW);
    delay(10);

    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
      #ifdef SERIAL_LOGGING
      Serial.printf("Camera init failed with error 0x%x", err);
      #endif
      return;
    }

}

void stabliseCamera() {
  // skip to allow auto balance
  #ifdef SERIAL_LOGGING
  Serial.println("Wait, then Camera read and skip to allow autoadjustment");
  #endif

  delay(STABILISATION_WAIT);
  for (int i=0;i<SKIP_COUNT;i++) {
    fb = esp_camera_fb_get();  
    if(fb) {
      esp_camera_fb_return(fb); 
    } else {
      #ifdef SERIAL_LOGGING
      Serial.println("Camera capture failed during skip");
      #endif
    }
  }

}

// SD functions
void initSD(bool flashLed) {
  // Initialize the SD card to not use GPIO4

  #ifdef SERIAL_LOGGING
  Serial.println("SD card init");
  #endif

  if (!SD_MMC.begin("/sdcard", true)){
      #ifdef SERIAL_LOGGING
      Serial.println("Failed to mount SD card");
      #endif
  }

  // Turns off the ESP32-CAM white on-board LED (flash) connected to GPIO 4
  // isolate keeps it off in deep sleep
  pinMode(WHITE_LED_GPIO, OUTPUT);
  if (flashLed) {
    digitalWrite(WHITE_LED_GPIO, HIGH);
    delay(1000);
  }
  digitalWrite(WHITE_LED_GPIO, LOW);
  rtc_gpio_isolate(GPIO_NUM_4);

  // Check for an SD card
  uint8_t cardType = SD_MMC.cardType();
  if (cardType == CARD_NONE){
      #ifdef SERIAL_LOGGING
      Serial.println("No SD card attached");
      #endif
  }

  #ifdef SERIAL_LOGGING
  Serial.println("Starting SD Card");
  #endif

  if(!SD_MMC.begin()){
    #ifdef SERIAL_LOGGING
    Serial.println("SD Card Mount Failed");
    #endif
  }

}

void writeImage(camera_fb_t * image,int suffix) {

  struct tm timeinfo;
  if(!getLocalTime(&timeinfo)){
    #ifdef SERIAL_LOGGING
    Serial.println("Failed to obtain time");
    #endif
    return;
  }

  char timeString[20];
  strftime(timeString, sizeof(timeString), "%Y-%m-%d_%H-%M-%S", &timeinfo);
  String path = "/img-" + String(suffix) + "-" + String(timeString) +".jpg";

  #ifdef SERIAL_LOGGING
  Serial.printf("Picture file name: %s\n", path.c_str());
  #endif

  File file = filesys.open(path.c_str(), FILE_WRITE);
  if(!file){
    #ifdef SERIAL_LOGGING
    Serial.println("Failed to open file in writing mode");
    #endif
  } 
  else {
    file.write(image->buf, image->len); // payload (image), payload length
  }
  file.close();

}

// SLEEP functions
#ifdef SERIAL_LOGGING
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
#endif

void hibernate() {
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH,   ESP_PD_OPTION_ON);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_PERIPH,   ESP_PD_OPTION_OFF);

    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_ON);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_SLOW_MEM, ESP_PD_OPTION_OFF);

    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_ON);
    esp_sleep_pd_config(ESP_PD_DOMAIN_RTC_FAST_MEM, ESP_PD_OPTION_OFF);

    esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL,         ESP_PD_OPTION_ON);
    esp_sleep_pd_config(ESP_PD_DOMAIN_XTAL,         ESP_PD_OPTION_OFF);

    // turn off camera power
    esp_camera_deinit();
    digitalWrite(PWDN_GPIO_NUM, HIGH);

    esp_deep_sleep_start();
}

void setup() {
  unsigned long start = millis();
  
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0); //disable brownout detector
  
  #ifdef SERIAL_LOGGING
  Serial.begin(115200);
  print_wakeup_reason();
  #endif

  // don't init time if it's a timer wake up
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  if (wakeup_reason != ESP_SLEEP_WAKEUP_TIMER) initTime("GMT0");

  #ifdef SERIAL_LOGGING
  printLocalTime();
  #endif

  initCamera();
  initSD(wakeup_reason != ESP_SLEEP_WAKEUP_TIMER);

  pinMode(SWITCH_GPIO, INPUT_PULLDOWN);
  int switchvalue = digitalRead(SWITCH_GPIO);
  #ifdef SERIAL_LOGGING
  Serial.printf("Input value %d\n", switchvalue);
  #endif

  stabliseCamera();

  #ifdef SERIAL_LOGGING
  Serial.println("Click!");
  #endif

  fb = esp_camera_fb_get();  

  if(fb) {
    writeImage(fb,switchvalue);
    esp_camera_fb_return(fb);
  } 
  #ifdef SERIAL_LOGGING
  else {
    Serial.println("Camera capture failed");
  }

  Serial.println("Going to sleep now");
  Serial.flush(); 
  #endif
  
  unsigned long timeToSleep;
  if(switchvalue == 0) timeToSleep = TIME_TO_SLEEP_LONG; else timeToSleep = TIME_TO_SLEEP_SHORT;

  esp_sleep_enable_timer_wakeup((timeToSleep * uS_TO_S_FACTOR - ((millis() - start) * 1000)) - BOOT_TIME_US);
  hibernate();
}

void loop() {}
