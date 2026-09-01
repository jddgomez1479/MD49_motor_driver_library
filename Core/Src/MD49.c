/*
 * MD49.c
 *
 *  Created on: Aug 14, 2026
 *      Author: juang
 */

#include "MD49.h"

// ============================================================================
// FUNCTIONS OF THE MD49 LIBRARY
// ============================================================================

HAL_StatusTypeDef MD49_ExCmd(UART_HandleTypeDef *huart, uint8_t cmd, uint8_t *params, uint8_t n_params, uint8_t *rx_data, uint8_t rx_bytes)
{
	uint8_t packet_tx[10];
	packet_tx[0] = MD49_SYNC_BYTE;
	packet_tx[1] = cmd;

	if (params != NULL && n_params > 0)
	{
		for (uint8_t i = 0; i < n_params; i++)
		{
			packet_tx[2 + i] = params[i];
		}
	}

	if(HAL_UART_Transmit(huart, packet_tx, 2 + n_params, 100) != HAL_OK) return HAL_ERROR;
	if(rx_bytes == 0 || rx_data == NULL) return HAL_OK;

	return HAL_UART_Receive(huart, rx_data, rx_bytes, 200);
}

HAL_StatusTypeDef MD49_SetSpeedMotors(UART_HandleTypeDef *huart, uint8_t *speed1, uint8_t *speed2) {
	if ((MD49_ExCmd(huart, MD49_CMD_SET_SPEED1, speed1, 1, NULL, 0) == HAL_OK) &&
			(MD49_ExCmd(huart, MD49_CMD_SET_SPEED2, speed2, 1, NULL, 0) == HAL_OK)) return HAL_OK;
	return HAL_ERROR;
}

HAL_StatusTypeDef MD49_GetEncoders(UART_HandleTypeDef *huart, uint32_t *enc1, uint32_t *enc2)
{
	uint8_t buffer_driver[8];

	if (MD49_ExCmd(huart, MD49_CMD_GET_ENCODERS, NULL, 0, buffer_driver, 8) == HAL_OK)
	{
			*enc1 = ((int32_t) buffer_driver[0] << 24)
					| ((int32_t) buffer_driver[1] << 16)
					| ((int32_t) buffer_driver[2] << 8)
					| (int32_t) buffer_driver[3];

			*enc2 = ((int32_t) buffer_driver[4] << 24)
					| ((int32_t) buffer_driver[5] << 16)
					| ((int32_t) buffer_driver[6] << 8)
					| (int32_t) buffer_driver[7];

			return HAL_OK;
	}

	return HAL_ERROR;
}



HAL_StatusTypeDef MD49_GetState(UART_HandleTypeDef *huart, uint8_t *ver, uint8_t *mode, float *v, float *i1, float *i2, uint8_t *t_err, uint8_t *ac) {

	uint8_t buffer_driver[1], err; //ver, mode, err, v, i1, i2, t_err;
	HAL_StatusTypeDef status = HAL_OK;

	if (MD49_ExCmd(huart, MD49_CMD_GET_VERSION, NULL, 0, buffer_driver, 1) == HAL_OK) {
		*ver = buffer_driver[0];
	} else status = HAL_ERROR;

	if (MD49_ExCmd(huart, MD49_CMD_GET_MODE, NULL, 0, buffer_driver, 1) == HAL_OK) {
		*mode = buffer_driver[0];
	} else status = HAL_ERROR;

	if (MD49_ExCmd(huart, MD49_CMD_GET_ERROR, NULL, 0, buffer_driver, 1) == HAL_OK) {
		err = buffer_driver[0];
		if (err & 0x04) *t_err = 2; // bit 2: motor 1 trip
		if (err & 0x08) *t_err = 3; // bit 3: motor 2 trip
		if (err & 0x10) *t_err = 4; // bit 4: motor 1 short
		if (err & 0x20) *t_err = 5; // bit 5: motor 2 short
		if (err & 0x40) *t_err = 6; // bit 6: over 30v
		if (err & 0x80) *t_err = 7; // bit 7: under 16v
	} else status = HAL_ERROR;

	if (MD49_ExCmd(huart, MD49_CMD_GET_VOLTS, NULL, 0, buffer_driver, 1) == HAL_OK) {
		*v = (float)buffer_driver[0];
	} else status = HAL_ERROR;

	if (MD49_ExCmd(huart, MD49_CMD_GET_CURRENT1, NULL, 0, buffer_driver, 1) == HAL_OK) {
		*i1 = (float)buffer_driver[0] / 10.0f;
	} else status = HAL_ERROR;

	if (MD49_ExCmd(huart, MD49_CMD_GET_CURRENT2, NULL, 0, buffer_driver, 1) == HAL_OK) {
		*i2 = (float)buffer_driver[0] / 10.0f;
	} else status = HAL_ERROR;

	if (MD49_ExCmd(huart, MD49_CMD_GET_ACCELERATION, NULL, 0, buffer_driver, 1) == HAL_OK) {
		*ac = buffer_driver[0];
	} else status = HAL_ERROR;

	return status;
}

HAL_StatusTypeDef MD49_GetVelocity(UART_HandleTypeDef *huart, float *vel_1_pps, float *vel_2_pps)
{
	static uint32_t t_ant = 0;
	static int32_t enc1_ant = 0;
	static int32_t enc2_ant = 0;

	uint32_t enc1_act, enc2_act;

	if(MD49_GetEncoders(huart, &enc1_act, &enc2_act) == HAL_OK)
	{

		uint32_t t_act = HAL_GetTick();
		uint32_t dt = t_act - t_ant;

		if (dt > 0)
		{
			float dt_s = dt / 1000.0f;
			int32_t d_enc1 = enc1_act - enc1_ant;
			int32_t d_enc2 = enc2_act - enc2_ant;

			*vel_1_pps = (float)d_enc1 / dt_s;
			*vel_2_pps = (float)d_enc2 / dt_s;

			t_ant = t_act;
			enc1_ant = enc1_act;
			enc2_ant = enc2_act;
		}

		return HAL_OK;
	}

	return HAL_ERROR;
}
