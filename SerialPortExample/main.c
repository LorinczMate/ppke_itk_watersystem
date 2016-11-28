#include "msp430f2274.h"
#include "UART.h" // Saját header
#include "include.h"
int main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 // Stop WDT
	BCSCTL1 = CALBC1_1MHZ;          // Set DCO (Digitally controlled oscillator)
	DCOCTL = CALDCO_1MHZ;                     // Set DCO

	setSerialPort();

	P2SEL &= ~(BIT7 + BIT6); //clear select bits for XIN,XOUT, which are set by default (utolsó két bit, az SPI interfész használatához kell.)
	// Configure ports -- switch inputs, LEDs, GDO0 to RX packet info from CCxxxx

	P1REN |= SW1_MASK; //  enable pullups on SW1
	P1OUT |= SW1_MASK;

	// Configure LED outputs on port 1
	P1DIR = LED1_MASK + LED2_MASK; //Outputs

	// setup for interrupts related to receipt of a message from the CC2500

	TURN_OFF_BOTH_LED;

	__bis_SR_register(GIE);                   // Enter LPM3, enable interrupts
	while (1) {
		if (BUTTON_PRESSED) {
			sendString("1,2,0,8,7,14,433,89");
			LINE_BREAK;
			__delay_cycles(5000);
		}
	}
}

