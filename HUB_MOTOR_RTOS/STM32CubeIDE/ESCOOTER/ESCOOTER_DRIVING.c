/*
 * ESCOOTER_DRIVING.c
 *
 *  Created on: 18 Jul 2023
 *      Author: TerenceLeung
 */

#include "ESCOOTER_DRIVING.h"
#include "cmsis_os.h"
#include "mc_api.h"
#include "main.h"

ESCOOTER_Driving_State Driving_State;
ESCOOTER_BrakeANDThrottleInput modeControl;

void ESCOOTER_saveStatus (uint8_t state)
{
	  Driving_State = state;
}

ESCOOTER_Driving_State ESCOOTER_getStatus()
{
	return Driving_State;
}

void ESCOOTER_Set_Limit(ESCOOTER_BrakeANDThrottleInput *limitHandle)
{
     modeControl = *limitHandle;
}

void ESCOOTER_Driving_Start()
{
	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_SET);
	/*Set acceleration ramp*/
}

void ESCOOTRT_Driving_Stop()
{
	//HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin,GPIO_PIN_RESET);
	/*Stop Motor!*/
}

void ESCOOTER_DrivingTaskControl(void const * argument)
{
     for(;;)
     {
    	 ESCOOTER_Driving_State Driving = ESCOOTER_getStatus();
         switch(Driving)
         {
              case DRIVING_IDLE:
            	  ESCOOTRT_Driving_Stop();
            	  break;

              case DRIVING_START:
            	  ESCOOTER_Driving_Start();
            	  break;

              case DRIVING_STOP:
            	  ESCOOTRT_Driving_Stop();
            	  break;

              default:
            	  break;
         }
     }
}
