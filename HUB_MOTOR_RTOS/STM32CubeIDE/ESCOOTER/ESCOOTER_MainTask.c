/*
 * ESCOOTER_StateMachine.c
 *
 *  Created on: 7 Jul 2023
 *      Author: TerenceLeung
 *
 *  ESCOOTER_MainTask.c provides functionalities for E-Scooter Driving Control, which is the core of Driving System
 *  Please verify it under the SYSTEM Debugging Mode
 */
#include "ESCOOTER_MainTask.h"
#include "ESCOOTER_MonitorTask.h"
#include "ESCOOTER_BrakeAndThrottle.h"
#include "ESCOOTER_DRIVING.h"
#include "escooter_config.h"
#include "mc_api.h"
#include "main.h"
#include "cmsis_os.h"

ESCOOTER_Operation tempHandle; //Store Dynamic Variables
ESCOOTER_Operation opInitHandle; //Store Static Variables

ESCOOTER_Physical_State pstateHandle; //Monitor the Escooter's Electrical & Mechanical Parameters
ESCOOTER_BrakeANDThrottleInput inputHandle;

/*ESboot() must be called when the E-Scooter is turned-on！*/
void ESboot()
{
    opInitHandle.bDrivingState    = DRIVING_IDLE;
    opInitHandle.BRAKE_Trigger    = false;
    opInitHandle.THROTTLE_Pressed = false;
    opInitHandle.need_KICK_OFF    = true;
    opInitHandle.ESCOOTER_MOVE    = false;

    /*Save the current driving status. IDLE Mode is set in Default*/
    ESCOOTER_saveStatus(opInitHandle.bDrivingState);

    /*Create The Task for controlling the E-Scooter Driving Status !!!*/
    osThreadDef(driving,ESCOOTER_DrivingTaskControl,osPriorityBelowNormal,0,128);
    opInitHandle.EScooterInDriving = osThreadCreate(osThread(driving),NULL);

    tempHandle = opInitHandle;

    /*Initialize the E-Scooter in Ambler Mode*/
    ESCOOTER_DrivingModeSetDefault(&inputHandle);
    ESCOOTER_DetermineDrivingMode(&inputHandle);

    /*Initialize the Motor Parameters*/
    ESCOOTER_PhysicalParameterInit(&pstateHandle);


}

/*Triggered by Dashboard's commands! Configure the driving mode by adjusting the current, speed and acceleration ramp limits*/
/*Save those parameters in form of &inputHandle pointer*/
/*Input some dummy commands -> check it*/
/*It's assumed that the E-Scooter is in IDLE State*/
void ESCOOTER_DriveModeConfig(int16_t max_IQ, int16_t allowable_rpm, uint16_t acceleration_ramp)
{
	ESCOOTER_ConfigDrivingMode(&inputHandle,max_IQ,allowable_rpm,acceleration_ramp);
}

/**To be called by motor control protocol**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
void ESCOOTER_InputThrottleSignal(int16_t targetCurrent)
{
	inputHandle.TARGET_IQ = targetCurrent;

	/*to give the message if the throttle is pressed or not ! */
	tempHandle.THROTTLE_Pressed = ESCOOTER_ThrottleSignalTrigger(&inputHandle);

}

/**To be called by motor control protocol**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
void ESCOOTER_InputBrakeSignal()
{
	tempHandle.BRAKE_Trigger = ESCOOTER_BrakeSignalTrigger(&tempHandle,&inputHandle);

	/*to give the message if the brake is pressed or not*/
}

/**To be called by motor control protocol**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
void ESCOOTER_DEBUG_CancelBrakeSignal()
{
	ESCOOTER_DEBUG_ReleaseBrakeSignal(&tempHandle,&inputHandle);
}

/**KICK OFF STEP**/
/**Before running the Main Task, please verify this function carefully**/
/*Input some dummy commands -> check it*/
bool ESCOOTER_IsCarReady()
{
	/*Before using the MC_API to track the Motor's status, to ensure the State Machine runs properly, it's better to input dummy data to check if the
	 *logic is right*/
    int16_t minSpeed = MC_GetMecSpeedAverageMotor1();
    //int16_t minSpeed = 110;
    if (minSpeed >= 100)
    {
    	return true;
    }
    return false;
}

/**Control The E-Scooter's Main Task**/
/*Please check all the functionalities e.g. brake,throttle,parameter inputs carefully before running the High Frequency Task*/
/*Input some dummy commands -> check it*/
__weak void ESCOOTER_StateMachineHighFrequencyTask(void const *argument)
{
     for(;;)
     {
    	  /*Before using the MC_API to track the Motor's status, to ensure the State Machine runs properly, it's better to input dummy data to check if the
    	   *logic is right*/
           uint16_t error = MC_GetOccurredFaultsMotor1();
           ESCOOTER_saveStatus(tempHandle.bDrivingState);
           /*To verify the state machine for debug purpose, please un-comment the following lines: */
    	   //int32_t status = 0;
    	   //uint16_t error = 0;

         if ((tempHandle.ESCOOTER_MOVE == false) && (tempHandle.need_KICK_OFF == true))
         {
             if ( (ESCOOTER_IsCarReady() == true) && (tempHandle.THROTTLE_Pressed == true) && (error == 0)
                   && (tempHandle.BRAKE_Trigger == false) )
             {
            	 tempHandle.ESCOOTER_MOVE = true;
            	 tempHandle.need_KICK_OFF = false;
            	 tempHandle.bDrivingState = DRIVING_START;
            	 ESCOOTER_saveStatus(tempHandle.bDrivingState);
             }
             else
             {
            	 tempHandle.ESCOOTER_MOVE = false;
            	 tempHandle.need_KICK_OFF = true;
            	 tempHandle.bDrivingState = DRIVING_IDLE;
            	 //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_13,GPIO_PIN_SET);
            	 //HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14,GPIO_PIN_RESET);
            	 ESCOOTER_saveStatus(tempHandle.bDrivingState);
             }
         }
         else if (tempHandle.ESCOOTER_MOVE == true)
		 {
        	 /*Stop it normally
        	  * There are several ways to stop the E-Scooter:
        	  * --> User Triggers the Brake Signal (Press Brake)
        	  * --> User Releases the Throttle with Iq closing to 0 !
        	  * --> Emergency Stop
        	  * The program must handle with the above conditions
        	  * To test for the second condition, we must get a throttle with dash-board / generate some dummy data!
        	  * */
             if (tempHandle.BRAKE_Trigger == true || (inputHandle.TARGET_IQ <= 6) )

             {
            	 tempHandle.THROTTLE_Pressed = false;
            	 tempHandle.need_KICK_OFF = true;
            	 tempHandle.bDrivingState = DRIVING_STOP;
            	 tempHandle.BRAKE_Trigger = false;
            	 tempHandle.ESCOOTER_MOVE = false;
            	 ESCOOTER_saveStatus(tempHandle.bDrivingState);
            	 HAL_Delay(1500);
            	 tempHandle.bDrivingState = DRIVING_IDLE;
            	 //osSignalSet(tempHandle.EScooterInDriving,0x03);
                 //HAL_Delay(2000);
             }

             /*Emergency Stop due to motor error, system error, ..... Once error occurs, system jumps to the following instructions... */
             /*As the MCSDK automatically stops the motor,we no need to call the driving stop functions....*/
             else if(error != 0)
             {
            	 tempHandle.THROTTLE_Pressed = false;
            	 tempHandle.need_KICK_OFF = true;
            	 tempHandle.bDrivingState = DRIVING_STOP;
            	 tempHandle.BRAKE_Trigger = false;
            	 tempHandle.ESCOOTER_MOVE = false;
            	 ESCOOTER_saveStatus(tempHandle.bDrivingState);
            	 HAL_Delay(1500);
            	 tempHandle.bDrivingState = DRIVING_IDLE;
             }
		 }
     }
}


/**Check the Motor Parameters Regularly**/
/*Input some dummy commands -> check it*/
void ESCOOTER_ParameterMonitoring(void const *argument)
{
    for(;;)
    {
    	pstateHandle = ESCOOTER_PhysicalParameterMonitoring(&pstateHandle);
    }
}
