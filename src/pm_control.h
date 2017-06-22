/*
   Phobia Motor Controller for RC and robotics.
   Copyright (C) 2017 Roman Belov <romblv@gmail.com>

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

#ifndef _H_PM_CONTROL_
#define _H_PM_CONTROL_

enum {
	PMC_BIT_ZERO_DRIFT_ESTIMATION		= 0x0001,
	PMC_BIT_HIGH_FREQUENCY_INJECTION	= 0x0002,
	PMC_BIT_FLUX_POLARITY_DETECTION		= 0x0004,
	PMC_BIT_THERMAL_DRIFT_ESTIMATION	= 0x0008,
	PMC_BIT_BEMF_WAVEFORM_COMPENSATION	= 0x0010,
	PMC_BIT_SPEED_CONTROL_LOOP		= 0x0100,
	PMC_BIT_POSITION_CONTROL_LOOP		= 0x0200,
	PMC_BIT_FORCED_CONTROL			= 0x0400,
	PMC_BIT_POWER_CONTROL_LOOP		= 0x0800,
};

enum {
	PMC_STATE_IDLE				= 0,
	PMC_STATE_ZERO_DRIFT,
	PMC_STATE_WAVE_HOLD,
	PMC_STATE_WAVE_SINE,
	PMC_STATE_CALIBRATION,
	PMC_STATE_START,
	PMC_STATE_STOP,
	PMC_STATE_END
};

enum {
	PMC_LU_DISABLED				= 0,
	PMC_LU_LOW_REGION,
	PMC_LU_HIGH_REGION
};

enum {
	PMC_OK					= 0,
	PMC_ERROR_CURRENT_SENSOR_A,
	PMC_ERROR_CURRENT_SENSOR_B,
	PMC_ERROR_OPEN_CIRCUIT,
	PMC_ERROR_OVERCURRENT,
	PMC_ERROR_LOW_VOLTAGE,
	PMC_ERROR_HIGH_VOLTAGE,
	PMC_ERROR_STABILITY_LOSS,
};

typedef struct {

	/* Frequency (Hz).
	 * */
	float		freq_hz;
	float		dT;

	/* PWM configuration.
	 * */
	int		pwm_resolution;
	int		pwm_minimal_pulse;
	int		pwm_dead_time;

	/* FSM variables.
	 * */
	int		m_bitmask;
	int		m_state;
	int		m_phase;
	int		m_errno;

	/* Timer variables.
	 * */
	int		t_value;
	int		t_end;

	/* Timer configuration.
	 * */
	float		tm_drift;
	float		tm_hold;
	float		tm_sine;
	float		tm_measure;
	float		tm_end;

	/* Current feedback.
	 * */
	float		fb_range;
	float		fb_iA;
	float		fb_iB;

	/* Probing variables.
	 * */
	float		pb_i_hold;
	float		pb_i_hold_Q;
	float		pb_i_sine;
	float		pb_freq_sine_hz;
	float		pb_speed_low;
	float		pb_speed_high;
	float		pb_settle_threshold;
	float		pb_gain_P;
	float		pb_gain_I;
	float		pb_temp[4];
	float		pb_DFT[8];

	/* Scale constants.
	 * */
	float		scal_A[2];
	float		scal_B[2];
	float		scal_U[2];

	/* Fault limits.
	 * */
	float		fault_residual_maximal;
	float		fault_residual_maximal_hold;
	float		fault_drift_maximal;
	float		fault_low_voltage;
	float		fault_low_voltage_hold;
	float		fault_high_voltage;
	float		fault_high_voltage_hold;

	/* Fault timers.
	 * */
	int		fault_residual_maximal_t;
	int		fault_low_voltage_t;
	int		fault_high_voltage_t;

	/* Actual VSI voltage.
	 * */
	float		vsi_X;
	float		vsi_Y;
	float		vsi_D;
	float		vsi_Q;

	/* Luenberger observer.
	 * */
	int		lu_region;
	float		lu_X[5];
	int		lu_revol;
	float		lu_gain_K[7];
	float		lu_low_threshold;
	float		lu_high_threshold;
	float		lu_temp[2];

	/* Measurement residual.
	 * */
	float		lu_residual_D;
	float		lu_residual_Q;
	float		lu_residual_variance;

	/* HFI observer.
	 * */
	float		hf_freq_hz;
	float		hf_swing_D;
	float		hf_flux_polarity;
	float		hf_CS[2];
	float		hf_gain_K[3];

	/* BEMF waveform compensation.
	 * */
	float           bemf_DFT[64];
	float           bemf_TEMP[32];
	float           bemf_gain_K;
	int		bemf_N;
	float		bemf_D;
	float		bemf_Q;

	/* Thermal drift.
	 * */
	float		thermal_R;
	float		thermal_gain_R[2];

	/* Zero drift.
	 * */
	float		drift_A;
	float		drift_B;
	float		drift_Q;

	/* Supply voltage.
	 * */
	float		const_U;

	/* Electrical constants.
	 * */
	float		const_E;
	float		const_R;
	float		const_Ld;
	float		const_Lq;

	/* Mechanical constants.
	 * */
	int		const_Zp;
	float		const_J;

	/* Current control loop.
	 * */
	float		i_low_maximal;
	float		i_high_maximal;
	float		i_power_consumption_maximal;
	float		i_power_regeneration_maximal;
	float		i_set_point_D;
	float		i_set_point_Q;
	float		i_slew_rate_D;
	float		i_slew_rate_Q;
	float		i_track_point_D;
	float		i_track_point_Q;
	float		i_integral_D;
	float		i_integral_Q;
	float		i_gain_P_D;
	float		i_gain_I_D;
	float		i_gain_P_Q;
	float		i_gain_I_Q;

	/* Speed control loop.
	 * */
	float		s_maximal;
	float		s_set_point;
	float		s_slew_rate;
	float		s_forced_D;
	float		s_forced_slew_rate;
	float		s_track_point;
	float		s_track_point_p[2];
	float		s_nonl_X4;
	float		s_nonl_gain_F;
	float		s_nonl_range;
	float		s_gain_P;

	/* Position control loop.
	 * */
	float		p_set_point[2];
	float		p_set_point_s;
	int		p_set_point_revol;
	float		p_gain_P;

	/* Power control loop.
	 * */
	float		w_set_point_watt;
	int		w_direction;

	/* Low-pass gains.
	 * */
	float		lp_gain[2];

	/* Informational.
	 * */
	float		n_power_watt;
	float		n_temperature_c;

	/* Control interface.
	 * */
	void 		(* pDC) (int, int, int);
	void 		(* pZ) (int);
}
pmc_t;

void pmc_default(pmc_t *pm);
void pmc_feedback(pmc_t *pm, float iA, float iB);
void pmc_voltage(pmc_t *pm, float uS);
void pmc_request(pmc_t *pm, int req);

const char *pmc_strerror(int errno);

void pmc_resistance(const float DFT[8], float *R);
void pmc_impedance(const float DFT[8], float freq_hz, float IMP[6]);

#endif /* _H_PM_CONTROL_ */

