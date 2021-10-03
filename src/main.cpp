#include <Arduino.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include <TinyGPS++.h>

// LoRa Chip (SX1278) SPI
#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISnO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)
#define BAND 868E6

// OLED (SSD1306) I2C
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET 4
#define SCREEN_ADDRESS 0x3c
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

// GPS Serial1
TinyGPSPlus gps;
HardwareSerial Serial1(1);

void all_println(const char *msg) {
  Serial.println(msg);
  display.println(msg);
  display.display();
}

void setup() {
  Serial.begin(115200);

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

  all_println("LoRa...");
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if(!LoRa.begin(BAND)) {
    Serial.println("LoRa init err");
    for(;;); // Loop forever
  }
  all_println("LoRa OK");

  all_println("GPS interface...");
  Serial1.begin(9600, SERIAL_8N1, 12, 15);
  // T-Beam NEO GPS TX pin is 34 and RX pin is 12.
  // These magical values were in official repo though
  all_println("GPS interface OK");

  all_println("OK");
}

void loop() {
}

void fetch_gps_data() {
  while (Serial1.available()) {
    gps.encode(Serial.read());
  }
}