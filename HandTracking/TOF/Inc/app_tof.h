#ifndef APP_TOF_H
#define APP_TOF_H

#include "vl53l5cx_api.h"
#include "vl53l5cx_plugin_detection_thresholds.h"

extern VL53L5CX_Configuration p_dev;
extern VL53L5CX_ResultsData results;
//extern VL53L5CX_DetectionThresholds p_thresholds;

extern uint8_t frequency;
extern uint8_t sharpener_percent;
extern uint8_t enable;
extern uint8_t i;
extern uint8_t ready;
extern int a;
extern int b;
extern int c;
extern int d;

//static int32_t stm32_write(uint16_t devAddr, uint16_t regAddr, uint8_t *p_data, uint16_t size);
//static int32_t stm32_read(uint16_t devAddr, uint16_t regAddr,uint8_t *p_data, uint16_t size);
//static int32_t stm32_read(uint16_t devAddr, uint16_t regAddr,uint8_t *p_data, uint16_t size);
//void do_xtalk_calibration_once(void);
void HardwareReset(void);
void Tof_init(void);
//void I2C_Scan(void);
void Tof_conf(void);
void feedbackcontroll(void);

#endif
