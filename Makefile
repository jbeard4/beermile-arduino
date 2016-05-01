ARDUINO_DIR            = /usr/share/arduino
TARGET                 = BeerMileNFC
ARDUINO_LIBS					 = SPI PN532_SPI PN532 PN532_HSU PN532 Wire PN532_I2C PN532 Wire/utility NDEF Grove_LCD_RGB_Backlight
MCU                    = atmega328p
F_CPU                  = 16000000
ARDUINO_PORT           = /dev/ttyACM0
AVRDUDE_ARD_BAUDRATE   = 115200
AVRDUDE_ARD_PROGRAMMER = arduino

include /usr/share/arduino/Arduino.mk
