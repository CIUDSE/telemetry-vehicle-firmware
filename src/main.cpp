#include <Arduino.h>
#include <LoRa.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

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
  display.display();
  display.println("OLED OK");
  Serial.println("OLED OK");

  Serial.println("LoRa...");
  display.println("LoRa...");
  display.display();
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if(!LoRa.begin(BAND)) {
    Serial.println("LoRa init err");
    for(;;); // Loop forever
  }
  Serial.println("LoRa OK");
  display.println("LoRa OK");
  display.display();
  
  Serial.println("OK");
  display.println("OK");
  display.display();
}

void loop() {
  // put your main code here, to run repeatedly:
}