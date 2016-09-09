/*
   Phobia Motor Controller for RC and robotics.
   Copyright (C) 2016 Roman Belov <romblv@gmail.com>

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include <stddef.h>

#include "hal/hal.h"
#include "hal/flash.h"
#include "hal/pwm.h"
#include "hal/usart.h"

#include "freertos/FreeRTOS.h"

#include "main.h"
#include "pmc.h"
#include "lib.h"
#include "sh.h"

#define CONF_VERSION		1

typedef struct {

	unsigned int		id;
	unsigned int		version;
	unsigned int		content[1021];
	unsigned int 		crc32;
}
conf_block_t;

unsigned int * const conf_vars[] = {

	(void *) &halPWM.freq_hz,
	(void *) &halPWM.dead_time_ns,
	(void *) &halUSART_baudRate,

	(void *) &ma.av_default_time,
	(void *) &ma.ap_J_measure_T,

	(void *) &pm.pwm_minimal_pulse,
	(void *) &pm.pwm_dead_time,
	(void *) &pm.m_bitmask,
	(void *) &pm.T_drift,
	(void *) &pm.T_hold,
	(void *) &pm.T_sine,
	(void *) &pm.T_measure,
	(void *) &pm.T_end,
	(void *) &pm.fb_range,
	(void *) &pm.wave_i_hold_X,
	(void *) &pm.wave_i_hold_Y,
	(void *) &pm.wave_i_sine,
	(void *) &pm.wave_freq_sine_hz,
	(void *) &pm.wave_gain_P,
	(void *) &pm.wave_gain_I,
	(void *) &pm.scal_A[0],
	(void *) &pm.scal_A[1],
	(void *) &pm.scal_B[0],
	(void *) &pm.scal_B[1],
	(void *) &pm.scal_U[0],
	(void *) &pm.scal_U[1],
	(void *) &pm.fault_residual_maximal,
	(void *) &pm.fault_drift_maximal,
	(void *) &pm.fault_low_voltage,
	(void *) &pm.fault_high_voltage,
	(void *) &pm.lu_gain_K[0],
	(void *) &pm.lu_gain_K[1],
	(void *) &pm.lu_gain_K[2],
	(void *) &pm.lu_gain_K[3],
	(void *) &pm.lu_gain_K[4],
	(void *) &pm.lu_gain_K[5],
	(void *) &pm.lu_gain_K[6],
	(void *) &pm.lu_threshold_low,
	(void *) &pm.lu_threshold_high,
	(void *) &pm.hf_freq_hz,
	(void *) &pm.hf_swing_D,
	(void *) &pm.hf_gain_K[0],
	(void *) &pm.hf_gain_K[1],
	(void *) &pm.hf_gain_K[2],
	(void *) &pm.bemf_DFT[0],
	(void *) &pm.bemf_DFT[1],
	(void *) &pm.bemf_DFT[2],
	(void *) &pm.bemf_DFT[3],
	(void *) &pm.bemf_DFT[4],
	(void *) &pm.bemf_DFT[5],
	(void *) &pm.bemf_DFT[6],
	(void *) &pm.bemf_DFT[7],
	(void *) &pm.bemf_DFT[8],
	(void *) &pm.bemf_DFT[9],
	(void *) &pm.bemf_DFT[10],
	(void *) &pm.bemf_DFT[11],
	(void *) &pm.bemf_DFT[12],
	(void *) &pm.bemf_DFT[13],
	(void *) &pm.bemf_DFT[14],
	(void *) &pm.bemf_DFT[15],
	(void *) &pm.bemf_DFT[16],
	(void *) &pm.bemf_DFT[17],
	(void *) &pm.bemf_DFT[18],
	(void *) &pm.bemf_DFT[19],
	(void *) &pm.bemf_DFT[20],
	(void *) &pm.bemf_DFT[21],
	(void *) &pm.bemf_DFT[22],
	(void *) &pm.bemf_DFT[23],
	(void *) &pm.bemf_DFT[24],
	(void *) &pm.bemf_DFT[25],
	(void *) &pm.bemf_DFT[26],
	(void *) &pm.bemf_DFT[27],
	(void *) &pm.bemf_DFT[28],
	(void *) &pm.bemf_DFT[29],
	(void *) &pm.bemf_DFT[30],
	(void *) &pm.bemf_DFT[31],
	(void *) &pm.bemf_DFT[32],
	(void *) &pm.bemf_DFT[33],
	(void *) &pm.bemf_DFT[34],
	(void *) &pm.bemf_DFT[35],
	(void *) &pm.bemf_DFT[36],
	(void *) &pm.bemf_DFT[37],
	(void *) &pm.bemf_DFT[38],
	(void *) &pm.bemf_DFT[39],
	(void *) &pm.bemf_DFT[40],
	(void *) &pm.bemf_DFT[41],
	(void *) &pm.bemf_DFT[42],
	(void *) &pm.bemf_DFT[43],
	(void *) &pm.bemf_DFT[44],
	(void *) &pm.bemf_DFT[45],
	(void *) &pm.bemf_DFT[46],
	(void *) &pm.bemf_DFT[47],
	(void *) &pm.bemf_DFT[48],
	(void *) &pm.bemf_DFT[49],
	(void *) &pm.bemf_DFT[50],
	(void *) &pm.bemf_DFT[51],
	(void *) &pm.bemf_DFT[52],
	(void *) &pm.bemf_DFT[53],
	(void *) &pm.bemf_DFT[54],
	(void *) &pm.bemf_DFT[55],
	(void *) &pm.bemf_DFT[56],
	(void *) &pm.bemf_DFT[57],
	(void *) &pm.bemf_DFT[58],
	(void *) &pm.bemf_DFT[59],
	(void *) &pm.bemf_DFT[60],
	(void *) &pm.bemf_DFT[61],
	(void *) &pm.bemf_DFT[62],
	(void *) &pm.bemf_DFT[63],
	(void *) &pm.bemf_gain_K,
	(void *) &pm.bemf_N,
	(void *) &pm.thermal_gain_R[0],
	(void *) &pm.thermal_gain_R[1],
	(void *) &pm.const_U,
	(void *) &pm.const_E,
	(void *) &pm.const_R,
	(void *) &pm.const_Ld,
	(void *) &pm.const_Lq,
	(void *) &pm.const_Zp,
	(void *) &pm.const_J,
	(void *) &pm.i_high_maximal,
	(void *) &pm.i_low_maximal,
	(void *) &pm.i_power_consumption_maximal,
	(void *) &pm.i_power_regeneration_maximal,
	(void *) &pm.i_slew_rate_D,
	(void *) &pm.i_slew_rate_Q,
	(void *) &pm.i_gain_P_D,
	(void *) &pm.i_gain_I_D,
	(void *) &pm.i_gain_P_Q,
	(void *) &pm.i_gain_I_Q,
	(void *) &pm.p_nonl_gain_F,
	(void *) &pm.p_nonl_range,
	(void *) &pm.p_slew_rate_w,
	(void *) &pm.p_forced_D,
	(void *) &pm.p_forced_slew_rate_w,
	(void *) &pm.p_gain_D,
	(void *) &pm.p_gain_P,
	(void *) &pm.lp_gain[0],
	(void *) &pm.lp_gain[1],

	NULL
};

static conf_block_t *
conf_block_scan(int vflag)
{
	conf_block_t			*curr, *last;

	curr = flash_sector_map[0];
	last = NULL;

	do {
		if (vflag || curr->version == CONF_VERSION) {

			if (crc32b(curr, sizeof(conf_block_t) - 4) == curr->crc32) {

				last = (curr->id > last->id) ? curr : last;
			}
		}

		curr += 1;

		if ((void *) curr >= flash_sector_map[FLASH_SECTOR_MAX - 1])
			break;
	}
	while (1);

	return last;
}

int conf_block_load()
{
	conf_block_t		*block;
	int			n, rc = -1;

	block = conf_block_scan(0);

	if (block != NULL) {

		for (n = 0; conf_vars[n] != NULL; ++n)
			*conf_vars[n] = block->content[n];

		pm.const_Ld_inversed = 1.f / pm.const_Ld;
		pm.const_Lq_inversed = 1.f / pm.const_Lq;

		rc = 0;
	}

	return rc;
}

static int
conf_is_block_dirty(const conf_block_t *block)
{
	const unsigned int	*us, *ue;
	int			dirty = 0;

	us = (const unsigned int *) block;
	ue = (const unsigned int *) (block + 1);

	while (us < ue) {

		if (*us++ != 0xFFFFFFFF)
			dirty = 1;
	}

	return dirty;
}

static int
conf_get_sector(const conf_block_t *block)
{
	int			n = 0;

	do {
		if (flash_sector_map[n + 1] > (void *) block)
			break;

		++n;
	}
	while (1);

	return n;
}

int conf_block_save()
{
	conf_block_t		*block, *temp;
	unsigned int		newid;
	int			n, rc;

	block = conf_block_scan(1);

	if (block != NULL) {

		newid = block->id + 1;
		block += 1;

		if ((void *) block >= flash_sector_map[FLASH_SECTOR_MAX - 1])
			block = flash_sector_map[0];
	}
	else {
		newid = 1;
		block = flash_sector_map[0];
	}

	if (conf_is_block_dirty(block)) {

		flash_erase(conf_get_sector(block));
	}

	temp = pvPortMalloc(sizeof(conf_block_t));

	temp->id = newid;
	temp->version = CONF_VERSION;

	for (n = 0; conf_vars[n] != NULL; ++n)
		temp->content[n] = *conf_vars[n];

	for (; n < sizeof(temp->content) / 4; ++n)
		temp->content[n] = 0xFFFFFFFF;

	temp->crc32 = crc32b(temp, sizeof(conf_block_t) - 4);

	flash_write(block, temp, sizeof(conf_block_t));

	vPortFree(temp);

	rc = (crc32b(block, sizeof(conf_block_t) - 4) == block->crc32) ? 0 : -1;

	return rc;
}

SH_DEF(conf_save)
{
	int			rc;

	if (pm.lu_region != PMC_LU_DISABLED)
		return ;

	printf("Flash ... ");

	rc = conf_block_save();

	printf("%s" EOL, (rc == 0) ? "Done" : "Failed");
}

SH_DEF(conf_info)
{
	conf_block_t		*block;

	block = conf_block_scan(0);
	block = (block == NULL) ? conf_block_scan(1) : block;

	if (block != NULL) {

		printf("ID %i" EOL, block->id);
		printf("VER %i / %i" EOL, block->version, CONF_VERSION);
	}
	else {
		printf("Invalid" EOL);
	}
}

