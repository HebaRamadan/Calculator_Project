/*
 * Keypad_cfg.h
 *
 *  Created on: Dec 3, 2020
 *      Author: Heba Ramadan
 */

#ifndef KEYPAD_CFG_H_
#define KEYPAD_CFG_H_

//We can change in this file only that's depend on which PIN I need to connect my Keypad
#define KEYPAD_u8R1_PIN DIO_u8PIN_16	      //R1 At PORTB PIN_0
#define KEYPAD_u8R2_PIN DIO_u8PIN_17          //R2 At PORTB PIN_1
#define KEYPAD_u8R3_PIN DIO_u8PIN_18		  //R3 At PORTB PIN_2
#define KEYPAD_u8R4_PIN DIO_u8PIN_19		  //R4 At PORTB PIN_3
#define KEYPAD_u8R5_PIN DIO_u8PIN_20

#define KEYPAD_u8C1_PIN DIO_u8PIN_21		  //C1 At PORTB PIN_4
#define KEYPAD_u8C2_PIN DIO_u8PIN_22		  //C2 At PORTB PIN_5
#define KEYPAD_u8C3_PIN DIO_u8PIN_23		  //C3 At PORTB PIN_6
#define KEYPAD_u8C4_PIN DIO_u8PIN_24		  //C4 At PORTB PIN_7

#endif /* KEYPAD_CFG_H_ */
