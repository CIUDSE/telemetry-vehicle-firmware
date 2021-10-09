#include <Arduino.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>
#include <ReactESP.h>
#include <axp20x.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// LoRa Chip (SX1278) SPI
#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND 915E6 // Can also use 868 Mhz bands 868E6


// OLED (SSD1306) I2C

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3c
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);


// GPS Serial1
TinyGPSPlus gps;
// HardwareSerial Serial1(1);
// Serial1 already defined as global in HardwareSerial.cpp

// Power Managment IC (AXP192)
// AXP20X_Class axp;

// MPU6050 Sensor
// Adafruit_MPU6050 mpu = Adafruit_MPU6050();

int global_heartbeat = 0;
int gps_serial_heartbeat = 0;
sensors_event_t accel_event, gyro_event, temp_event;

void all_println(const char *msg) {
  Serial.println(msg);
  //display.println(msg);
  //display.display();
}

void fetch_gps_data() {
  while (Serial1.available()) {
    gps.encode(Serial1.read());
    gps_serial_heartbeat = (gps_serial_heartbeat + 1) % 10000;
  }
}

ReactESP app([] () {
  Serial.begin(115200);

  /*
  if (!axp.begin(Wire, AXP192_SLAVE_ADDRESS)) {
    Serial.println("AXP192 Begin PASS");
  } else {
    Serial.println("AXP192 Begin FAIL");
  }
  axp.setPowerOutPut(AXP192_LDO2, AXP202_ON);
  axp.setPowerOutPut(AXP192_LDO3, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC2, AXP202_ON);
  axp.setPowerOutPut(AXP192_EXTEN, AXP202_ON);
  axp.setPowerOutPut(AXP192_DCDC1, AXP202_ON);
  */

  /*
  Serial.println("OLED...");
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println("SSD1306 alloc err");
    for(;;); // Loop forever
  }
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0,0);
  all_println("OLED OK");
  */

  all_println("LoRa...");
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if(!LoRa.begin(BAND)) {
    Serial.println("LoRa init err");
    for(;;); // Loop forever
  }
  all_println("LoRa OK");

  all_println("GPS...");
  Serial1.begin(9600, SERIAL_8N1, 34, 12);
  app.onTick(fetch_gps_data);
  all_println("GPS OK");

  /*
  all_println("MPU...");
  mpu.begin();
  all_println("MPU OK");
  */

  all_println("OK");
  
  /*
  app.onRepeat(250, [] () {
    mpu.getEvent(&accel_event, &gyro_event, &temp_event);
  });
  app.onRepeat(2000, [] () {
    LoRa.beginPacket();
    LoRa.printf("TMP %f\n", temp_event.temperature);
    LoRa.endPacket();
  });
  app.onRepeat(500, [] () {
    sensors_vec_t gyro = gyro_event.gyro;
    LoRa.beginPacket();
    LoRa.printf("HDG %f\n", gyro.heading);
    LoRa.printf("PCH %f\n", gyro.pitch);
    LoRa.printf("RLL %f\n", gyro.roll);
    LoRa.endPacket();
  });
  app.onRepeat(500, [] () {
    sensors_vec_t accel = accel_event.acceleration;
    LoRa.beginPacket();
    LoRa.printf("ACX %f\n", accel.x);
    LoRa.printf("ACY %f\n", accel.y);
    LoRa.printf("ACZ %f\n", accel.z);
    LoRa.endPacket();
  });
  */
  app.onRepeat(2000, [] () {
    LoRa.beginPacket();
    LoRa.println("<3");
    LoRa.endPacket();
  });
  app.onRepeat(1000, [] () {
    if(gps.location.isValid()){
      LoRa.beginPacket();
      LoRa.println("GPS");
      LoRa.printf("LNG %f\n", gps.location.lng());
      LoRa.printf("LAT %f\n", gps.location.lat());
      LoRa.endPacket();
    }
  });
  app.onRepeat(1000, [] () {
    //display.clearDisplay();
    //display.setCursor(0, 0);

    Serial.printf("<3 GLB %i GPS %i\n", global_heartbeat, gps_serial_heartbeat);    
    //display.printf("<3 GLB %i GPS %i\n", global_heartbeat, gps_serial_heartbeat);
    Serial.printf("#SAT %i\n", gps.satellites.value());
    //display.printf("#SAT %i\n", gps.satellites.value());
    //display.printf("LNG %9.6f\n", gps.location.lng());
    Serial.printf("LNG %9.6f\n", gps.location.lng());
    //display.printf("LAT %9.6f\n", gps.location.lat());
    Serial.printf("LAT %9.6f\n", gps.location.lat());
    

    //display.display();
    global_heartbeat = (global_heartbeat + 1) % 1000;
  });
});