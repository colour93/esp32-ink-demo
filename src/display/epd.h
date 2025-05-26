#ifndef EPD_H
#define EPD_H

#define USE_HSPI_FOR_EPD
#define ENABLE_GxEPD2_GFX 0

#include <GxEPD2_BW.h>
#include <U8g2_for_Adafruit_GFX.h>

#include "icons.h"
#include "position.h"
#include "connect.h"
#include "ble_client.h"

#define GxEPD2_DISPLAY_CLASS GxEPD2_BW
#define GxEPD2_DRIVER_CLASS GxEPD2_213_B74

namespace EPD {

extern SPIClass hspi;

// SPI引脚定义
#define PIN_SPI_SCK 13
#define PIN_SPI_DIN 14
#define PIN_SPI_CS 15
#define PIN_SPI_BUSY 25
#define PIN_SPI_RST 26
#define PIN_SPI_DC 27

extern GxEPD2_BW<GxEPD2_213_B74, GxEPD2_213_B74::HEIGHT> display;
extern U8G2_FOR_ADAFRUIT_GFX u8g2;

void displayStatus(const bool autoDisplay);
void displayWifiStatus(const bool autoDisplay);
void displayCollectorStatus(const bool autoDisplay);

void setup();

void clearScreen();
void clearCoreArea();

} // namespace EPD

#endif
