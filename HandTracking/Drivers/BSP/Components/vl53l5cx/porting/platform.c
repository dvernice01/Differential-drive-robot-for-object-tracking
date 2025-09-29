/**
  ******************************************************************************
  * @file    platform.h
  * @author  IMG SW Application Team
  * @brief   This file contains all the platform functions prototypes
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

#include "platform.h"
#include "main.h"
#include "stm32f4xx_hal.h"
#include <stdio.h>

extern I2C_HandleTypeDef hi2c1;

int32_t VL53L5CX_RdByte(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_value)
{
  return p_platform->Read(p_platform->address, RegisterAdress, p_value, 1U);
}

int32_t VL53L5CX_WrByte(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t value)
{
  return p_platform->Write(p_platform->address, RegisterAdress, &value, 1U);
}

int32_t VL53L5CX_WrMulti(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_values,
		uint32_t size)
{
  return p_platform->Write(p_platform->address, RegisterAdress, p_values, size);
}

int32_t VL53L5CX_RdMulti(
		VL53L5CX_Platform *p_platform,
		uint16_t RegisterAdress,
		uint8_t *p_values,
		uint32_t size)
{
  return p_platform->Read(p_platform->address, RegisterAdress, p_values, size);
}

void VL53L5CX_SwapBuffer(
    uint8_t     *buffer,
    uint16_t     size)
{
  uint32_t i, tmp;


  for(i = 0; i < size; i = i + 4)
  {
    tmp = (
      buffer[i]<<24)
    |(buffer[i+1]<<16)
    |(buffer[i+2]<<8)
    |(buffer[i+3]);

    memcpy(&(buffer[i]), &tmp, 4);
  }
}

int32_t VL53L5CX_WaitMs(
		VL53L5CX_Platform *p_platform,
		uint32_t TimeMs)
{
  uint32_t tickstart;
  tickstart = p_platform->GetTick();

  while ((p_platform->GetTick() - tickstart) < TimeMs);

  return 0;
}


/**
 * @brief Scrive su I²C (indirizzo a 16 bit).
 */
int32_t VL53L5CX_I2C_Write(uint16_t DevAddr, uint16_t Reg,
                           uint8_t *pData, uint16_t Length) {
    if (HAL_I2C_Mem_Write(&hi2c1, DevAddr, Reg,
                          I2C_MEMADD_SIZE_16BIT, pData,
                          Length, 1000) == HAL_OK) {
        return 0; // Successo
    }
    return 255; // Errore
}

/**
 * @brief Legge da I²C (indirizzo a 16 bit).
 */
int32_t VL53L5CX_I2C_Read(uint16_t DevAddr, uint16_t Reg,
                          uint8_t *pData, uint16_t Length) {
    if (HAL_I2C_Mem_Read(&hi2c1, DevAddr, Reg,
                         I2C_MEMADD_SIZE_16BIT, pData,
                         Length, 1000) == HAL_OK) {
        return 0; // Successo
    }
    return 255; // Errore
}

/**
 * @brief Restituisce il tick corrente (ms).
 */
int32_t VL53L5CX_GetTick(void) {
    return HAL_GetTick();
}
