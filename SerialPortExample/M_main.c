//----------------------------------------------------------------------------
//  Demo Application for MSP430/CC1100-2500 Interface Code Library v1.0
//  For EZ-RF2500 -- Roscoe Giles
// Basic operation is to push button 1 and send a message to the other board.
// When a message is sent, the green LED is toggled (LED2)
// When a message is received, the red LED is toggled. (LED 1)
//-----------------------------------------------------------------------------
//  Originally by
//  K. Quiring
//  Texas Instruments, Inc.
//  July 2006
//----------------------------------------------------------------------------

#include <stdlib.h>
#include "M_include.h"
#include "UART.h"
#include "M_DataLinkLayer.h"
#include "msp430x22x4.h"

unsigned int ADC_value = 0;

void ConfigureAdc(void);

extern char paTable[];
extern char paTableLen;
char txBuffer[70];
char rxBuffer[70];
unsigned int i;


int main(void) {
	WDTCTL = WDTPW + WDTHOLD;
	BCSCTL1 = CALBC1_1MHZ;
	DCOCTL = CALDCO_1MHZ;

	P2SEL |= BIT1;
	__enable_interrupt();

	setSerialPort();

	P2SEL &= ~(BIT7 + BIT6);

	TI_CC_SPISetup();
	TI_CC_PowerupResetCCxxxx();
	writeRFSettings();
	TI_CC_SPIWriteBurstReg(TI_CCxxx0_PATABLE, paTable, paTableLen);


	P1REN |= SW1_MASK;
	P1OUT |= SW1_MASK;
	P1IES = SW1_MASK;
	P1IFG &= ~(SW1_MASK);
	P1IE = SW1_MASK;


	P1DIR = LED1_MASK + LED2_MASK;

	TI_CC_GDO0_PxIES |= TI_CC_GDO0_PIN;
	TI_CC_GDO0_PxIFG &= ~TI_CC_GDO0_PIN;
	TI_CC_GDO0_PxIE |= TI_CC_GDO0_PIN;

	TI_CC_SPIStrobe(TI_CCxxx0_SRX);
	__bis_SR_register(GIE);
	sendString("Eoo, im alive");

	TURN_OFF_BOTH_LED;
	char counter = 0;
	sendString("Im alive - 3");
	TURN_ON_GREEN_LED;
	int A[1000];
	sendString("Im alive - 4");
	while (1) {
		if (BUTTON_PRESSED) {
			A[counter] = counter;
			sendString(A);
			sendMyMeasurementDLPacket(counter, A);
			counter++;
			TURN_ON_RED_LED;
			__delay_cycles(5000);
			TURN_OFF_RED_LED;
		}
	}
}



// Port 1 interrupt service routine
#pragma vector=PORT1_VECTOR
__interrupt void wakeUpFromPort1BUTTON(void) {
	__bic_SR_register_on_exit(CPUOFF);
	P1IFG &= ~(SW1_MASK);
}

