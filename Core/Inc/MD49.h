/*
 * MD49.h
 *
 *  Created on: Aug 14, 2026
 *      Author: juang
 */

#ifndef INC_MD49_H_
#define INC_MD49_H_

#include "main.h"


// ============================================================================
// DEFINITIONS AND COMMANDS OF MD49 DRIVER
// ============================================================================

#define MD49_SYNC_BYTE 0x00

#define MD49_CMD_GET_SPEED1 0x21
#define MD49_CMD_GET_SPEED2 0x22
#define MD49_CMD_GET_ENCODER1 0x23
#define MD49_CMD_GET_ENCODER2 0x24
#define MD49_CMD_GET_ENCODERS 0x25
#define MD49_CMD_GET_VOLTS 0x26
#define MD49_CMD_GET_CURRENT1 0x27
#define MD49_CMD_GET_CURRENT2 0x28
#define MD49_CMD_GET_VERSION 0x29
#define MD49_CMD_GET_ACCELERATION 0x2A
#define MD49_CMD_GET_MODE 0x2B
#define MD49_CMD_GET_VI 0x2C
#define MD49_CMD_GET_ERROR 0x2D
#define MD49_CMD_SET_SPEED1 0x31
#define MD49_CMD_SET_SPEED2 0x32
#define MD49_CMD_SET_MODE 0x34
#define MD49_CMD_RESET_ENCODERS 0x35
#define MD49_CMD_DISABLE_REGULATOR 0x36
#define MD49_CMD_ENABLE_REGULATOR 0x37
#define MD49_CMD_DISABLE_TIMEOUT 0x38
#define MD49_CMD_ENABLE_TIMEOUT 0x39

// ============================================================================
// PROTOTIPES OF THE FUNCTIONS
// ============================================================================


HAL_StatusTypeDef MD49_ExCmd(UART_HandleTypeDef *huart, uint8_t cmd, uint8_t *params, uint8_t n_params, uint8_t *rx_data, uint8_t rx_bytes);
HAL_StatusTypeDef MD49_SetSpeedMotors(UART_HandleTypeDef *huart, uint8_t *speed1, uint8_t *speed2);
HAL_StatusTypeDef MD49_GetEncoders(UART_HandleTypeDef *huart, uint32_t *enc1, uint32_t *enc2);
HAL_StatusTypeDef MD49_GetState(UART_HandleTypeDef *huart, uint8_t *ver, uint8_t *mode, float *v, float *i1, float *i2, uint8_t *t_err, uint8_t *ac);
HAL_StatusTypeDef MD49_GetVelocity(UART_HandleTypeDef *huart, float *vel_1_pps, float *vel_2_pps);

#endif /* INC_MD49_H_ */
