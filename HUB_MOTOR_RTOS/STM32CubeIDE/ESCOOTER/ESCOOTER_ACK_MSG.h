/*
 * ESCOOTER_ACK_MSG.h
 *
 *  Created on: 10 Jul 2023
 *      Author: TerenceLeung
 */

#ifndef ESCOOTER_ESCOOTER_ACK_MSG_H_
#define ESCOOTER_ESCOOTER_ACK_MSG_H_

#ifdef __cplusplus
extern "C"
{
#endif

/*Send Acknowledgement Message Once Command has successfully transferred*/
uint8_t THROTTLE             = 0x08;
uint8_t BRAKE                = 0X09;
uint8_t release_Brake        = 0x0A;
uint8_t debug_torque_output  = 0x0B;
uint8_t debug_config_success = 0x0C;

#ifdef __cplusplus
}
#endif
#endif /* ESCOOTER_ESCOOTER_ACK_MSG_H_ */
