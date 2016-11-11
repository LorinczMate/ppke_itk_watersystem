//----------------------------------------------------------------------------
//  Description:  Master include file
//
//  Demo Application for MSP430/CC1100-2500 Interface Code Library v1.0
//
//  K. Quiring
//  Texas Instruments, Inc.
//  July 2006
//  IAR Embedded Workbench v3.41
//----------------------------------------------------------------------------


#include "TI_CC_CC2500.h"
#include "TI_CC_msp430.h"
#include "TI_CC_spi.h"
#include "TI_CC_hardware_board.h"
#include "CC2500.h"

// bit masks for P1 on the RF2500 target board
#define LED1_MASK              0x01 // == BIT0
#define LED2_MASK              0x02 // == BIT1
#define SW1_MASK               0x04 // == BIT2
#define TURN_ON_RED_LED		   P1OUT |= BIT0
#define TURN_OFF_RED_LED	   P1OUT &= ~BIT0
#define TURN_ON_GREEN_LED	   P1OUT |= BIT1
#define TURN_OFF_GREEN_LED	   P1OUT &= ~BIT1
#define TURN_ON_BOTH_LED       P1OUT |= (LED1_MASK+LED2_MASK)
#define TURN_OFF_BOTH_LED	   P1OUT &= ~BIT1; P1OUT &= ~BIT0
#define BUTTON_PRESSED		   !(P1IN & BIT2)
#define BUTTON_NOT_PRESSED	   (P1IN & BIT2)
#define LINE_BREAK			   sendString("\n\r")
#define DOUBLE_LINE_BREAK	   sendString("\n\r\n\r")
