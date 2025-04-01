#include "esp_camera.h"
#include "Arduino.h"
#include "FS.h"                // SD Card ESP32
#include "SD_MMC.h"            // SD Card ESP32
#include "soc/soc.h"           // Disable brownout problems
#include "soc/rtc_cntl_reg.h"  // Disable brownout problems
#include "driver/rtc_io.h"
#include <EEPROM.h>

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

#define SKIP_COUNT 15
#define STABILISATION_WAIT 6000 
#define WHITE_LED_GPIO 4
#define DONE_PIN 12
#define EEPROM_SIZE 4
#define DEFAULT_WAKE_SKIP 6

camera_fb_t * fb = NULL;
fs::FS &filesys = SD_MMC; 
String logstr = "";

// Acknolwedgement
//   This code is mainly copied from examples on the Internet, particularly
//       https://www.instructables.com/ESP32-CAM-Take-Photo-and-Save-to-MicroSD-Card-With/
//       https://randomnerdtutorials.com/esp32-cam-photo-microsd-card-timestamp/
//
// But with a few fixes from elsewhere
// 

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
    //   config.frame_size = FRAMESIZE_UXGA; 
    //   config.jpeg_quality = 10;
    //   config.fb_count = 2;
    // } else {
      config.frame_size = FRAMESIZE_SVGA;
      config.jpeg_quality = 12;
      config.fb_count = 1;
    // }
    
    esp_err_t err = esp_camera_init(&config);
    if (err != ESP_OK) {
      #ifdef SERIAL_LOGGING
      Serial.printf("Camera init failed with error 0x%x", err);
      #endif
      logstr = logstr + "|Camera Init err " + err;
      return;
    }

}

void stabliseCamera() {
  // skip to allow auto balance
  delay(STABILISATION_WAIT);
  for (int i=0;i<SKIP_COUNT;i++) {
    fb = esp_camera_fb_get();  
    if(fb) {
      esp_camera_fb_return(fb); 
    } 
  }
}

// SD functions
void initSD() {
  // Initialize the SD card to not use GPIO4
  SD_MMC.begin("/sdcard", true);

  // Turns off the ESP32-CAM white on-board LED (flash) connected to GPIO 4
  pinMode(WHITE_LED_GPIO, OUTPUT);
  digitalWrite(WHITE_LED_GPIO, LOW);
  //rtc_gpio_isolate(GPIO_NUM_4);

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
    log = logstr + "|SD mount fail";
    #endif
  }

}

//Read a file in SD card
int readSkipFromSDConfigFile(){
  String path = "/config.txt";
  int skip = DEFAULT_WAKE_SKIP;

  File file = filesys.open(path.c_str());

  if(file) {
    if (file.available()){
        String line = file.readString();
        skip = line.toInt();
    }
    file.close();
  }
  else {
      #ifdef SERIAL_LOGGING
      Serial.println("Failed to open file for reading");
      #endif
      logstr = logstr + "|config fail";
  }

  return skip;
}

void writeImage(camera_fb_t * image, int pictureNumber) {

  // Path where new picture will be saved in SD Card
  String path = "/img-" + String(pictureNumber) +".jpg";
  File file = filesys.open(path.c_str(), FILE_WRITE);
  if(file) {
    file.write(image->buf, image->len); // payload (image), payload length
  }

  file.close();
}

void disableBrownout() {
  WRITE_PERI_REG(RTC_CNTL_BROWN_OUT_REG, 0);
}

  #ifdef SERIAL_LOGGING
  Serial.begin(115200);
  #endif

  logstr = logstr + "|SD init";
  initSD(); // because this defines IO pins and is needed for wake skip

  pinMode(DONE_PIN, OUTPUT); // moving after SD init because it does NOT work in situ
  digitalWrite(DONE_PIN, LOW);

  logstr = logstr + "|read config";
  int wakeskip = readSkipFromFile();

  // read and increment pictureNumber

  int pictureNumber = 0;
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(0, p);
  p = p + 1;
  EEPROM.put(0, p);
  EEPROM.commit();
  return p;
}

  logstr = logstr + "|pic# " + pictureNumber;

  if (pictureNumber % wakeskip == 0) {
    initCamera();
    stabliseCamera();

    #ifdef SERIAL_LOGGING
    Serial.println("Click!");
    #endif
    logstr = logstr + "|click";

    fb = esp_camera_fb_get();  
  
    if(fb) {
      writeImage(fb, pictureNumber);
      esp_camera_fb_return(fb);
    } 
    #ifdef SERIAL_LOGGING
    else {
      Serial.println("Camera capture failed");
      logstr = logstr + "|picture fail";
    }
    #endif
  } else {
      logstr = logstr + "|skip";
  }
  #ifdef SERIAL_LOGGING
  else {
    Serial.println("Skipping this time");
  }
  Serial.println("Going to sleep now");
  Serial.flush(); 
  #endif

  // write log line to file
  String logpath = "/log.txt";

  File logfile = filesys.open(logpath.c_str(), FILE_WRITE);
  if(logfile){
    logfile.println(logstr); // payload (image), payload length
  }
  logfile.close();

}

// keep signalling done until we're turned off
void loop() {
    signalDone(true);
    delay(1);
    signalDone(false);
    delay(1);
}
