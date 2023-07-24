/*
 * ESCOOTER_StateMachine.h
 *
 *  Created on: 7 Jul 2023
 *      Author: TerenceLeung
 */

#ifndef ESCOOTER_ESCOOTER_MAINTASK_H_
#define ESCOOTER_ESCOOTER_MAINTASK_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdbool.h"
#include "stdint.h"
#include "cmsis_os.h"

typedef enum
{
	DRIVING_IDLE  = 0,  /*EScooter is in standby mode! */
	DRIVING_START = 1,  /*EScooter is in driving mode! */
	DRIVING_STOP  = 2   /*EScooter is in stop mode! */
}ESCOOTER_Driving_State;

typedef struct
{
	ESCOOTER_Driving_State bDrivingState;
	bool BRAKE_Trigger;
    bool THROTTLE_Pressed;
    bool need_KICK_OFF;
    bool ESCOOTER_MOVE;
	osThreadId EScooterInDriving;
}ESCOOTER_Operation;


void ESboot();

void ESCOOTER_DriveModeConfig(int16_t max_IQ, int16_t allowable_rpm, uint16_t acceleration_ramp);

bool ESCOOTER_IsCarReady();

void ESCOOTER_InputThrottleSignal(int16_t targetCurrent);

void ESCOOTER_InputBrakeSignal();

void ESCOOTER_DEBUG_CancelBrakeSignal();

void ESCOOTER_StateMachineHighFrequencyTask(void const *argument);

void ESCOOTER_ParameterMonitoring(void const *argument);



#ifdef __cplusplus
}
#endif
#endif /* ESCOOTER_ESCOOTER_MAINTASK_H_ */
