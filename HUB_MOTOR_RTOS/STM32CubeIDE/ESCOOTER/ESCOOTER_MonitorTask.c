/*
 * ESCOOTER_MonitorTask.c
 *
 *  Created on: 7 Jul 2023
 *      Author: TerenceLeung
 */
#include "ESCOOTER_MonitorTask.h"
#include "mc_api.h"
#include "main.h"

void ESCOOTER_PhysicalParameterInit(ESCOOTER_Physical_State *stateHandle)
{
	stateHandle->current_speed = 0;
	stateHandle->phase_current = 0;
	stateHandle->phase_voltage = 0;
	stateHandle->motor_status  = 0;
}

ESCOOTER_Physical_State ESCOOTER_PhysicalParameterMonitoring(ESCOOTER_Physical_State *stateHandle)
{
	/*You could test it by inputing dummy data */
    stateHandle->current_speed = MC_GetMecSpeedAverageMotor1();
    stateHandle->phase_current = MC_GetPhaseCurrentAmplitudeMotor1();
    stateHandle->phase_voltage = MC_GetPhaseVoltageAmplitudeMotor1();
    stateHandle->motor_status = (int32_t)MC_GetSTMStateMotor1();
    return *stateHandle;
}
