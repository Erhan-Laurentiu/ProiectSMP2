#include <msp430.h>
#include <stdio.h>

#include"BME280.h"
#include"CCS811.h"
#include"MAX44009.h"
#include"I2C.h"
#include"UART1.h"
#include"UART0.h"
#include"Delay.h"
#include"PCF8574.h"
#include"LCD.h"
#include"ESP8266.h"

#ifndef MAIN_H_
#define MAIN_H_




inline void WATCHDOG_DISABLE();
void HW_INIT();
void SENSORS_INIT();
void SENSOR_READ();
void SendInt(long sensorValue);
void SENSORS_SEND_UART();
void SENSORS_SEND_LCD();

void SENSORS_READ();

void ESP_RESET();
void ESP_PIN_INIT();



void SENSOR_SEND_UART1();

#endif
