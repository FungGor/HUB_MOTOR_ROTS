/*
 * ESCOOTER_DRIVING.h
 *
 *  Created on: 18 Jul 2023
 *      Author: TerenceLeung
 */

#ifndef ESCOOTER_ESCOOTER_DRIVING_H_
#define ESCOOTER_ESCOOTER_DRIVING_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "stdint.h"
#include "stdbool.h"
#include "ESCOOTER_MainTask.h"
#include "ESCOOTER_BrakeAndThrottle.h"

void ESCOOTER_saveStatus (uint8_t state);

ESCOOTER_Driving_State ESCOOTER_getStatus();

void ESCOOTER_Set_Limit(ESCOOTER_BrakeANDThrottleInput *limitHandle);

void ESCOOTER_Driving_Start();

void ESCOOTER_Driving_Stop();

void ESCOOTER_DrivingTaskControl(void const * argument);

#ifdef __cplusplus
}
#endif
#endif /* ESCOOTER_ESCOOTER_DRIVING_H_ */
